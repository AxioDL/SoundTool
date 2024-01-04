#ifndef _DISKIO_FILE_H_
#define _DISKIO_FILE_H_

#include <f5/types.h>
#include <f5/f5classes.h>
#include <set>

struct DIOPrivateFindData;

class DIOLabel {
public:
  DIOLabel(const f5::string& _name, u64 _offset) : name(_name), offset(_offset) {}

  const f5::string& GetName() const { return name; }
  const u64 GetOffset() const { return offset; }
private:
  f5::string name;
  u64 offset;
};

class DIOLabelCmp {
public:
  bool operator()(const DIOLabel& s1,const DIOLabel& s2) const {
    return s1.GetName().str()< s2.GetName().str();
  }
};

class DIOLabelList : public std::set<DIOLabel, DIOLabelCmp> {
public:
  void add(const f5::string& name, u64 offset) {
    insert(DIOLabel(name, offset));
  }

  const u64 get(const f5::string& name) const {
    const_iterator it = find(DIOLabel(name, 0));

    if (it == end()) {
      return -1;
    }

    return (*it).GetOffset();
  }
};

typedef void (*DIOFileExecuteCallback)(char);

class DIOFile {
public:
  enum DIOEndianType {
    DIO_ENDIAN_BIG,
    DIO_ENDIAN_LITTLE,
  };

  enum DIOSeekMode {
    DIO_SEEK_BEGIN,
    DIO_SEEK_CURRENT,
    DIO_SEEK_END
  };

  enum DIOOpenMode {
    DIO_OPENMODE_READ,
    DIO_OPENMODE_WRITE,
    DIO_OPENMODE_APPEND,
    DIO_OPENMODE_APPEND_AT_END
  };

  enum DIOLabelMode {
    DIO_LABEL_WRITE32,
    DIO_LABEL_WRITE64
  };

  enum DIOError {
    DIO_ERROR_OK,
    DIO_ERROR_1,
    DIO_ERROR_2,
    DIO_ERROR_3,
    DIO_ERROR_4
  };

  enum DIOFileAttributes {
    DIO_ATTR_ARCHIVE   = 1 << 0,
    DIO_ATTR_HIDDEN    = 1 << 1,
    DIO_ATTR_READONLY  = 1 << 2,
    DIO_ATTR_SYSTEM    = 1 << 3,
    DIO_ATTR_TEMPORARY = 1 << 4,
  };

  class DIOFileTime {
  private:
    friend void DIOCopySystemTime(DIOFileTime& dTime, const SYSTEMTIME& sTime);
    u16 year;
    u16 month;
    u16 weekDay;
    u16 yearDay;
    u16 hour;
    u16 minute;
    u16 second;
    u16 millisecond;
  };

  class DIOFileInfo {

  public:

  private:
    friend class DIOFile;
    friend bool DIOCopyFindData(DIOFileInfo& info);
    f5::string baseFilename;
    f5::string filepath;
    f5::string filename;
    f5::string alternateFilename;
    u64 filesize;
    DIOFileTime createTime;
    DIOFileTime accessTime;
    DIOFileTime writeTime;
    bool isDirectory;
    u32 attributes;
    DIOPrivateFindData* privateFindData;
  };

  virtual void SetEndian(DIOEndianType)=0;
  virtual DIOOpenMode GetAccessMode(void)=0;
  virtual bool GetFileTime(DIOFileTime& creation, DIOFileTime& access, DIOFileTime& write)=0;
  virtual bool GetFileSize(u64& size)=0;
  virtual bool GetFileSize(u32& size)=0;
  virtual bool GetPosition(u64& position)=0;
  virtual bool GetPosition(u32& position)=0;
  virtual bool SetPosition(s64 position, DIOSeekMode mode)=0;
  virtual bool SetPosition(u64 position, DIOSeekMode mode)=0;
  virtual bool SetPosition(s32 position, DIOSeekMode mode)=0;
  virtual bool SetPosition(u32 position, DIOSeekMode mode)=0;
  virtual bool Read(void* buffer, u32 elemSize, u32 numElem)=0;
  virtual bool Read(s8& value)=0;
  virtual bool Read(u8& value)=0;
  virtual bool Read(s16& value)=0;
  virtual bool Read(u16& value)=0;
  virtual bool Read(s32& value)=0;
  virtual bool Read(u32& value)=0;
  virtual bool Read(s64& value)=0;
  virtual bool Read(u64& value)=0;
  virtual bool Read(float& value)=0;
  virtual bool Read(double& value)=0;
  virtual bool Read(v32& value)=0;
  virtual bool ReadString(f5::string& str)=0;
  virtual bool Write(const void* buffer, u32 elemSize, u32 numElem)=0;
  virtual bool Write(s8 value)=0;
  virtual bool Write(u8 value)=0;
  virtual bool Write(s16 value)=0;
  virtual bool Write(u16 value)=0;
  virtual bool Write(s32 value)=0;
  virtual bool Write(u32 value)=0;
  virtual bool Write(s64 value)=0;
  virtual bool Write(u64 value)=0;
  virtual bool Write(float value)=0;
  virtual bool Write(double value)=0;
  virtual bool Write(v32 value)=0;
  virtual bool WriteString(const f5::string& string, ...)=0;
  virtual bool WriteStringZ(const f5::string& string, ...)=0;
  virtual bool Insert(DIOFile* source)=0;
  virtual bool Insert(DIOFile* source, u64 length)=0;
  virtual void SetLabelMode(DIOLabelMode mode)=0;
  virtual bool SetLabel(const f5::string& name)=0;
  virtual u64 GetLabel(const f5::string& name)const=0;
  virtual bool WriteLabel(const f5::string& name)=0;

  static bool Rename(const f5::string& oldName, const f5::string& newName);
  static bool Copy(const f5::string& oldName, const f5::string& newName);
  static bool Delete(const f5::string& name);
    
  static bool ChangeDirectory(const f5::string& directory);
  static bool CreateDirectoryA(const f5::string& name);

  static bool FindFirst(const f5::string& mask, DIOFileInfo& info);
  static bool FindNext(DIOFileInfo& info);
  static bool FindClose(DIOFileInfo& info);

  static bool MakeRelativePath(f5::string& path, const f5::string& from, bool fromFolder, const f5::string& to, bool toFolder);
  static bool MakeRelativePath(f5::string& relPath, const f5::string& fromPath, const f5::string& toPath);
  static bool ChopExtension(f5::string& name, f5::string& extension);
  static bool CleanupPath(f5::string& path);
  static bool PathsIdentical(const f5::string& path1, const f5::string& path2);
  static bool ExpandPath(const f5::string& path, f5::string& buffer);
  static void SplitName(const f5::string& full, f5::string& path, f5::string& base);
  static bool CreatePath(const f5::string& path, bool isFilename);
  static f5::string CreateUniqueTempName();


  static DIOFile* OpenFile(const f5::string& name, DIOOpenMode mode);
  static bool CloseFile(DIOFile*& file);

  static DIOError Execute(const f5::string&, u32 &,DIOFileExecuteCallback stdOut, DIOFileExecuteCallback errOut);
};


#endif //_DISKIO_FILE_H_

