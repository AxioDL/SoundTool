#ifndef _MUDPROPERTIES_H_
#define _MUDPROPERTIES_H_

#include <f5/f5classes.h>

#include <mud/MUDDate.h>

class MUDProperties {
public:
  MUDProperties() {}

  const bool IsDirectory() const { return directory; }
  const bool IsReadOnly() const { return readOnly; }
  const bool IsHidden() const { return hidden; }
  const bool IsSystem() const { return system; }
  const bool IsAccessActive() const { return accessActive; }
  const bool IsModifyActive() const { return modifyActive; }

  void SetDirectory(bool flag) { directory = flag; }
  void SetReadOnly(bool flag) { readOnly = flag; }
  void SetHidden(bool flag) { hidden = flag; }
  void SetSystem(bool flag) { system = flag; }
  void SetAccessActive(bool flag) { accessActive = flag; }
  void SetModifyActive(bool flag) { modifyActive = flag; }

  MUDDate& GetCreatedDate() { return createdDate; }
  const MUDDate& GetCreatedDate() const { return createdDate; }
  f5::string& GetCreatedBy() { return createdBy; }
  const f5::string& GetCreatedBy() const { return createdBy; }

  MUDDate& GetModifiedDate() { return modifiedDate; }
  const MUDDate& GetModifiedDate() const { return modifiedDate; }
  f5::string& GetModifiedBy() { return createdBy; }
  const f5::string& GetModifiedBy() const { return createdBy; }

private:
  bool directory;
  bool readOnly;
  bool hidden;
  bool system;
  bool accessActive;
  bool modifyActive;
  MUDDate createdDate;
  f5::string createdBy;
  MUDDate modifiedDate;
  f5::string modifiedBy;
};

#endif // MUDPROPERTIES_H_