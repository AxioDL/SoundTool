#include <mud/MUDMPIManager.h>
#include <mud/MUDBaseInterface.h>
#include <mpi/MPIManager.h>
#include <stdio.h>

class _MUDMPIManager : public MUDMPIManager {
public:
  _MUDMPIManager() {
    service = NULL;
    manager = NULL;
  }
  virtual ~_MUDMPIManager() {}
  void SetManager(MPIManagerInterface* _manager) { manager = manager; }
  MPIManagerInterface* GetManager() const { return manager; }
  MPIMUDServiceBaseInterface* GetService() const { return service; }
  void SetService(MPIMUDServiceBaseInterface* _service) { service = _service; }


  MPIBaseInterface* LoadPlugIn(char const * name);
  bool AddPlugIn(const char* name,MPIBaseInterface* baseInterface);
  bool RemovePlugIn(const char* name);
  bool RemoveAllPlugIns();
  void* FindInterface(const GUID* id, const GUID* subType,MPIBaseInterface*& baseInterface);
  MPIBaseInterface * GetFirstPlugin();
  MPIBaseInterface * GetNextPlugin();

private:
  MPIMUDServiceBaseInterface* service;
  MPIManagerInterface*  manager;
};

MUDMPIManager*  MUDMPIManager::CreatePluginManager(void) {
  printf("MUDMPIManager::CreatePluginManager\n");
  _MUDMPIManager* manager = new _MUDMPIManager();

  if (manager) {
    manager->SetManager(MPICreatePlugInManager());
    if (manager->GetManager() == NULL) {
      printf("Failed to build manager...\n");
      delete manager;
      manager = NULL;
      return NULL;
    }

    manager->SetService(new MPIMUDServiceBaseInterface());
    manager->AddPlugIn("MUDServices", manager->GetService());
  }
  return manager;
}

MPIBaseInterface* _MUDMPIManager::LoadPlugIn(char const * name) {
  return manager->LoadPlugIn(name);
}

bool _MUDMPIManager::AddPlugIn(const char* name,MPIBaseInterface* baseInterface) {
  return manager->AddPlugIn(name, baseInterface);
}

bool _MUDMPIManager::RemovePlugIn(const char* name) {
  return manager->RemovePlugIn(name);
}

bool _MUDMPIManager::RemoveAllPlugIns() {
  return manager->RemoveAllPlugIns();
}

void* _MUDMPIManager::FindInterface(const GUID* id, const GUID* subType,MPIBaseInterface*& baseInterface) {
  return manager->FindInterface(id, subType, baseInterface);
}

MPIBaseInterface * _MUDMPIManager::GetFirstPlugin() {
  return manager->GetFirstPlugin();
}

MPIBaseInterface * _MUDMPIManager::GetNextPlugin() {
  return manager->GetNextPlugin();
}