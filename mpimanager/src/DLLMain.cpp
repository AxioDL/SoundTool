// mpi_manager.cpp : Defines the entry point for the DLL application.
//

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#include <mpi/MPIManager.h>

static MPIManagerInterface* mpiManager = NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
  if (ul_reason_for_call) {
    if (ul_reason_for_call == 1) {
      printf("HELLO!\n");
      mpiManager = NULL;
    }
  } else if (mpiManager) {
    delete mpiManager;
  }

  return TRUE;

}

extern "C" MPIManagerInterface* MPICreatePlugInManager() {
  printf("Creating MPIManager\n");
  mpiManager = new MPIManager();
  return mpiManager;

}
