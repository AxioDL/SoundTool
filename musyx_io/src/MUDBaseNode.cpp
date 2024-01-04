#include <mud/MUDBaseNode.h>
#include <mud/MUDAccessBaseInterface.h>
#include <mud/MUDRoot.h>
#include <rpc.h>
#include <time.h>

MUDBaseNode::MUDBaseNode(MUDRoot* dBase, MUDBaseNode* _parent, 
                         MUDBaseNode* _prevSibling, MUDBaseNode* _nextSibling, 
                         const f5::string& _name, u16 _id, const MUDTypeStorage* forcedInstanceID) {
  parent = _parent;
  prevSibling = _prevSibling;
  child = NULL;
  rootInterface = dBase;
  baseInterface = NULL;

  if (_prevSibling) {
    _prevSibling->nextSibling = this;
  }

  if (_nextSibling) {
    _nextSibling->prevSibling = this;
  }

  name = _name;
  id = _id;
  properties.SetDirectory(true);
  properties.SetReadOnly(false);
  properties.SetHidden(false);
  properties.SetSystem(false);
  properties.SetAccessActive(false);
  properties.SetModifyActive(false);

  GetDate(properties.GetCreatedDate());
  GetCurrentUser(properties.GetCreatedBy());

  properties.GetModifiedDate() = properties.GetCreatedDate();
  properties.GetModifiedBy() = properties.GetCreatedBy();

  if (forcedInstanceID) {
    instanceID = *forcedInstanceID;
  } else {
    RPC_STATUS ret = UuidCreate((GUID*)&instanceID);
    assert(ret == RPC_S_OK);
  }
  dBase->AddBaseNode(this);
}

bool MUDBaseNode::RequestModifyData() {
  if (properties.IsReadOnly()) {
    return false;
  } 
  GetDate(properties.GetModifiedDate());
  GetCurrentUser(properties.GetModifiedBy());
  return true;
}

bool MUDBaseNode::BeginModify() {
  if (properties.IsReadOnly()) {
    return false;
  }
  if (properties.IsModifyActive()) {
    return false;
  }

  properties.SetModifyActive(true);
  return true;
}

void MUDBaseNode::EndModify(MUDUndoInfo* info) {
  MUDUndoList::iterator it;

  if (GetRootInterface()->IsUndoEnabled()) {
    if (info) {
      MUDUndoEntry* undoEntry;
      if (static_cast<MUDRoot*>(rootInterface)->AddToUndoList(undoEntry)) {
        undoEntry->SetType(type);
        undoEntry->SetFlags(0);
        undoEntry->SetSpecial(info->GetSpecialType(), info->GetSpecial());
      } else {
        GetRootInterface()->ClearUndoList();

        if (typeInterface && info->GetSpecial()) {
          typeInterface->DiscardUndoInfo(info);
        }
      }
    } else {
      GetRootInterface()->ClearUndoList();
    }
  } else if (info && typeInterface && info->GetSpecial()) {
    typeInterface->DiscardUndoInfo(info);
  }

  static_cast<MUDRoot*>(rootInterface)->NodeModifyNotification(this);
}

bool MUDBaseNode::RequestAccess() {
  properties.SetAccessActive(true);
  return true;
}

void MUDBaseNode::EndAccess() {
  properties.SetAccessActive(false);
}


MUDBaseNodeInterface* MUDBaseNode::GetChild() const {
  return child;
}

MUDBaseNodeInterface* MUDBaseNode::GetSibling() const {
  return nextSibling;
}

MUDBaseNodeInterface* MUDBaseNode::GetParent() const {
  return parent;
}

const f5::string& MUDBaseNode::GetName() const {
  return name;
}

u16 MUDBaseNode::GetID() const {
  return id;
}

const MUDTypeStorage& MUDBaseNode::GetType() const {
  return type;
}

bool MUDBaseNode::IsType(const MUDTypeStorage& _type) const {
  return type == _type;
}

const MUDTypeStorage& MUDBaseNode::GetInstanceID() const {
  return instanceID;
}

MUDRootInterface* MUDBaseNode::GetRootInterface() const {
  return rootInterface;
}

MUDAccessBaseInterface* MUDBaseNode::GetTypeInterface() const {
  return typeInterface;
}

MPIBaseInterface* MUDBaseNode::GetBaseInterface() const {
  return baseInterface;
}

const MUDProperties& MUDBaseNode::GetProperties() const {
  return properties;
}


bool MUDBaseNode::AddReference(MUDBaseNodeInterface* nodeInterface) {
  assert(nodeInterface != NULL);

  MUDBaseNode* node = static_cast<MUDBaseNode*>(nodeInterface);

  if (RequestModifyData() && node != this) {
    referencedNodes.insert(node);


    node->nodesReferencing.insert(this);
    return true;
  }


  return false;
}

bool MUDBaseNode::RemoveReference(MUDBaseNodeInterface* nodeInterface) {
  MUDBaseNodeSet::iterator it;
  MUDBaseNode* node = static_cast<MUDBaseNode*>(nodeInterface);


  if (!RequestModifyData()) {
    return false;
  }


  if (node) {
    if (referencedNodes.Remove(node)) {
      node->nodesReferencing.Remove(this);
      return true;
    }

    return false;
  }


  for (it = referencedNodes.begin(); it != referencedNodes.end(); ++it) {
    (*it)->nodesReferencing.Remove(this);
  }

  referencedNodes.clear();

  return true;
}

bool MUDBaseNode::IsNodeReferenced(const MUDBaseNodeInterface* node) const {
  MUDBaseNodeSet::const_iterator it;

  for (it = referencedNodes.begin(); it != referencedNodes.end(); ++it) {
    if (*it == node) {
      return true;
    }
  }

  return false;
}

bool MUDBaseNode::IsNodeReferenced(u16 id) const {
  MUDBaseNode* node = static_cast<MUDBaseNode*>(rootInterface->FindNode(id));

  if (!node) {
    return false;
  }


  MUDBaseNodeSet::const_iterator it;
  for (it = referencedNodes.begin(); it != referencedNodes.end(); ++it) {
    if (*it == node) {
      return true;
    }
  }

  return false;
}

MUDBaseNodeInterfaceList MUDBaseNode::GetReferencedNodes() const {
  MUDBaseNodeInterfaceList list;

  MUDBaseNodeSet::const_iterator it;

  for (it = referencedNodes.begin(); it != referencedNodes.end(); ++it) {
    list.push_back(*it);
  }

  return list;
}

size_t MUDBaseNode::GetNumReferencedNodes() const {
  return referencedNodes.size(); 
}

MUDBaseNodeInterfaceList MUDBaseNode::GetNodesReferencing() const {
  MUDBaseNodeInterfaceList list;

  MUDBaseNodeSet::const_iterator it;

  for (it = nodesReferencing.begin(); it != nodesReferencing.end(); ++it) {
    list.push_back(*it);
  }

  return list;
}


size_t MUDBaseNode::GetNumNodesReferencing() const {
  return nodesReferencing.size(); 
}

void MUDBaseNode::GetDate(MUDDate& date) {
  tm *lt; 
  time_t ti; 
  time(&ti);
  lt = localtime(&ti);
  date = MUDDate(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900, lt->tm_hour, lt->tm_min, lt->tm_sec);
}

void  MUDBaseNode::GetCurrentUser(f5::string& user) {
  char name[512];
  u32 len;
  if (WNetGetUserA(0, name, &len) != 0) {
    user = "unknown";
  } else {
    user = name;
  }
}