#ifndef _MUDUNDOGROUP_H_
#define _MUDUNDOGROUP_H_


#include <f5/f5classes.h>
#include <list>

#include <mud/MUDTypeStorage.h>
#include <mud/MUDUndoEntry.h>

class MUDUndoGroup : public std::list<MUDUndoEntry*> {
public:
  ~MUDUndoGroup() {
    iterator it;
    for (it = begin(); it != end(); ++it) {
      if (*it) {
        delete *it;
      }
    }
  }

  void SetDescription(const f5::string& _description) { description = _description; }
  const f5::string& GetDescription() const { return description; }
private:
  f5::string description;
};

#endif // _MUDUNDOGROUP_H_