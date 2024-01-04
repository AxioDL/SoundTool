#include <mud/MUDUndoList.h>
#include <mud/MUDUndoGroup.h>

void MUDUndoList::FreeAllEntries() {
  iterator it;

  for (it = begin(); it != end(); ++it) {
    if (*it) {
      delete *it;
    }
  }

  clear();
}

void MUDUndoList::FreeLastEntry() {
  if (!empty()) {
    if ((back())) {
      delete back();
      pop_back();
    }
  }
}