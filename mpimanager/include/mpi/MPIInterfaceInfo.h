#ifndef _MPIINTERFACEINFO_H_
#define _MPIINTERFACEINFO_H_

#include <f5/types.h>

struct MPIInterfaceInfo {
  char data1[256];
  GUID id;
  char data2[256];
  u32 unk;
};

#endif // _MPIINTERFACEINFO_H_