#ifndef _MPIMANAGER_H_
#define _MPIMANAGER_H_


#ifdef MPI_MANAGER_EXPORTS
# define MPICALL __cdecl
# define MPIEXPORT __declspec(dllexport)
#else
# define MPIEXPORT __declspec(dllimport)
# define MPICALL __cdecl
#endif

#include <mpi/MPIManagerInterface.h>
#include <mpi/MPIPlugInList.h>
#include <stdio.h>

class MPIManager : public MPIManagerInterface {
public:
  MPIManager() {
    printf("MPIManager..........\n");
  };
  MPIBaseInterface* LoadPlugIn(char const * name);
  bool AddPlugIn(const char* name,MPIBaseInterface* baseInterface);
  bool RemovePlugIn(const char* name);
  bool RemoveAllPlugIns();
  void* FindInterface(const GUID* id, const GUID* subType,MPIBaseInterface*& baseInterface);
  MPIBaseInterface * GetFirstPlugin();
  MPIBaseInterface * GetNextPlugin();

private:
  MPIPlugInList pluginList;
  MPIPlugInList::iterator curPlugin;
};

extern "C" MPIEXPORT MPIManagerInterface* MPICreatePlugInManager();


#endif // _MPIMANAGER_H_