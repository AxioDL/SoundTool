#ifndef _MUDMPIMANAGER_H_
#define _MUDMPIMANAGER_H_

#include <mpi/MPIManagerInterface.h>

#include "Globals.h"

class MUDMPIManager : public MPIManagerInterface {
public:
  virtual ~MUDMPIManager() {}
  static MUDEXPORT MUDMPIManager*  MUDCALL CreatePluginManager(void);
};

#endif // _MUDMPIMANAGER_H_