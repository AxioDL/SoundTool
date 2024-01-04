#include <mpi/MPIManager.h>
#include <mpi/MPIPlugInList.h>
#include <mpi/MPIBaseInterface.h>
#include <mpi/MPIVersionInfo.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef MPIBaseInterface* (*MPIRequestBaseInterface)();

MPIBaseInterface * MPIManager::LoadPlugIn(const char* name) {
  printf("Attempting to create plugin %s\n", name);
  const char* baseName;
  HMODULE hInst;
  HMODULE hLibModule;
  MPIRequestBaseInterface RequestBaseInterface;
  MPIBaseInterface* baseInterface;


  hInst = LoadLibraryA(name);

  if (!hInst) {
    return NULL;
  }

  baseName = strrchr(name, '\\');

  if (baseName) {
    ++baseName;
  } else {
    baseName = name;
  }

  hLibModule = GetModuleHandleA(baseName);

  if (hLibModule) {
    RequestBaseInterface = (MPIRequestBaseInterface)GetProcAddress(hLibModule, "RequestBaseInterface");
    if (RequestBaseInterface) {
      printf("Got RequestBaseInterface proc\n");
      baseInterface = RequestBaseInterface();

      if (baseInterface) {
        baseInterface->SetManagerInterface(this);
        pluginList.push_back(MPIPlugInData(name, hInst, hLibModule, baseInterface));
        printf("Success %s\n", baseName);
        return baseInterface;
      }
    }

    FreeLibrary(hLibModule);
  }
  return NULL;
}


bool MPIManager::AddPlugIn(const char* name, MPIBaseInterface* baseInterface) {
  printf("Adding plugin %s\n", name);
  baseInterface->SetManagerInterface(this);
  pluginList.push_back(MPIPlugInData(name, NULL, NULL, baseInterface));
  return true;
}

bool MPIManager::RemovePlugIn(const char* name) {
  MPIPlugInData* mpiData = pluginList.Find(name);

  if (mpiData) {
    if (mpiData->GetBaseInterface()->RequestRemoval()) {
      HMODULE handle = mpiData->GetModuleHandleA();

      if (!handle || FreeLibrary(handle)) {
        pluginList.Remove(name);
        return true;
      }
    }
  }

  return false;
}

bool MPIManager::RemoveAllPlugIns() {
  MPIPlugInList::iterator it;
  MPIPlugInList::iterator nit;

  for (it = pluginList.begin(); it != pluginList.end() ; it = nit) {
  }

  nit = it++;
  return RemovePlugIn(it->GetName());
}

static const char* GuidToString(const GUID* guid, char* buf) {
  sprintf(buf, "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}", guid->Data1, guid->Data2, guid->Data3,
          guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3], guid->Data4[4], guid->Data4[5],
          guid->Data4[6], guid->Data4[7]);
  return buf;
}

void* MPIManager::FindInterface(const GUID* id, const GUID* subType,MPIBaseInterface*& baseInterface) {
  MPIPlugInList::iterator it;
  char guidStr[40];
  GuidToString(id, guidStr);
  printf("Searching for interface %s...\n", guidStr);
  if (subType) {
    GuidToString(subType, guidStr);
    printf("SubType %s...\n", guidStr);
  }

  for (it = pluginList.begin(); it != pluginList.end(); ++it) {

    MPIBaseInterface* bi = it->GetBaseInterface();
    if (bi) {
      void* iFace = bi->CreateInterface(id, subType);
      if (iFace) {
        baseInterface = bi;
        return iFace;
      }
    }
  }

  return NULL;
}

MPIBaseInterface* MPIManager::GetFirstPlugin() {
  if (pluginList.empty()) {
    return NULL;
  }

  curPlugin = pluginList.begin();

  return (curPlugin++)->GetBaseInterface();
}


MPIBaseInterface* MPIManager::GetNextPlugin() {
  if (curPlugin == pluginList.end()) {
    return NULL;
  }

  return (curPlugin++)->GetBaseInterface();
}