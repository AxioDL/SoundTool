#ifndef _MPIPLUGINLIST_H_
#define _MPIPLUGINLIST_H_


#include <mpi/MPIPlugInData.h>

#include <list>

class MPIPlugInList : public std::list<MPIPlugInData> {
public:
  void Add(const MPIPlugInData& plugInData) {
    push_back(plugInData);
  }

  MPIPlugInData* Find(const char* name);
  void Remove(const char* name);
};


#endif // _MPIPLUGINLIST_H_