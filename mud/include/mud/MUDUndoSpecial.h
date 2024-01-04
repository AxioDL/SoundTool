#ifndef _MUDUNDOSPECIAL_H_
#define _MUDUNDOSPECIAL_H_


#include <mud/MUDTypes.h>

#include <mud/MUDTypeStorage.h>

#include <f5/f5classes.h>


class MUDDeletedNode;
class MUDUndoSpecial {
public:
  MUDDeletedNode* node;
  MUDTypeStorage type;
  MUDUndoSpecialType specialType;
  union {
    f5::string* stringSpecial;
    MUDUndoSpecial* undoSpecial;
    void* special;
  };
};
#endif // _MUDUNDOSPECIAL_H_