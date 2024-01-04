#ifndef _MUDBASENODE_H_
#define _MUDBASENODE_H_

#pragma warning(disable: 4786) // identifier was truncated to '255' characters in the debug information
#include <mud/MUDBaseNodeInterface.h>
#include <mud/MUDBaseNodeSet.h>
#include <mud/MUDTypeStorage.h>
#include <mud/MUDProperties.h>
#include <mud/MUDUndoList.h>
#include <mud/MUDUndoEntry.h>
#include <mud/MUDUndoInfo.h>

#include <f5/types.h>
#include <f5/f5classes.h>

class MUDAccessBaseInterface;
class MUDRootInterface;
class MUDRoot;
class MPIBaseInterface;
class MUDBaseNode : public MUDBaseNodeInterface {
public:
  MUDBaseNode(MUDRoot* dBase, MUDBaseNode* _parent, 
              MUDBaseNode* _prevSibling, MUDBaseNode* _nextSibling, const f5::string& _name, 
              u16 _id, const MUDTypeStorage* forcedInstanceID);

  bool RequestModifyData();

  bool BeginModify();

  void EndModify(MUDUndoInfo* info);
  
  bool RequestAccess();

  void EndAccess();
	
  bool AddReference(MUDBaseNodeInterface* nodeInterface);
	
  bool RemoveReference(MUDBaseNodeInterface* nodeInterface);
	
  void SetReferencedNodes(const MUDBaseNodeSet& _referencedNodes) {
    referencedNodes = _referencedNodes;
  }

  MUDBaseNodeInterfaceList GetReferencedNodes() const;
  
  size_t GetNumReferencedNodes() const;
  
  void SetNodesReferencing(const MUDBaseNodeSet& _nodesReferencing) {
    nodesReferencing = _nodesReferencing;
  }

  MUDBaseNodeInterfaceList GetNodesReferencing() const;

  
  size_t GetNumNodesReferencing() const;
  
  bool IsNodeReferenced(u16 id) const;
  
  bool IsNodeReferenced(const MUDBaseNodeInterface* node) const;
  
  void SetTypeInterface(MUDAccessBaseInterface* _typeInterface) { typeInterface = _typeInterface; }
  
  MUDAccessBaseInterface* GetTypeInterface() { return typeInterface; }
  
  MUDAccessBaseInterface* GetTypeInterface() const;
  
  void SetBaseInterface(MPIBaseInterface* _baseInterface) { baseInterface = _baseInterface; }
  
  MPIBaseInterface* GetBaseInterface() { return baseInterface; }
  
  MPIBaseInterface* GetBaseInterface() const;
  
  MUDRootInterface* GetRootInterface() const;
  
  void SetChild(MUDBaseNodeInterface* _child) { child = _child; }
  MUDBaseNodeInterface* GetChild() { return child; }
  MUDBaseNodeInterface* GetChild() const;    

  void SetPrevSibling(MUDBaseNodeInterface* _prevSibling) { prevSibling = _prevSibling; }
  MUDBaseNodeInterface* GetPrevSibling() { return prevSibling; }
  MUDBaseNodeInterface* GetPrevSibling() const { return prevSibling; }

  void SetSibling(MUDBaseNodeInterface* _nextSibling) { nextSibling = _nextSibling; }
  MUDBaseNodeInterface* GetSibling() { return nextSibling; }
  MUDBaseNodeInterface* GetSibling() const;

  
  void SetParent(MUDBaseNodeInterface* _parent) { parent = _parent; }
  MUDBaseNodeInterface* GetParent() { return parent; }
  MUDBaseNodeInterface* GetParent() const;
  
  const f5::string& GetName() { return name; }
  
  const f5::string& GetName() const;
  
  void SetName(const f5::string& _name) { name = _name; }
  
  u16 GetID() const;
  
  MUDTypeStorage& GetInstanceID() { return instanceID; }
  
  const MUDTypeStorage& GetInstanceID() const;
  
  void SetType(const MUDTypeStorage& _type) { type = _type; }
  void SetType(const GUID& _type) { type = _type; }
  const MUDTypeStorage& GetType() { return type; }
  const MUDTypeStorage& GetType() const;
  
  bool IsType(const MUDTypeStorage& _type) const;
  
  MUDProperties& GetProperties() { return properties; }
  const MUDProperties& GetProperties(void) const;

  
  bool IsDirectory() const { return properties.IsDirectory(); }
  
  void SetDirectory(bool flag) { properties.SetDirectory(flag); }

private:
  friend class MUDRoot;
  void GetDate(MUDDate& date);
  void GetCurrentUser(f5::string& user);

  f5::string name;
  MUDTypeStorage type;
  u16 id;
  MUDTypeStorage instanceID;
  MUDProperties properties;
  MUDBaseNodeInterface* parent;
  MUDBaseNodeInterface* prevSibling;
  MUDBaseNodeInterface* nextSibling;
  MUDBaseNodeInterface* child;
  MUDBaseNodeSet referencedNodes;
  MUDBaseNodeSet nodesReferencing;
  MPIBaseInterface* baseInterface;
  MUDAccessBaseInterface* typeInterface;
  MUDRootInterface* rootInterface;

};

static const GUID mudTypeBaseNode;

#endif // _MUDBASENODE_H_