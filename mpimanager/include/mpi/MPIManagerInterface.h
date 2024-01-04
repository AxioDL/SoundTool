#ifndef _MPIMANAGERINTERFACE_H_
#define _MPIMANAGERINTERFACE_H_

#include <f5/f5classes.h>

class MPIBaseInterface;
class MPIManagerInterface {
public:
  virtual MPIBaseInterface* LoadPlugIn(char const * name)=0;
  virtual bool AddPlugIn(const char* name,MPIBaseInterface* baseInterface)=0;
  virtual bool RemovePlugIn(const char* name)=0;
  virtual bool RemoveAllPlugIns()=0;
  virtual void* FindInterface(const GUID* id, const GUID* subType,MPIBaseInterface*& baseInterface)=0;
  virtual MPIBaseInterface * GetFirstPlugin()=0;
  virtual MPIBaseInterface * GetNextPlugin()=0;
};

#endif // _MPIMANAGERINTERFACE_H_