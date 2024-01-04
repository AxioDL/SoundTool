#ifndef _MUDUNDOINFO_H_
#define _MUDUNDOINFO_H_

#include <f5/f5classes.h>
#include <mud/MUDTypes.h>

class MUDUndoSpecial;
class MUDUndoInfo {
public:
  MUDUndoInfo() {
    specialType = MUD_UNDO_SPECIAL_NONE;
    special = NULL;
  }

  MUDUndoInfo(const f5::string& _description, f5::string* _stringSpecial) {
    description = _description;
    stringSpecial = _stringSpecial;
    specialType = MUD_UNDO_SPECIAL_STRING;
  }

  MUDUndoInfo(const f5::string& _description, MUDUndoSpecial* _undoSpecial) {
    description = _description;
    undoSpecial = _undoSpecial;
    specialType = MUD_UNDO_SPECIAL_SPECIAL;
  }

  MUDUndoInfo(void* _special, MUDUndoSpecialType _specialType) {
    specialType = _specialType;
    special = _special;
  }

  MUDUndoSpecialType GetSpecialType() { return specialType; }
  void* GetSpecial() const { return special; } 
  f5::string* GetStringSpecial() const { return stringSpecial; }
  MUDUndoSpecial* GetUndoSpecial() const { return undoSpecial; }
private:
  MUDUndoSpecialType specialType;
  union {
    f5::string* stringSpecial;
    MUDUndoSpecial* undoSpecial;
    void* special;
  };
  f5::string description;
};

#endif // _MUDUNDOINFO_H_