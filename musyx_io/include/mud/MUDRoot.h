#ifndef _MUDROOT_H_
#define _MUDROOT_H_

#include <mud/MUDRootInterface.h>
#include <mud/MUDUndoList.h>
#include <mud/MUDUndoGroup.h>
#include <mud/MUDNodeCallbackList.h>
#include <mud/MUDBaseNode.h>
#include <mud/MUDModificationInfo.h>

#include <set>
#include <vector>

class MUDIDTransInfo;
class MPIManagerInterface;

static const GUID neutralType;
static const GUID dbSubTypeDolphin;

class DIOFile;
class MUDRoot : public MUDRootInterface {
public:
  class MUDRootEntry {
  public:
    MUDRootEntry(MUDRoot* db, const f5::string& _name) {
      name = _name;
      node = db;
    }

    const f5::string& GetName() const { return name; }
    MUDRootInterface* GetNode() const { return node; }
  private:
    f5::string name;
    MUDRootInterface* node;
  };

  class MUDBaseNodeFinder {
  public:
    class MUDBaseNodeNameRef;
    class MUDBaseNodeNameRefCmp {
    public:
      bool operator()(const MUDBaseNodeNameRef& a, const MUDBaseNodeNameRef& b) const {
        return a.GetName().str() < b.GetName().str();
      }
    };
    class MUDBaseNodeNameRef {
    public:
      MUDBaseNodeNameRef(const f5::string& _name) {
        name = _name;
        node = NULL;
      }
      MUDBaseNodeNameRef(MUDBaseNode* _node) {
        name = _node->GetName();
        node = _node;
      }

      const f5::string& GetName() const { return name; }
      MUDBaseNode* GetNode() const { return node; }
      void SetNode(MUDBaseNode* _node) { node = _node; }
    private:
      f5::string name;
      MUDBaseNode* node;
    };


    typedef std::set<MUDBaseNodeNameRef, MUDBaseNodeNameRefCmp> SetType;
    MUDBaseNode* find(const f5::string& name) const {
      SetType::const_iterator it = set.find(name);

      if (it != set.end()) {
        return it->GetNode();
      }

      return NULL;
    }

    typedef std::vector<MUDBaseNode*> ListType;
    MUDBaseNode* find(u16 id) {
      if (id != 0xFFFF && id < list.size()) {
        return list[id];
      }

      return NULL;
    }


    MUDBaseNodeInterfaceList find(const MUDTypeStorage* type) const {
      MUDBaseNodeInterfaceList res;
      SetType::const_iterator it;

      for (it = set.begin(); it != set.end(); ++it) {
        if (it->GetNode()->IsType(*type)) {
          res.push_back(it->GetNode());
        }
      }

      return res;
    }

    void remove(const MUDBaseNode* node) {

      SetType::iterator it = set.find(node->GetName());

      if (it != set.end()) {
        set.erase(it);
      }

      u16 id = node->GetID();

      if (id == list.size() - 1) {
        list.resize(id);
      } else {
        list[id] = NULL;
      }
    }

    void rename(MUDBaseNode* node, const f5::string& oldName) {
      SetType::iterator it = set.find(oldName);
      if (it != set.end()) {
        set.erase(it);
        set.insert(node);
      }
    }

    void add(MUDBaseNode* node) {
      std::pair<SetType::iterator, bool> res1 = set.insert(MUDBaseNodeNameRef(node));

      if (!res1.second) {
        res1.first->SetNode(node);
      }

      u16 id = node->GetID();
      size_t i = list.size();

      if (id >= i) {

        list.resize(id + 1);

        while (i < id) {
          list[i] = NULL;
        }
      }

      list[id] = node;
    }
  private:
    SetType set;
    ListType list;
  };

  MUDRoot(MPIManagerInterface* _mpi, const GUID* _subType);
  ~MUDRoot();
  MUDBaseNodeInterface* CreateNode(MUDBaseNodeInterface* parentInterface, const f5::string& name, const MUDTypeStorage* type, const MUDTypeStorage* forcedInstanceID);
  MUDBaseNodeInterface* CreateNode(MUDBaseNodeInterface* parentInterface, const f5::string& name, const MUDTypeStorage* type);
  bool DeleteNode(MUDBaseNodeInterface* node, bool deleteSiblings) const;
  bool DeleteNode(MUDBaseNodeInterface* node) const;
  bool DeleteHiddenNodes(MUDBaseNodeInterface * node) const;
  const MUDBaseNodeInterface* GetRootNode() const;
  MUDBaseNodeInterface* GetRootNode();
  MUDBaseNodeInterfaceList FindNode(const MUDTypeStorage* type) const;
  const MUDBaseNodeInterface* FindNode(u16 id) const;
  MUDBaseNodeInterface* FindNode(u16 id);
  const MUDBaseNodeInterface* FindNode(const f5::string& name) const;
  MUDBaseNodeInterface* FindNode(const f5::string& name);
  bool RenameNode(MUDBaseNodeInterface* nodeInterface, const f5::string& _name);
  bool SetNodeType(MUDBaseNodeInterface* nodeInterface,const MUDTypeStorage* type);
  bool SetHidden(MUDBaseNodeInterface* node, bool flag);
  bool SetDirectory(MUDBaseNodeInterface* node, bool flag);
  bool SetReadOnly(MUDBaseNodeInterface* node, bool flag);
  bool SetSystem(MUDBaseNodeInterface* node, bool flag);
  bool MoveNode(MUDBaseNodeInterface * nodeInterface, MUDBaseNodeInterface* newParentInterface);
  MUDBaseNodeInterface* CopyNode(const MUDBaseNodeInterface* node, const f5::string& name,MUDBaseNodeInterface * parent);
  MUDState GetState() const;
  bool GetLastError(f5::string& text) const;
  void EnableIDTranslation();
  void DisableIDTranslation();
  u16 ImportIDReference(u16 oldID, MUDIDType type);
  bool ForceIDTranslation(u16 oldID, u16 newID, MUDIDType idTypey);
  MUDBaseNodeInterface* ImportNode(MUDBaseNodeInterface* parent, const f5::string& name, const MUDTypeStorage* type, u16 oldID, MUDIDType idType);
  bool Load(const f5::string& filename);
  bool Save(const f5::string& filename);
  bool SaveTree(DIOFile* file,const f5::string& path,const MUDBaseNodeInterface* node, bool setup) const;
  const f5::string& GetFilename() const;
  void SetFilename(const f5::string& filename);
  const f5::string& GetCurrentLoadLocation() const;
  void SetExportEndian(MUDEndian endian);
  MUDEndian GetExportEndian() const;
  void SetDefaultSampleType(MUDSampleType type);
  void OnlyDefaultSampleType(bool on);
  MUDSampleType GetDefaultSampleType() const;
  bool GetOnlyDefaultSampleType() const;
  const MUDBaseNodeInterface* GetSampleSetupNode() const;
  MUDBaseNodeInterface* GetSampleSetupNode();
  const MUDBaseNodeInterface* GetMacroSetupNode() const;
  MUDBaseNodeInterface* GetMacroSetupNode();
  bool SetSampleSetupNode(MUDBaseNodeInterface * sampleSetup);
  bool SetMacroSetupNode(MUDBaseNodeInterface* macroSetup);
  bool SetLoadNodeCallback(const MUDTypeStorage& type, MUDLoadNodeCallback callback);
  bool EnableUndo() { 
    undoList.SetEnabled(true); 
    return true;
  }
  bool DisableUndo() { 
    undoList.SetEnabled(false); 
    return true;
  }
  bool IsUndoEnabled() const { return undoList.IsEnabled(); }

  bool Undo();
  bool GetCurrentUndoDescription(f5::string& description) const {
    if (undoList.empty()) {
      return false;
    }

    description = undoList.back()->GetDescription();
    return true;
  }

  bool StartUndoGroup() {
    if (!undoList.IsEnabled()) {
      return false;
    }

    undoList.SetUndoGroup(new MUDUndoGroup());
    return undoList.GetUndoGroup() != NULL;
  }
  bool StopUndoGroup(const f5::string& description) {
    if (undoList.IsEnabled()) {
      if (undoList.empty()) {
        delete undoList.GetUndoGroup();
        undoList.SetUndoGroup(NULL);
        return true;
      }

      if (description.str().empty()) {
        undoList.GetUndoGroup()->SetDescription(undoList.GetUndoGroup()->back()->GetDescription());
      } else {
        undoList.GetUndoGroup()->SetDescription(description);
      }
      undoList.push_back(undoList.GetUndoGroup());
      undoList.SetUndoGroup(NULL);
      return true;
    }

    return false;
  }

  bool RemoveFromUndoList() {
    undoList.FreeLastEntry();
    return true;
  }
  bool ClearUndoList() {
    undoList.FreeAllEntries();
    return true;
  }
  bool DisplayUndoList() const {
    std::list<MUDUndoGroup*>::const_iterator it;
    std::list<MUDUndoEntry*>::const_iterator itUG;

    if (undoList.empty()) {
      return true;
    }

    for (it = undoList.begin(); it != undoList.end(); ++it) {
      for (itUG = (*it)->begin(); itUG != (*it)->end(); ++itUG) {
      }
    }
    return true;
  }

  void NodeModifyNotification(MUDBaseNodeInterface* node) {
    ModificationNotify(MUDModificationInfo(2, node->GetID()));
  }

  void SetModifyNotificationCallback(MUDModifyNotificationCallback callback) {
    nodeCallbackList.SetModificationNotifyCallback(callback);
  }
  MUDBaseNodeInterface * GetNodeFromInstanceID(const MUDTypeStorage& instID, MUDBaseNodeInterface* node);
  bool IsShutDownActive();
  void ShutDownActive() { shutdownActive = true; }
  const GUID* GetDataBaseSubType() const;
  bool SetDataBaseSubType(const GUID* _subType);
  virtual bool SetNodeName(MUDBaseNodeInterface* nodeInterface, const f5::string& _name);

  void ModificationNotify(const MUDModificationInfo& modInfo) {
    if (nodeCallbackList.GetModificationNotifyCallback() && nodeCallbackList.IsModificationNotifyEnabled()) {
      nodeCallbackList.GetModificationNotifyCallback()(this, modInfo);
    }
  }

  void EnableModificationCallback() {
    nodeCallbackList.SetModificationNotifyEnabled(true);
  }

  void DisableModificationCallback() {
    nodeCallbackList.SetModificationNotifyEnabled(false);
  }
  void SetState(MUDState _state);
  u16 MakeID();


  static std::list<MUDRootEntry>& GetDataBaseList() { return dataBaseList; }
  f5::string MakeTempNodeName();
  bool MUDRoot::SendParentRenameNotification(MUDBaseNodeInterface* nodeInterface);

  void AddBaseNode(MUDBaseNode* node) { baseNodeFinder.add(node); }

  bool AddToUndoList(MUDUndoEntry*& undoEntry) {
    if (undoList.GetUndoGroup()) {
      undoList.GetUndoGroup()->push_back(new MUDUndoEntry(this));

      undoEntry = undoList.GetUndoGroup()->back();
      if (undoEntry) {
        return true;
      }

      undoList.GetUndoGroup()->pop_back();
    }

    return false;
  }


  MPIManagerInterface* GetManagerInterface() { return mpi; }
  const MPIManagerInterface* GetManagerInterface() const { return mpi; }
private:
  static std::list<MUDRootEntry> dataBaseList;

  MUDState state;
  MUDBaseNodeInterface* rootNode;
  MUDBaseNodeFinder baseNodeFinder;
  f5::string filename;
  f5::string currentLoadLocation;
  MUDEndian exportEndian;
  MUDSampleType defaultSampleType;
  bool onlyDefaultSampleType;
  u16 idStart;
  MPIManagerInterface* mpi;
  GUID subType;
  MUDIDTransInfo* transInfo;
  MUDUndoList undoList;
  MUDBaseNodeInterface* macroSetupNode;
  MUDBaseNodeInterface* sampleSetupNode;
  bool shutdownActive;
  MUDNodeCallbackList nodeCallbackList;
};

#endif // _MUDROOT_H_