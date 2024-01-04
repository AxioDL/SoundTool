#ifndef _MPIPLUGINDATA_H_
#define _MPIPLUGINDATA_H_


#define WIN32_LEAN_AND_MEAN
#include <windows.h>


class MPIBaseInterface;
class MPIPlugInData {
public:
  MPIPlugInData(const char* _name, HMODULE _hInst, HMODULE _hLibModule, MPIBaseInterface* _baseInterface);

  const char* GetName();

  MPIBaseInterface* GetBaseInterface();

  void SetName(const char* _name);

  HMODULE GetModuleHandleA();
private:
  char name[512];
  HMODULE hInst;
  HMODULE hLibModule;
  MPIBaseInterface* baseInterface;
};
#endif // _MPIPLUGINDATA_H_