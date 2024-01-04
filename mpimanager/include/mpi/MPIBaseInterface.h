#ifndef _MPIBASEINTERFACE_H_
#define _MPIBASEINTERFACE_H_

#include <f5/f5classes.h>
#include <f5/types.h>

class MPIManagerInterface;

struct MPIInterfaceInfo;
struct MPIInterfaceDescriptor;
struct MPIVersionInfo;

class MPIBaseInterface {
public:
  virtual void SetManagerInterface(MPIManagerInterface* _managerInterface)=0;
  virtual const MPIVersionInfo* GetVersionInfo() const=0;
  virtual const MPIInterfaceInfo * GetFirstInterface() const=0;
  virtual const MPIInterfaceInfo * GetNextInterface() const=0;
  virtual void* CreateInterface(const GUID* guid,const GUID* subType)=0;
  virtual void* CreateInterface(const MPIInterfaceDescriptor* interfaceDesc)=0;
  virtual void ReleaseInterface(const GUID* guid, void* interfacePtr)=0;
  virtual void ReleaseInterface(const MPIInterfaceDescriptor* interfaceDesc,void* interfacePtr)=0;
  virtual bool RequestRemoval()=0;
};

#endif // _MPIBASEINTERFACE_H_