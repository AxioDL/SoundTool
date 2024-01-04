#ifndef _MUDTYPESTORAGE_H_
#define _MUDTYPESTORAGE_H_

#include <guiddef.h>

class MUDTypeStorage : public GUID {
public:
  MUDTypeStorage() {};
  MUDTypeStorage(const GUID* guid) {
    *this = guid;
  };

  MUDTypeStorage& operator=(const GUID* guid) {
    if (guid) {
      *this = *guid;
    } else {
      Data1 = 0;
      Data2 = 0;
      Data3 = 0;
      for (u32 i = 0; i < 8; ++i) {
        Data4[i] = 0;
      }
    }

    return *this;
  }

  MUDTypeStorage& operator=(const GUID& guid) {
    Data1 = guid.Data1;
    Data2 = guid.Data2;
    Data3 = guid.Data3;
    for (u32 i = 0; i < 8; ++i) {
      Data4[i] = guid.Data4[i];
    }
    return *this;
  }

  bool operator==(const MUDTypeStorage& right) const {
    return memcmp(this, &right, sizeof(GUID)) == 0;
  }

  bool operator!=(const MUDTypeStorage& right) const {
    return memcmp(this, &right, sizeof(GUID)) != 0;
  }

  operator const MUDTypeStorage*()const {
    return this;
  }
};

#endif // _MUDTYPESTORAGE_H_