#ifndef _MPIVERSIONINFO_H_
#define _MPIVERSIONINFO_H_

#include <f5/types.h>

struct MPIVersionInfo {
  char name[256];
  char copyright[256];
  char description[256];
  char author[256];
  u32 version;
};
#endif // _MPIVERSIONINFO_H_