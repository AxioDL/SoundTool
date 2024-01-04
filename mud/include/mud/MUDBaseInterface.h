#ifndef _MUD_BASEINTERFACE_H_
#define _MUD_BASEINTERFACE_H_

#include <mpi/MPIBaseInterface.h>
#include <mpi/MPIInterfaceInfo.h>

#include <list>

class MPIManagerInterface;
class MPIMUDServiceBaseInterface : public MPIBaseInterface {
public:
  MPIMUDServiceBaseInterface();
  void SetManagerInterface(MPIManagerInterface* _managerInterface);
  const MPIVersionInfo* GetVersionInfo() const;
  const MPIInterfaceInfo * GetFirstInterface() const;
  const MPIInterfaceInfo * GetNextInterface() const;
  void* CreateInterface(const GUID* guid,const GUID* subType);
  void* CreateInterface(const MPIInterfaceDescriptor* interfaceDesc);
  void ReleaseInterface(const GUID* guid, void* interfacePtr);
  void ReleaseInterface(const MPIInterfaceDescriptor* interfaceDesc,void* interfacePtr);
  bool RequestRemoval();
private:
  MPIManagerInterface* managerInterface;
  std::list<MPIInterfaceInfo> interfaceInfos;
  mutable std::list<MPIInterfaceInfo>::const_iterator currentInterfaceInfo;
  u32 databaseInterfaceCount;
  u32 scriptInterfaceCount;
  u32 inOutInterfaceCount;
};

#endif // _MUD_BASEINTERFACE_H_