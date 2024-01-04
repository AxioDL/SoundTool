#include <mpi/MPIPlugInList.h>

MPIPlugInData* MPIPlugInList::Find(const char* name) {
  iterator it = begin();
  iterator end_it = end();

  while (it != end_it) {
    if (!strcmp(name, it->GetName())) {
      return &*it;
    }

    ++it;
  }

  return NULL;
}

void MPIPlugInList::Remove(const char* name) {
  iterator it = begin();
  iterator end_it = end();

  while (it != end_it) {
    if (!strcmp(name, it->GetName())) {
      erase(it);
      return;
    }

    ++it;
  }
}