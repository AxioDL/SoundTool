#ifndef _MUDDATABASEBUILDER_H_
#define _MUDDATABASEBUILDER_H_

#include <mud/Globals.h>
#include <f5/f5classes.h>

#include <list>

class MPIManagerInterface;
class MUDRootInterface;
class MUDDataBaseBuilder {
public:
  static MUDEXPORT MUDRootInterface* MUDCALL CreateDataBase(MPIManagerInterface * mpi, const f5::string& name, const GUID* subType);
  static MUDEXPORT bool MUDCALL DestroyDataBase(const f5::string& name);
  static MUDEXPORT MUDRootInterface * MUDCALL FindDataBase(const f5::string& name);
  static MUDEXPORT std::list<f5::string> MUDCALL GetAllDataBaseNames(void);
};

#endif // _MUDDATABASEBUILDER_H_