#ifndef _MPIINTERFACEDESCRIPTOR_H_
#define _MPIINTERFACEDESCRIPTOR_H_


#include <f5/types.h>

struct MPIInterfaceDescriptor {
  u8 unk1[256];
  GUID id;
  bool hasSubType;
  GUID subType;
};


#endif