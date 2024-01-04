#include <mpi/MPIPlugInData.h>

MPIPlugInData::MPIPlugInData(const char* _name, HINSTANCE _hInst, HINSTANCE _hLibModule, MPIBaseInterface* _baseInterface) {
  SetName(_name);
  hInst = _hInst;
  hLibModule = _hLibModule;
  baseInterface = _baseInterface;
}

const char* MPIPlugInData::GetName() { 
  return name; 
}

MPIBaseInterface* MPIPlugInData::GetBaseInterface() {
  return baseInterface;
}

void MPIPlugInData::SetName(const char* _name) {
  strncpy(name, _name, sizeof(name) - 1);
  name[sizeof(name) - 1] = 0;
}


HMODULE MPIPlugInData::GetModuleHandleA() {
  return hLibModule;
}