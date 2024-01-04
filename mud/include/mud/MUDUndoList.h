#ifndef _MUDUNDOLIST_H_
#define _MUDUNDOLIST_H_


#include <list>

#include <f5/types.h>

class MUDUndoGroup;
class MUDUndoList : public std::list<MUDUndoGroup*> {
public:
  ~MUDUndoList() {
    FreeAllEntries();
  }

  void FreeAllEntries();
  void FreeLastEntry();

  void SetEnabled(bool flag) { enabled = flag; }
  bool IsEnabled() const { return enabled; }
  void SetUndoGroup(MUDUndoGroup* _undoGroup) { undoGroup = _undoGroup; }
  MUDUndoGroup* GetUndoGroup() { return undoGroup; }
  const MUDUndoGroup* GetUndoGroup() const { return undoGroup; }
private:
  bool enabled;
  MUDUndoGroup* undoGroup;
};

#endif // _MUDUNDOLIST_H_