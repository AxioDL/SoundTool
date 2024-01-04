#ifndef _MUDSERVICEDATABASE_H_
#define _MUDSERVICEDATABASE_H_

#include <mud/MUDServiceDataBaseInterface.h>

class MUDServiceDataBase : public MUDServiceDataBaseInterface {
public:
  MUDRootInterface* GetDataBase(f5::string&) { return NULL; }
};

#endif // _MUDSERVICEDATABASE_H_

