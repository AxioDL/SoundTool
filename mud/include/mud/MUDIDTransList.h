#ifndef _MUDIDTRANSLIST_H_
#define _MUDIDTRANSLIST_H_

#include <f5/types.h>

#include <list>

class MUDIDTransEntry {
public:
  MUDIDTransEntry(u16 _importID, u16 _mudID) : importID(_importID), mudID(_mudID) {}

  void SetImport(u16 _importID) { importID = _importID; }
  u16 GetImport() { return importID; }
  const u16 GetImport() const { return importID; }


  void SetMUD(u16 _mudID) { mudID = _mudID; }
  u16 GetMUD() { return mudID; }
  const u16 GetMUD() const { return mudID; }
private:
  u16 importID;
  u16 mudID;
};

class MUDIDTransList : public std::list<MUDIDTransEntry> {
public:
  void Add(u16 importID, u16 mudID) {
    iterator it;
    for (it = begin(); it != end(); ++it) {
      if (it->GetImport() == importID) {
        it->SetMUD(mudID);
      }
    }
    push_back(MUDIDTransEntry(importID, mudID));
  }

  u16 GetMUD(u16 id) {
    for (const_iterator it = begin(); it != end(); ++it) {
      if (it->GetImport() == id) {
        return it->GetMUD();
      }
    }
    return -1;
  }
};



#endif // _MUDIDTRANSLIST_H_