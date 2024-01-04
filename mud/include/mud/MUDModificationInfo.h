#ifndef _MUDMODIFICATIONINFO_H_
#define _MUDMODIFICATIONINFO_H_

#include <f5/types.h>

class MUDModificationInfo {
public:
  MUDModificationInfo(u32 unk, u16 _id) {
    field_0 = unk;
    id = _id;
  }

private:
  u32 field_0;
  u16 id;
};

#endif // _MUDMODIFICATIONINFO_H_