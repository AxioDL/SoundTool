#ifndef _MUDBASENODEINTERFACE_H_
#define _MUDBASENODEINTERFACE_H_


#include <f5/types.h>
#include <f5/f5classes.h>

#include <mud/MUDBaseNodeInterfaceList.h>

class MUDUndoInfo;
class MUDAccessBaseInterface;
class MUDTypeStorage;
class MUDRootInterface;
class MUDProperties;

class MPIBaseInterface;

class MUDBaseNodeInterface {
public:
	virtual bool RequestModifyData()=0;
	virtual bool BeginModify()=0;
	virtual void EndModify(MUDUndoInfo* info)=0;
	virtual bool RequestAccess()=0;
	virtual void EndAccess()=0;
	virtual bool AddReference(MUDBaseNodeInterface* nodeInterface)=0;
	virtual bool RemoveReference(MUDBaseNodeInterface* nodeInterface)=0;
	virtual MUDBaseNodeInterfaceList GetReferencedNodes() const=0;
  virtual size_t GetNumReferencedNodes() const=0;
  virtual MUDBaseNodeInterfaceList GetNodesReferencing() const=0;
  virtual size_t GetNumNodesReferencing() const=0;
  virtual bool IsNodeReferenced(u16 id) const=0;
  virtual bool IsNodeReferenced(const MUDBaseNodeInterface* node) const=0;
  virtual MUDAccessBaseInterface* GetTypeInterface() const=0;
  virtual MPIBaseInterface* GetBaseInterface() const=0;
  virtual MUDRootInterface* GetRootInterface() const=0;
  virtual MUDBaseNodeInterface* GetChild() const=0;
  virtual MUDBaseNodeInterface* GetSibling() const=0;
  virtual MUDBaseNodeInterface* GetParent() const=0;
  virtual const f5::string& GetName() const=0;
  virtual u16 GetID() const=0;
  virtual const MUDTypeStorage& GetInstanceID() const=0;
  virtual const MUDTypeStorage& GetType() const=0;
  virtual bool IsType(const MUDTypeStorage& _type) const=0;
  virtual const MUDProperties& GetProperties(void) const=0;
};


#endif _MUDBASENODEINTERFACE_H_