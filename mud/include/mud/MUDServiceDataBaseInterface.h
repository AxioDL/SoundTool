#ifndef _MUDSERVICEDATABASEINTERFACE_H_
#define _MUDSERVICEDATABASEINTERFACE_H_

#include <f5/f5classes.h>

class MUDRootInterface;
class MUDServiceDataBaseInterface {
public:
  virtual MUDRootInterface* GetDataBase(f5::string&)=0;
};


#endif // _MUDSERVICEDATABASEINTERFACE_H_