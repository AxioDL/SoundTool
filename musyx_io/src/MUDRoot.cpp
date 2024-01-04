#include <mud/MUDRoot.h>

#include <mud/MUDAccessBaseInterface.h>
#include <mud/MUDDeletedNode.h>
#include <mud/MUDIDTransInfo.h>
#include <mud/MUDModificationInfo.h>
#include <mud/MUDUndoGroup.h>
#include <mud/MUDUndoSpecial.h>


#include <mpi/MPIBaseInterface.h>
#include <mpi/MPIManagerInterface.h>

#include <diskio.h>

#include <rpc.h>

DEFINE_GUID(neutralType, 0x71093C48, 0xFE30, 0x49A5, 
            0xBE, 0xFA, 0xB6, 0x9C, 0x67, 0x5D, 0xEB, 0xDB);
DEFINE_GUID(dbSubTypeDolphin, 0x600B51BC, 0x928A, 0x4AA1, 
            0x87, 0xBC, 0x8A, 0xD8, 0x70, 0xD5, 0x69, 0x88);
DEFINE_GUID(dbSubtypePC,0x2855BEBF, 0x1ECF, 0x41A4,
            0xAD, 0x66, 0xDB, 0x3D, 0xD0, 0x4D, 0xB3, 0x64);

std::list<MUDRoot::MUDRootEntry> MUDRoot::dataBaseList;

MUDRoot::MUDRoot(MPIManagerInterface* _mpi, const GUID* _subType) {
  unsigned char* subtypeStr;
  UuidToString((GUID*)_subType, &subtypeStr);
  printf("Creating MUDRoot with MPI %p and subtype %s\n", _mpi, subtypeStr);
  RpcStringFree(&subtypeStr);

  state = MUD_STATE_OK;
  rootNode = NULL;
  mpi = _mpi;
  idStart = 0;
  exportEndian = MUD_ENDIAN_1;
  undoList.SetEnabled(true);
  undoList.SetUndoGroup(NULL);
  macroSetupNode = NULL;
  sampleSetupNode = NULL;
  shutdownActive = false;
  nodeCallbackList.SetModificationNotifyCallback(NULL);
  nodeCallbackList.SetModificationNotifyEnabled(false);
  subType = *_subType;
}

MUDRoot::~MUDRoot() {
  ModificationNotify(MUDModificationInfo(4, -1));

  if (!undoList.empty()) {
    ClearUndoList();
  }

  if (rootNode) {
    DeleteNode(rootNode, true);
  }
}

bool MUDRoot::IsShutDownActive() {
  return shutdownActive;
}

bool MUDRoot::Undo() {
  MUDUndoInfo undoInfo;
  MUDUndoEntry* undoEntry;
  MUDUndoList::iterator it;
  MUDUndoGroup::iterator itUG;
  MUDUndoSpecial* undoSpecial;
  MUDBaseNode* node;
  MUDBaseNode* parent;
  MUDAccessBaseInterface* pInterface;
  f5::string oldName;

  if (!IsUndoEnabled()) {
    return false;
  }

  DisplayUndoList();
  if (undoList.empty()) {
    return true;
  }

  MUDUndoGroup* undoGroup = undoList.back();

  itUG = undoGroup->end()--;

  for (u32 i = undoGroup->size(); i > 0; --itUG, --i) {
    undoEntry = (*itUG);
    undoInfo = MUDUndoInfo(undoEntry->GetSpecial(), undoEntry->GetSpecialType());

    if (undoEntry->GetFlags() & 1) {
      switch(undoEntry->GetSpecialType()) {
      case MUD_UNDO_SPECIAL_SPECIAL: {
        undoSpecial = undoEntry->GetUndoSpecial();

        if (!undoSpecial->node->HasParent()) {
          parent = NULL;
        } else {
          parent = static_cast<MUDBaseNode*>(GetNodeFromInstanceID(undoSpecial->node->GetParentInstanceID(), NULL));
          if (!parent) {
            ClearUndoList();
            return false;
          }
        }

        MUDBaseNodeSet refList;
        MUDBaseNodeSet refByList;

        MUDBaseNode* selfReference = NULL;
        std::list<MUDTypeStorage>::const_iterator it;
        for (it = undoSpecial->node->GetReferencedNodes().begin(); it != undoSpecial->node->GetReferencedNodes().end() ; ++it) {
          assert(*it != undoEntry->GetInstanceID());

          MUDBaseNode* refNode = static_cast<MUDBaseNode*>(GetNodeFromInstanceID(*it, NULL));

          if (!refNode) {
            ClearUndoList();
            return false;
          }

          refList.Add(refNode);
        }

        assert(undoSpecial->node->GetNodesReferencing().empty());
        DisableUndo();
        node = static_cast<MUDBaseNode*>(CreateNode(parent, undoSpecial->node->GetName(), undoEntry->GetType(), undoEntry->GetInstanceID()));

        if (!node) {
          EnableUndo();
          ClearUndoList;
          return false;
        }

        undoInfo = MUDUndoInfo(undoSpecial->special, undoSpecial->specialType);

        node->referencedNodes = refList;
        MUDBaseNodeSet::const_iterator itL;
        for (itL = refList.begin(); itL != refList.end(); ++itL) {
          (*itL)->nodesReferencing.Add(node);
        }

        EnableUndo();
        pInterface = node->GetTypeInterface();
      }break;
      case MUD_UNDO_SPECIAL_VOID:
        undoSpecial = undoEntry->GetUndoSpecial();
        continue;
      case MUD_UNDO_SPECIAL_STRING:
        node = static_cast<MUDBaseNode*>(GetNodeFromInstanceID(undoEntry->GetInstanceID(), NULL));
        if(!node) {
          ClearUndoList();
          return false;
        }

        oldName = node->GetName();
        SetNodeName(node, *undoEntry->GetStringSpecial());
        if (undoEntry->GetType() != mudTypeBaseNode) {
          if (!node->GetTypeInterface()->Rename(node, oldName)) {
            ClearUndoList();
            return false;
          }
        }
        continue;
      default:
        assert(false);
      }
    } else {
      node = static_cast<MUDBaseNode*>(GetNodeFromInstanceID(undoEntry->GetInstanceID(), NULL));

      if (!node) {
        ClearUndoList();
        return false;
      }

      if (node->GetType() != undoEntry->GetType()) {
        ClearUndoList();
        return false;
      }
      pInterface = node->GetTypeInterface();
    }
    u16 id = node->GetID();

    if (!pInterface->DoUndo(&undoInfo)) {
      ClearUndoList();
      return false;
    }

    if (FindNode(id)) {
      MUDModificationInfo modInfo(2, id);

      ModificationNotify(modInfo);
    }
  }

  RemoveFromUndoList();
  return true;
}

MUDBaseNodeInterface* MUDRoot::CreateNode(MUDBaseNodeInterface* parentInterface, const f5::string& name, 
                                          const MUDTypeStorage* type, const MUDTypeStorage* forcedInstanceID) {
  MUDBaseNode* parent = static_cast<MUDBaseNode*>(parentInterface);
  MUDBaseNode* node = NULL;

  if (!parentInterface || parent->IsDirectory()) {
    MUDBaseNode* sibling = static_cast<MUDBaseNode*>(parent ? parent->GetChild() : rootNode);
    if (FindNode(name)) {
      SetState(MUD_STATE_NODE_DUPLICATED);
    } else {
      node = new MUDBaseNode(this, parent, NULL, sibling, name, MakeID(), forcedInstanceID);
    }


    if (node) {
      if (parent) {
        parent->SetChild(node);
      } else {
        rootNode = node;
      }

      if (type) {
        if (SetNodeType(node, type)) {
          node->SetDirectory(false);
        } else {
          delete node;
          node = NULL;
        }
      }

      if (node) {
        MUDModificationInfo modInfo(0, node->GetID());
        ModificationNotify(modInfo);
        SetState(MUD_STATE_OK);
      }
    } else {
      SetState(MUD_STATE_OUT_OF_MEMORY);
    }

    return node;
  } else {
    SetState(MUD_STATE_PARENT_NOT_DIRECTORY);
  }
  return NULL; 
}

MUDBaseNodeInterface* MUDRoot::CreateNode(MUDBaseNodeInterface* parentInterface, const f5::string& name, 
                                          const MUDTypeStorage* type) { 
  return MUDRoot::CreateNode(parentInterface, name, type, NULL);
}

bool MUDRoot::DeleteNode(MUDBaseNodeInterface* node, bool deleteSiblings) const { return false; }

bool MUDRoot::DeleteNode(MUDBaseNodeInterface* node) const { return DeleteNode(node, false); }

u16 MUDRoot::MakeID() {
  u16 id;
  do {
    id = idStart;
    ++idStart;
    if (id == 0xFFFF) {
      id = idStart;
      ++idStart;
    }
  } while (FindNode(id));

  return id;
}

MUDBaseNodeInterface* MUDRoot::GetRootNode() {
  return rootNode;
}

const MUDBaseNodeInterface* MUDRoot::GetRootNode() const {
  return rootNode;
}

MUDBaseNodeInterface* MUDRoot::CopyNode(const MUDBaseNodeInterface* node, const f5::string& name, MUDBaseNodeInterface* parent) {
  MUDBaseNodeInterface* newNode = CreateNode(parent, name, node->GetType());

  if (!newNode || newNode->GetTypeInterface()->Copy(node)) {
    return newNode;
  } else {
    DeleteNode(newNode);
  }

  return NULL;
}

MUDBaseNodeInterface* MUDRoot::GetNodeFromInstanceID(const MUDTypeStorage& instID, MUDBaseNodeInterface* nodeInterface) {
  MUDBaseNode* node = static_cast<MUDBaseNode*>(nodeInterface);

  if (nodeInterface || (node = static_cast<MUDBaseNode*>(rootNode))) {
    if (node->GetInstanceID() == instID) {
      return node;
    }
    MUDBaseNodeInterface* res;
    if (node->GetChild() && (res = GetNodeFromInstanceID(instID, node->GetChild()))) {
      return res;
    }

    if (node->GetSibling() && (res = GetNodeFromInstanceID(instID, node->GetSibling()))) {
      return res;
    }
  }

  return NULL;
}

MUDBaseNodeInterface* MUDRoot::FindNode(const f5::string& name) {
  return baseNodeFinder.find(name);
}

const MUDBaseNodeInterface* MUDRoot::FindNode(const f5::string& name) const {
  return baseNodeFinder.find(name);
}


MUDBaseNodeInterface* MUDRoot::FindNode(u16 id) {
  return baseNodeFinder.find(id);
}

const MUDBaseNodeInterface* MUDRoot::FindNode(u16 id) const {
  return baseNodeFinder.find(id);
}

MUDBaseNodeInterfaceList MUDRoot::FindNode(const MUDTypeStorage* type) const {
  return baseNodeFinder.find(type);
}

MUDBaseNodeInterface* MUDRoot::GetMacroSetupNode() {
  return macroSetupNode;
}

const MUDBaseNodeInterface* MUDRoot::GetMacroSetupNode() const {
  return macroSetupNode;
}


MUDBaseNodeInterface* MUDRoot::GetSampleSetupNode() {
  return sampleSetupNode;
}

const MUDBaseNodeInterface* MUDRoot::GetSampleSetupNode() const {
  return sampleSetupNode;
}


bool MUDRoot::SetSampleSetupNode(MUDBaseNodeInterface* sampleSetup) {
  if (sampleSetupNode && sampleSetupNode != sampleSetup) {
    return false;
  }

  sampleSetupNode = sampleSetup;
  return true;
}

bool MUDRoot::SetMacroSetupNode(MUDBaseNodeInterface* macroSetup) {
  if (macroSetupNode && macroSetupNode != macroSetup) {
    return false;
  }

  macroSetupNode = macroSetup;
  return true;
}

MUDState MUDRoot::GetState() const {
  return state;
}

static const char* stateTab[] = {
   "Ok",
   "Unknown node type",
   "Out of memory",
   "Node name duplicated in subtree",
   "Node is still referenced",
   "Could not create node",
   "No ID translation information available",
   "Specified parent node is not a directory",
   "Node still has at least one child",
   "Node is read only",
   "Node is still being accessed by another caller",
   "Node is still being modified by another caller",
   "Could not read file",
   "Bad file format",
   "Could not write file",
   "Could not create file",
   "Unsupported file version",
   "Cannot change database subtype",
   "Rename of node is disallowed",
   "Database is of different subtype than loaded data",
   "Singular node type has been duplicated",
};

bool MUDRoot::GetLastError(f5::string& text) const {
  text = stateTab[state];
  return state != MUD_STATE_OK;
}

void MUDRoot::SetState(MUDState _state) {
  state = _state;
}

void MUDRoot::EnableIDTranslation() {
  DisableIDTranslation();

  transInfo = new MUDIDTransInfo();

  assert(transInfo != NULL);
}

void MUDRoot::DisableIDTranslation() {
  if (transInfo) {
    delete transInfo;
    transInfo = NULL;
  }
}


f5::string MUDRoot::MakeTempNodeName() {
  GUID id;
  char buffer[256];
  UuidCreate(&id);
  sprintf(buffer, "tmp%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x", 
          id.Data1, id.Data2, id.Data3, 
          id.Data4[0], id.Data4[1], id.Data4[2], 
          id.Data4[3], id.Data4[4], id.Data4[5], 
          id.Data4[6], id.Data4[7]);

  return f5::string(buffer);
}

bool MUDRoot::ForceIDTranslation(u16 oldID, u16 newID, MUDIDType idType) {
  if (!transInfo) {
    return false;
  }

  switch(idType) {
  case MUD_ID_TYPE_0:
    transInfo->field_18.Add(oldID, newID);
    break;
  case MUD_ID_TYPE_1:
    transInfo->field_0.Add(oldID, newID);
    break;
  case MUD_ID_TYPE_2:
    transInfo->field_C.Add(oldID, newID);
    break;
  case MUD_ID_TYPE_3:
    transInfo->field_24.Add(oldID, newID);
    break;
  case MUD_ID_TYPE_4:
    transInfo->field_30.Add(oldID, newID);
    break;
  }
  return true;
}

u16 MUDRoot::ImportIDReference(u16 oldID,MUDIDType idType) {
  if (!transInfo) {
    SetState(MUD_STATE_NO_ID_TRANSLATION_INFO);
    return -1;
  }

  MUDIDTransList* tl;

  switch(idType) {
  case MUD_ID_TYPE_0:
    tl = &transInfo->field_18;
    break;
  case MUD_ID_TYPE_1:
    tl = &transInfo->field_0;
    break;
  case MUD_ID_TYPE_2:
    tl = &transInfo->field_C;
    break;
  case MUD_ID_TYPE_3:
    tl = &transInfo->field_24;
    break;
  case MUD_ID_TYPE_4:
    tl = &transInfo->field_30;
    break;
  }

  u16 id = tl->GetMUD(oldID);

  if (id == 0xFFFF) {
    f5::string tmpName = MakeTempNodeName();
    MUDBaseNodeInterface* node = CreateNode(NULL, tmpName, NULL);

    if (!node) {
      return -1;
    }

    SetHidden(node, true);
    id = node->GetID();
    tl->Add(oldID, id);
  }

  return id;
}

bool MUDRoot::SendParentRenameNotification(MUDBaseNodeInterface* nodeInterface) {
  if (!nodeInterface) {
    return true;
  }

  if (nodeInterface->GetType() != mudTypeBaseNode && !nodeInterface->GetTypeInterface()->ParentHasBeenRenamed()) {
    return false;
  }

  if (!SendParentRenameNotification(nodeInterface->GetSibling())) {
    return false;
  }

  if (!SendParentRenameNotification(nodeInterface->GetChild())) {
    return false;
  }


  return true;
}

bool MUDRoot::RenameNode(MUDBaseNodeInterface* nodeInterface, const f5::string& _name) {
  return false;
}

bool MUDRoot::SetNodeName(MUDBaseNodeInterface* nodeInterface, const f5::string& _name) {
  assert(nodeInterface != NULL);

  MUDBaseNode* node = static_cast<MUDBaseNode*>(nodeInterface);

  if (nodeInterface->RequestModifyData()) {

    if (!FindNode(_name)) {
      f5::string oldName = node->GetName();
      node->SetName(_name);

      baseNodeFinder.rename(node, oldName);
      return true;
    }

    SetState(MUD_STATE_NODE_DUPLICATED);
  } else {
    SetState(MUD_STATE_NODE_READ_ONLY);
  }
  return true;
}

bool MUDRoot::SetNodeType(MUDBaseNodeInterface* nodeInterface, const MUDTypeStorage* type) {
  assert(nodeInterface != NULL);

  MUDBaseNode* node = static_cast<MUDBaseNode*>(nodeInterface);

  if (!nodeInterface->RequestModifyData()) {
    SetState(MUD_STATE_NODE_READ_ONLY);
    return false;
  }


  if (node->GetTypeInterface()) {
    node->GetBaseInterface()->ReleaseInterface(type, node->GetTypeInterface());
    node->SetTypeInterface(NULL);
    node->SetBaseInterface(NULL);
  }

  if (!type) {
    node->SetType(mudTypeBaseNode);
    return true;
  }

  MPIBaseInterface* base = node->GetBaseInterface();
  node->SetTypeInterface(reinterpret_cast<MUDAccessBaseInterface*>(mpi->FindInterface(type, GetDataBaseSubType(), base)));

  if (!node->GetTypeInterface()) {
    SetState(MUD_STATE_UNKNOWN_NODE_TYPE);
    return false;
  }

  if (!node->GetChild()) {
    SetDirectory(node, false);
  }


  bool singularTypeDuplicated;
  if (node->GetTypeInterface()->Init(node, singularTypeDuplicated)) {
    node->SetType(*type);
    return true;
  }

  if (singularTypeDuplicated) {
    SetState(MUD_STATE_SINGULAR_NODE_TYPE_DUPLICATED);
  } else {
    SetState(MUD_STATE_NODE_COULD_NOT_CREATE);
  }

  node->GetBaseInterface()->ReleaseInterface(type, node->GetTypeInterface());
  node->SetBaseInterface(NULL);
  node->SetTypeInterface(NULL);
  return false;
}


bool MUDRoot::MoveNode(MUDBaseNodeInterface* nodeInterface, MUDBaseNodeInterface* newParentInterface) {
  assert(nodeInterface != NULL);
  MUDBaseNode* node = static_cast<MUDBaseNode*>(nodeInterface);
  MUDBaseNode* newParent = static_cast<MUDBaseNode*>(newParentInterface);

  if (nodeInterface->GetType() == mudTypeBaseNode || nodeInterface->GetTypeInterface()->Move(nodeInterface, newParentInterface)) {
    if (node->GetPrevSibling()) {
      static_cast<MUDBaseNode*>(node->GetPrevSibling())->SetSibling(node->GetSibling());
    } else if (node->GetParent()) {
      static_cast<MUDBaseNode*>(node->GetParent())->SetChild(node->GetSibling());
    } else {
      rootNode = node->GetSibling();
    }

    if (node->GetSibling()) {
      static_cast<MUDBaseNode*>(node->GetSibling())->SetPrevSibling(node->GetPrevSibling());
    }

    if (newParent) {
      node->SetSibling(newParent->GetChild());
      if (node->GetSibling()) {
        static_cast<MUDBaseNode*>(node->GetSibling())->SetPrevSibling(node);
      }

      node->SetPrevSibling(NULL);
      node->SetParent(newParent);
      newParent->SetChild(node);
    } else {
      node->SetSibling(rootNode);

      if (node->GetSibling()) {
        static_cast<MUDBaseNode*>(node->GetSibling())->SetPrevSibling(node);
      }

      node->SetPrevSibling(NULL);
      node->SetParent(NULL);
      rootNode = node;
    }

    return true;
  }

  return false;
}

MUDBaseNodeInterface* MUDRoot::ImportNode(MUDBaseNodeInterface *,const f5::string&,const MUDTypeStorage*, u16, MUDIDType) {
  return NULL;
}

bool MUDRoot::SetHidden(MUDBaseNodeInterface* node, bool flag) {
  static_cast<MUDBaseNode*>(node)->GetProperties().SetHidden(flag);
  return true;
}

bool MUDRoot::DeleteHiddenNodes(MUDBaseNodeInterface* node) const {
  bool undoState;
  bool first = false;
  if (!node) {
    undoState = IsUndoEnabled();
    node = const_cast<MUDRoot*>(this)->GetRootNode();
    const_cast<MUDRoot*>(this)->DisableUndo();
    first = true;
  }

  if (static_cast<MUDBaseNode*>(node)->GetProperties().IsHidden()) {
    DeleteNode(node);
  }

  if (node->GetSibling()) {
    DeleteHiddenNodes(node->GetSibling());
  }

  if (node->GetChild()) {
    DeleteHiddenNodes(node->GetChild());
  }

  if (first && undoState) {
    const_cast<MUDRoot*>(this)->EnableUndo();
  }

  return true;
}


bool MUDRoot::SetDirectory(MUDBaseNodeInterface* node, bool flag) {
  if (!static_cast<MUDBaseNode*>(node)->GetChild() || flag) {
      static_cast<MUDBaseNode*>(node)->GetProperties().SetDirectory(flag);
      return true;
  } 
  SetState(MUD_STATE_NODE_HAS_CHILDREN);
  return false;
}

bool MUDRoot::SetSystem(MUDBaseNodeInterface* node, bool flag) {
  static_cast<MUDBaseNode*>(node)->GetProperties().SetSystem(flag);
  return true;
}

bool MUDRoot::SetReadOnly(MUDBaseNodeInterface* node, bool flag) {
  static_cast<MUDBaseNode*>(node)->GetProperties().SetReadOnly(flag);
  return true;
}

void MUDRoot::SetExportEndian(MUDEndian endian) {
  exportEndian = endian;
}

MUDEndian MUDRoot::GetExportEndian() const {
  return exportEndian;
}

void MUDRoot::SetDefaultSampleType(MUDSampleType type) {
  defaultSampleType = type;
}

void MUDRoot::OnlyDefaultSampleType(bool on) {
  onlyDefaultSampleType = on;
}

MUDSampleType MUDRoot::GetDefaultSampleType() const {
  return defaultSampleType;
}

bool MUDRoot::GetOnlyDefaultSampleType() const {
  return onlyDefaultSampleType;
}

const f5::string& MUDRoot::GetFilename() const {
  return filename;
}

const f5::string& MUDRoot::GetCurrentLoadLocation() const {
  return currentLoadLocation;
}

void MUDRoot::SetFilename(const f5::string& name) {

  if (!DIOFile::ExpandPath(name, filename)) {
    filename = name;
  }

  DIOFile::CleanupPath(filename);
}

bool MUDRoot::SetLoadNodeCallback(const MUDTypeStorage& type, MUDLoadNodeCallback callback) {
  
  if (callback) {
    nodeCallbackList.insert(MUDNodeCallback(type, callback));
  } else {
    MUDNodeCallbackList::iterator it = nodeCallbackList.find(type);
    if (it != nodeCallbackList.end()) {
      nodeCallbackList.erase(it);
    }
  }

  return true;
}


bool MUDRoot::Load(const f5::string& filename) {
  return false;
}

bool MUDRoot::Save(const f5::string& filename) {
  return false;
}

bool MUDRoot::SaveTree(DIOFile* file,const f5::string& path,const MUDBaseNodeInterface* node, bool setup) const {
  return false;
}

const GUID* MUDRoot::GetDataBaseSubType() const {
  return &subType;
}

bool MUDRoot::SetDataBaseSubType(const GUID* _subType) {
  if (IsEqualGUID(subType, neutralType)) {
    SetState(MUD_STATE_CANNOT_CHANGE_DATABASE_SUBTYPE);
    return false;
  }

  subType = *_subType;

  return true;
}