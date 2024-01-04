#ifndef _SCRIPTHOST_H_
#define _SCRIPTHOST_H_

#include <f5/types.h>

#ifdef SCRIPT_HOST_EXPORTS
# define SCHCALL __stdcall
# define SCHEXPORT __declspec(dllexport)
#else
# define SCHEXPORT __declspec(dllimport)
# define SCHCALL __stdcall
#endif


enum SCRHOSTparamType {
  SRCHOST_PT_0
};

enum SCRHOSTengineType {

};

struct SCRHOSTparam {
  SCHEXPORT SCRHOSTparam();
  SCHEXPORT SCRHOSTparam(s32, ...);
  SCHEXPORT ~SCRHOSTparam();

  SCHEXPORT SCRHOSTparam& operator=(const SCRHOSTparam& right);

  s32* params;
};

bool SCHEXPORT SCHCALL scrhostAddFunction(const char*,void *, SCRHOSTparamType, const SCRHOSTparam&);
bool SCHEXPORT SCHCALL scrhostAddFunction(const char*,void *, SCRHOSTparamType, s32, const SCRHOSTparamType* );
bool SCHEXPORT SCHCALL scrhostAttachScript(u32, const char*);
u32  SCHEXPORT SCHCALL scrhostCreateEngine(SCRHOSTengineType);
bool SCHEXPORT SCHCALL scrhostDeleteEngine(u32);
bool SCHEXPORT SCHCALL scrhostExecute(u32);
bool SCHEXPORT SCHCALL scrhostInitialize();
void SCHEXPORT SCHCALL scrhostShutdown();

#endif // _SCRIPTHOST_H_