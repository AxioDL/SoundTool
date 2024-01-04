#ifndef _MUDACCESSBASEINTERFACE_H_
#define _MUDACCESSBASEINTERFACE_H_


#include <f5/f5classes.h>

#include <mud/MUDTypes.h>

class MUDBaseNodeInterface;
class MUDUndoInfo;
class DIOFile;
class MUDAccessBaseInterface {
public:
  virtual bool Init(MUDBaseNodeInterface* _baseNode, bool& singularTypeDuplicated)=0;
  virtual bool Copy(const MUDBaseNodeInterface* node)=0;
  virtual bool Load(DIOFile* file, const f5::string* name)=0;
  virtual bool Save(DIOFile* file, const f5::string* name)=0;
  virtual bool DoUndo(MUDUndoInfo* info)=0;
  virtual bool DiscardUndoInfo(MUDUndoInfo* info) const=0;
  virtual bool GatherDeleteInfo(MUDUndoInfo* info)=0;
  virtual bool GetLastError(f5::string& text, u32& code) const=0;
  virtual bool Move(MUDBaseNodeInterface* node, MUDBaseNodeInterface* newParent)=0;
  virtual bool Rename(MUDBaseNodeInterface* node, const f5::string& name)=0;
  virtual bool ParentHasBeenRenamed()=0;
  virtual MUDIDType GetRefType()=0;
/* These belong to APAccessInterface
  virtual const MUDTypeStorage*& GetApplicationGUID() const=0;
  virtual void SetApplicationGUID(const MUDTypeStorage * _guid)=0;
  virtual const void* GetData() const=0;
  virtual u64 GetDataSize() const=0;  
  */
};

#endif _MUDACCESSBASEINTERFACE_H_