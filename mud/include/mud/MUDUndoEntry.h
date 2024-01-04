#ifndef _MUDUNDOENTRY_H_
#define _MUDUNDOENTRY_H_


#include <f5/f5classes.h>
#include <mud/MUDTypes.h>
#include <mud/MUDTypeStorage.h>

class MUDUndoSpecial;
class MUDRoot;
class MUDUndoEntry {
public:

  MUDUndoEntry(MUDRoot* _root);
  ~MUDUndoEntry();
  void SetDescription(const f5::string& _description) { description = _description; }
  const f5::string& GetDescription() const { return description; }

  void SetType(const MUDTypeStorage& _type) { type = _type; }
  const MUDTypeStorage& GetType() const { return type; }

  void SetInstanceID(const MUDTypeStorage& _instanceID) { instanceID = _instanceID; }
  const MUDTypeStorage& GetInstanceID() const { return instanceID; }

  void SetFlags(u32 _flags) { flags = _flags; }
  u32 GetFlags() const { return flags; }

  void SetSpecial(MUDUndoSpecialType _specialType, void* _special) {
    specialType = _specialType;
    special = _special;
  }

  MUDUndoSpecialType GetSpecialType() const { return specialType; }
  
  f5::string* GetStringSpecial() const {
    return stringSpecial;
  }

  
  MUDUndoSpecial* GetUndoSpecial() const {
    return undoSpecial;
  }

  
  void* GetSpecial() const {
    return special;
  }

private:
  MUDRoot* root;
  f5::string description;
  MUDTypeStorage type;
  MUDTypeStorage instanceID;
  u32 flags;
  MUDUndoSpecialType specialType;
  union {
    f5::string* stringSpecial;
    MUDUndoSpecial* undoSpecial;
    void* special;
  };
};


#endif // _MUDUNDOENTRY_H_