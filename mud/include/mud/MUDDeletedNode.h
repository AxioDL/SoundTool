#ifndef _MUDDELETEDNODE_H_
#define _MUDDELETEDNODE_H_

#include <mud/MUDTypeStorage.h>
#include <mud/MUDProperties.h>

#include <f5/f5classes.h>

#include <list>

class MUDDeletedNode {
public:

  void SetName(const f5::string& _name) { name = _name; }
  const f5::string& GetName() const { return name; }

  void SetType(const MUDTypeStorage& _type) { type = _type; }
  MUDTypeStorage& GetType() { return type; }
  const MUDTypeStorage& GetType() const { return type; }

  void SetID(u16 _id) { id = id; }
  const u16 GetID() const { return id; }

  void SetProperties(const MUDProperties& _properties) { properties = _properties; }
  MUDProperties& GetProperties() { return properties; }
  const MUDProperties& GetProperties() const { return properties; }

  void SetParentInstanceID(const MUDTypeStorage& _parentInstanceID) { parentInstanceID = _parentInstanceID; }
  MUDTypeStorage& GetParentInstanceID() { return parentInstanceID; }
  const MUDTypeStorage& GetParentInstanceID() const { return parentInstanceID; }

  void SetReferencedNodes(const std::list<MUDTypeStorage>& _referencedNodes) { referencedNodes = _referencedNodes; }
  std::list<MUDTypeStorage>& GetReferencedNodes() { return referencedNodes; }
  const std::list<MUDTypeStorage>& GetReferencedNodes() const { return referencedNodes; }
  size_t GetNumReferencedNodes() const { return referencedNodes.size(); }

  void SetNodesReferencing(const std::list<MUDTypeStorage>& _nodesReferencing) { nodesReferencing = _nodesReferencing; }
  std::list<MUDTypeStorage>& GetNodesReferencing() { return nodesReferencing; }
  const std::list<MUDTypeStorage>& GetNodesReferencing() const { return nodesReferencing; }
  size_t GetNumNodesReferencing() const { return nodesReferencing.size(); }

  void SetHasParent(bool flag) { noParent = !flag; }
  bool HasParent() const { return noParent != 0; }
  
private:
  f5::string name;
  MUDTypeStorage type;
  u16 id;
  MUDProperties properties;
  MUDTypeStorage parentInstanceID;
  std::list<MUDTypeStorage> referencedNodes;
  std::list<MUDTypeStorage> nodesReferencing;
  bool noParent;
};


#endif // _MUDDELETEDNODE_H_