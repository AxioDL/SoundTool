#include <diskio.h>
#include <list>

#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>

static void DIOCopySystemTime(DIOFile::DIOFileTime& dTime, const SYSTEMTIME& sTime);

struct DIOPrivateFindData {
  HANDLE findHandle;
  WIN32_FIND_DATAA findData;
  f5::string filepath;
  f5::string field_15C;
  f5::string mask;
};

class _DIOFile : public DIOFile {
public:

  void SetEndian(DIOEndianType);
  DIOOpenMode GetAccessMode(void);
  bool GetFileTime(DIOFileTime& creation, DIOFileTime& access, DIOFileTime& write);
  bool GetFileSize(u64& size);
  bool GetFileSize(u32& size);
  bool GetPosition(u64& position);
  bool GetPosition(u32& position);
  bool SetPosition(s64 position, DIOSeekMode mode);
  bool SetPosition(u64 position, DIOSeekMode mode);
  bool SetPosition(s32 position, DIOSeekMode mode);
  bool SetPosition(u32 position, DIOSeekMode mode);
  bool Read(void* buffer, u32 elemSize, u32 numElem);
  bool Read(s8& value);
  bool Read(u8& value);
  bool Read(s16& value);
  bool Read(u16& value);
  bool Read(s32& value);
  bool Read(u32& value);
  bool Read(s64& value);
  bool Read(u64& value);
  bool Read(float& value);
  bool Read(double& value);
  bool Read(v32& value);
  bool ReadString(f5::string& str);
  bool Write(const void* buffer, u32 elemSize, u32 numElem);
  bool Write(s8 value);
  bool Write(u8 value);
  bool Write(s16 value);
  bool Write(u16 value);
  bool Write(s32 value);
  bool Write(u32 value);
  bool Write(s64 value);
  bool Write(u64 value);
  bool Write(float value);
  bool Write(double value);
  bool Write(v32 value);
  bool WriteString(const f5::string& string, ...);
  bool WriteStringZ(const f5::string& string, ...);
  bool Insert(DIOFile* source);
  bool Insert(DIOFile* source, u64 length);
  void SetLabelMode(DIOLabelMode mode);
  bool SetLabel(const f5::string& name);
  u64 GetLabel(const f5::string& name)const;
  bool WriteLabel(const f5::string& name);

  static DIOError ExecuteWin95(const f5::string& cmd, u32& result,DIOFileExecuteCallback stdOut,DIOFileExecuteCallback errOut);
  static DIOError ExecuteWinNT(const f5::string& cmd, u32& result,DIOFileExecuteCallback stdOut,DIOFileExecuteCallback errOut);

  void SetAccessMode(DIOOpenMode _mode) { mode = mode; }
  void SetHandle(HANDLE _handle) { handle = _handle; }
  const HANDLE GetHandle() const { return handle; }

  const DIOLabelList& GetExplicitLabels() const { return explicitLabels; }
  const DIOLabelList& GetImplicitLabels() const { return implicitLabels; }

private:
  HANDLE handle;
  DIOEndianType endian;
  DIOOpenMode mode;
  DIOLabelMode labelMode;
  DIOLabelList explicitLabels;
  DIOLabelList implicitLabels;
};


void _DIOFile::SetEndian(DIOEndianType endian) {
  endian = endian;
}

DIOFile::DIOOpenMode _DIOFile::GetAccessMode(void) {  return mode; }

bool _DIOFile::GetFileTime(DIOFileTime& creation, DIOFileTime& access, DIOFileTime& write) {
  FILETIME ct;
  FILETIME at;
  FILETIME wt;
  SYSTEMTIME sTime;

  if (::GetFileTime(handle, &ct, &at, &wt)) {
    FileTimeToSystemTime(&ct, &sTime);
    DIOCopySystemTime(creation, sTime);
    FileTimeToSystemTime(&at, &sTime);
    DIOCopySystemTime(access, sTime);
    FileTimeToSystemTime(&wt, &sTime);
    DIOCopySystemTime(write, sTime);
    return true;
  }

  return false;
}

bool _DIOFile::GetFileSize(u64& size) {
  u32 sizeLo;
  u32 sizeHi;
  sizeLo = ::GetFileSize(handle, &sizeHi);

  if (sizeLo == -1 && GetLastError()) {
    return false;
  }

  size = (sizeHi << 32) | sizeLo;
  return true;
}

bool _DIOFile::GetFileSize(u32& size){
  u32 sizeHi;

  size = ::GetFileSize(handle, &sizeHi);
  if ( size == -1 && GetLastError()) {
    return false;
  }
 
  return sizeHi == 0;
}

bool _DIOFile::GetPosition(u64& position) {
  s32 positionHi;
  s32 positionLo;

  positionHi = 0;
  positionLo = SetFilePointer(handle, 0, &positionHi, FILE_CURRENT);
  if (positionLo == -1 && GetLastError()) {
    return false;
  }

  position = (positionHi << 32) | positionLo;
  return true;
}

bool _DIOFile::GetPosition(u32& position) {
  position = SetFilePointer(handle, 0, NULL, FILE_CURRENT);
  return position != -1;
}

bool _DIOFile::SetPosition(s64 position, DIOSeekMode mode) {
  return SetPosition((u64)position, mode);
}

bool _DIOFile::SetPosition(u64 position, DIOSeekMode mode) {
  s32 posHi = (position >> 32) & 0xFFFFFFFF;
  s32 posLo = (position >> 0) & 0xFFFFFFFF;
  s32 res;
  switch (mode) {
  case DIO_SEEK_BEGIN:
    res = SetFilePointer(handle, posLo, &posHi, FILE_BEGIN);
    break;
  case DIO_SEEK_CURRENT:
    res = SetFilePointer(handle, posLo, &posHi, FILE_CURRENT);
    break;
  case DIO_SEEK_END:
    res = SetFilePointer(handle, posLo, &posHi, FILE_END);
    break;
  }

  return res != -1;
}

bool _DIOFile::SetPosition(s32 position, DIOSeekMode mode) {
  return SetPosition((u32)position, mode);
}

bool _DIOFile::SetPosition(u32 position, DIOSeekMode mode){
  s32 res;
  switch (mode) {
  case DIO_SEEK_BEGIN:
    res = SetFilePointer(handle, position, NULL, FILE_BEGIN);
    break;
  case DIO_SEEK_CURRENT:
    res = SetFilePointer(handle, position, NULL, FILE_CURRENT);
    break;
  case DIO_SEEK_END:
    res = SetFilePointer(handle, position, NULL, FILE_END);
    break;
  }

  return res != -1;
}

bool _DIOFile::Read(void* buffer, u32 elemSize, u32 numElem) {
  u32 n;
  u16* w;
  u32* l;
  u64* d;
  u32 v32;
  u64 v64;
  u16 v16;

  switch(elemSize) {
  case sizeof(u8):
    return ReadFile(handle, buffer, numElem, &n, NULL) && n == numElem;
  case sizeof(u16):
    w = (u16*)buffer;
    while (numElem) {
      if (!Read(v16)) {
        return false;
      }
      *w = v16;
      ++w;
      --numElem;
    }

    return true;
  case sizeof(u32):
    l = (u32*)buffer;
    while(numElem) {
      if (!Read(v32)) {
        return false;
      }
      *l = v32;
      ++l;
      --numElem;
    }
    return true;
  case sizeof(u64):
    d = (u64*)buffer;
    while(numElem) {
      if (!Read(v64)) {
        return false;
      }
      *d = v64;
      ++d;
      --numElem;
    }
    return true;
  default:
    return false;
  }
}

bool _DIOFile::Read(s8& value){
  u32 n;
  return ReadFile(handle, &value, sizeof(s8), &n, NULL) && n == sizeof(s8);
}

bool _DIOFile::Read(u8& value){
  u32 n;
  return ReadFile(handle, &value, sizeof(u8), &n, NULL) && n == sizeof(u8);
}

bool _DIOFile::Read(s16& value) {
  u32 n;
  if (ReadFile(handle, &value, sizeof(s16), &n, NULL) && n == sizeof(s16)) {
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
    if (endian == DIO_ENDIAN_BIG) {
#else 
    if (endian == DIO_ENDIAN_LITTLE) {
#endif
      value = (value << 8) | (value >> 8);
    }

    return true;
  }

  return false;
}

bool _DIOFile::Read(u16& value) {
  u32 n;
  if (ReadFile(handle, &value, sizeof(u16), &n, NULL) && n == sizeof(u16)) {
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
    if (endian == DIO_ENDIAN_BIG) {
#else 
    if (endian == DIO_ENDIAN_LITTLE) {
#endif
      value = ((value & 0xff00) << 8) | ((value & 0x00ff) >> 8);
    }

    return true;
  }

  return false;
}

bool _DIOFile::Read(s32& value) {
  u32 n;
  if (ReadFile(handle, &value, sizeof(s32), &n, NULL) && n == sizeof(s32)) {
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
    if (endian == DIO_ENDIAN_BIG) {
#else 
    if (endian == DIO_ENDIAN_LITTLE) {
#endif
      value = ((value & 0xff000000) << 24) | ((value & 0x000000ff) >> 24) |
              ((value & 0x00ff0000) <<  8) | ((value & 0x0000ff00) >>  8);
    }

    return true;
  }

  return false;
}

bool _DIOFile::Read(u32& value) {
  u32 n;
  if (ReadFile(handle, &value, sizeof(u32), &n, NULL) && n == sizeof(u32)) {
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
    if (endian == DIO_ENDIAN_BIG) {
#else 
    if (endian == DIO_ENDIAN_LITTLE) {
#endif
      value = ((value & 0xff000000) << 24) | ((value & 0x000000ff) >> 24) |
              ((value & 0x00ff0000) <<  8) | ((value & 0x0000ff00) >>  8);
    }

    return true;
  }

  return false;
}

bool _DIOFile::Read(s64& value) {
  u32 n;
  if (ReadFile(handle, &value, sizeof(s64), &n, NULL) && n == sizeof(s64)) {
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
    if (endian == DIO_ENDIAN_BIG) {
#else 
    if (endian == DIO_ENDIAN_LITTLE) {
#endif
      value = ((value & 0x00000000ffffffff) << 32) | ((value & 0xffffffff00000000) >> 32) | 
              ((value & 0x0000ffff0000ffff) << 16) | ((value & 0xffff0000ffff0000) >> 16) |
              ((value & 0x00ff00ff00ff00ff) <<  8) | ((value & 0xff00ff00ff00ff00) >>  8);
    }

    return true;
  }

  return false;
}

bool _DIOFile::Read(u64& value) {
  u32 n;
  if (ReadFile(handle, &value, sizeof(u64), &n, NULL) && n == sizeof(u64)) {
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
    if (endian == DIO_ENDIAN_BIG) {
#else 
    if (endian == DIO_ENDIAN_LITTLE) {
#endif
      value = ((value & 0x00000000ffffffff) << 32) | ((value & 0xffffffff00000000) >> 32) | 
              ((value & 0x0000ffff0000ffff) << 16) | ((value & 0xffff0000ffff0000) >> 16) |
              ((value & 0x00ff00ff00ff00ff) <<  8) | ((value & 0xff00ff00ff00ff00) >>  8);
    }

    return true;
  }

  return false;
}

bool _DIOFile::Read(float& value) {
  u32 v;
  if (Read(v)) {
    value = *(float*)(&v);
    return true;
  }

  return false;
}

bool _DIOFile::Read(double& value) {
  u64 v;
  if (Read(v)) {
    value = *(double*)(&v);
    return true;
  }

  return false;
}

bool _DIOFile::Read(v32& value) {
  return Read(value.x) && Read(value.y) && Read(value.z);
}

bool _DIOFile::ReadString(f5::string& str) {
  char c;
  str.erase();

  while(true) {
    if (!Read(&c, 1, 1)) {
      return false;
    }

    if (!c) {
      break;
    }

    str += c;
  }

  return true;
}

bool _DIOFile::Write(const void* buffer, u32 elemSize, u32 numElem) {
  u32 n;
  u16* w;
  u32* l;
  u64* d;
  u32 v32;
  u64 v64;
  u16 v16;

  switch(elemSize) {
  case sizeof(u8):
    return WriteFile(handle, buffer, numElem, &n, NULL) && n == numElem;
  case sizeof(u16):
    w = (u16*)buffer;
    while (numElem) {
      v16 = *w;
      ++w;
      if (!Write(v16)) {
        return false;
      }
      --numElem;
    }

    return true;
  case sizeof(u32):
    l = (u32*)buffer;
    while(numElem) {
      v32 = *l;
      ++l;
      if (!Write(v32)) {
        return false;
      }

      --numElem;
    }
    return true;
  case sizeof(u64):
    d = (u64*)buffer;
    while(numElem) {
      v64 = *d;
      ++d;
      if (!Write(v64)) {
        return false;
      }

      --numElem;
    }
    return true;
  default:
    return false;
  }
}

bool _DIOFile::Write(s8 value) {
  u32 n;
  return WriteFile(this->handle, &value, sizeof(s8), &n, NULL) && n == sizeof(s8);
}

bool _DIOFile::Write(u8 value) {
  u32 n;
  return WriteFile(this->handle, &value, sizeof(u8), &n, NULL) && n == sizeof(u8);
}

bool _DIOFile::Write(s16 value) {
  u32 n;
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
  if (endian == DIO_ENDIAN_BIG) {
#else 
  if (endian == DIO_ENDIAN_LITTLE) {
#endif
    value = ((value & 0xff00) << 8) | ((value & 0x00ff) >> 8);
  }
  return WriteFile(this->handle, &value, sizeof(s16), &n, NULL) && n == sizeof(s16);
}

bool _DIOFile::Write(u16 value) {
  u32 n;
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
  if (endian == DIO_ENDIAN_BIG) {
#else 
  if (endian == DIO_ENDIAN_LITTLE) {
#endif
    value = ((value & 0xff00) << 8) | ((value & 0x00ff) >> 8);
  }
  return WriteFile(this->handle, &value, sizeof(u16), &n, NULL) && n == sizeof(u16);
}

bool _DIOFile::Write(s32 value) {
  u32 n;
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
  if (endian == DIO_ENDIAN_BIG) {
#else 
  if (endian == DIO_ENDIAN_LITTLE) {
#endif
    value = ((value & 0xff000000) << 24) | ((value & 0x000000ff) >> 24) |
            ((value & 0x00ff0000) <<  8) | ((value & 0x0000ff00) >>  8);
  }
  return WriteFile(this->handle, &value, sizeof(s32), &n, NULL) && n == sizeof(s32);
}

bool _DIOFile::Write(u32 value) {
  u32 n;
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
  if (endian == DIO_ENDIAN_BIG) {
#else 
  if (endian == DIO_ENDIAN_LITTLE) {
#endif
    value = ((value & 0xff000000) << 24) | ((value & 0x000000ff) >> 24) |
            ((value & 0x00ff0000) <<  8) | ((value & 0x0000ff00) >>  8);
  }
  return WriteFile(this->handle, &value, sizeof(u32), &n, NULL) && n == sizeof(u32);
}

bool _DIOFile::Write(s64 value) {
  u32 n;
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
  if (endian == DIO_ENDIAN_BIG) {
#else 
  if (endian == DIO_ENDIAN_LITTLE) {
#endif
    value = ((value & 0x00000000ffffffff) << 32) | ((value & 0xffffffff00000000) >> 32) | 
            ((value & 0x0000ffff0000ffff) << 16) | ((value & 0xffff0000ffff0000) >> 16) |
            ((value & 0x00ff00ff00ff00ff) <<  8) | ((value & 0xff00ff00ff00ff00) >>  8);
  }

  return WriteFile(this->handle, &value, sizeof(s64), &n, NULL) && n == sizeof(s64);
}

bool _DIOFile::Write(u64 value) {
  u32 n;
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
  if (endian == DIO_ENDIAN_BIG) {
#else 
  if (endian == DIO_ENDIAN_LITTLE) {
#endif
    value = ((value & 0x00000000ffffffff) << 32) | ((value & 0xffffffff00000000) >> 32) | 
            ((value & 0x0000ffff0000ffff) << 16) | ((value & 0xffff0000ffff0000) >> 16) |
            ((value & 0x00ff00ff00ff00ff) <<  8) | ((value & 0xff00ff00ff00ff00) >>  8);
  }

  return WriteFile(this->handle, &value, sizeof(u64), &n, NULL) && n == sizeof(u64);
}

bool _DIOFile::Write(float value) {
  return Write(*(u32*)(&value));
}

bool _DIOFile::Write(double value) {
  return Write(*(u64*)(&value));
}

bool _DIOFile::Write(v32 value) {
  return Write(value.x) && Write(value.y) && Write(value.z);
}

bool _DIOFile::WriteString(const f5::string& string, ...) {
  va_list va;
  va_start(va, string);
  char* buffer1;
  f5::string buffer2;
  buffer1 = new char[string.size() + 0x20000];
  assert(buffer1!=NULL);

  vsprintf(buffer1, string.c_str(), va);
  buffer2 = buffer1;
  delete buffer1;

  for (size_t pos = 0; ; pos += 2) {
    size_t posa = buffer2.find_first_of("\n", pos);
    if (posa == f5::string::base_type::npos) {
      break;
    }

    buffer2.str().insert(posa, L"\r");
  }

  return Write(buffer2.c_str(), 1, buffer2.size());
}

bool _DIOFile::WriteStringZ(const f5::string& string, ...) {
  va_list va;
  va_start(va, string);
  char* buffer1;
  f5::string buffer2;
  buffer1 = new char[string.size() + 0x20000];
  assert(buffer1!=NULL);

  vsprintf(buffer1, string.c_str(), va);
  buffer2 = buffer1;
  delete buffer1;

  for (size_t pos = 0; ; pos += 2) {
    size_t posa = buffer2.find_first_of('\n', pos);
    if (posa == f5::string::base_type::npos) {
      break;
    }

    buffer2.str().insert(posa, L"\r");
  }

  return Write(buffer2.c_str(), 1, buffer2.size() + 1);
}

bool _DIOFile::Insert(DIOFile* source) {
  u64 size;
  u64 pos;
  if (source->GetFileSize(size) && source->GetPosition(pos)) {
    if (pos == size) {
      return false;
    }

    return Insert(source, size - pos);
  }

  return false;
}

bool _DIOFile::Insert(DIOFile* source, u64 length) {
  bool ret;
  void* buffer = malloc(0x10000u);
  ret = false;

  if (buffer) {
    while (length > 0x10000u) {
      if (!source->Read(buffer, 1, 0x10000) || !Write(buffer, 1, 0x10000u)) {
        goto quit_insert;
      }

      length -= 0x10000;
    }

    if (source->Read(buffer, 1, length) && !Write(buffer, 1, length)) {
      ret = true;
    }

quit_insert:
    free(buffer);
  }

  return ret;
}

void _DIOFile::SetLabelMode(DIOLabelMode mode) {
  labelMode = mode;
}

bool _DIOFile::SetLabel(const f5::string& name) {
  u32 pos;

  if (GetPosition(pos)) {
    explicitLabels.add(name, pos);
    return true;
  }

  return false;
}

u64 _DIOFile::GetLabel(const f5::string& name) const {

  return explicitLabels.get(name);
}

bool _DIOFile::WriteLabel(const f5::string& name) {
  u64 value = explicitLabels.get(name);
  u64 pos;
  if (value == -1) {
    GetPosition(pos);

    implicitLabels.add(name, pos);

    if (labelMode == DIO_LABEL_WRITE64) {
      return Write(pos);
    }
    return Write((u32)pos);
  } 
  
  if (labelMode == DIO_LABEL_WRITE64) {
    return Write(value);
  }

  return Write((u32)value);
}

struct EXInfo {
  f5::string cmd;
  DIOFile::DIOError error;
  u32 exitCode;
};

static void DIOCopySystemTime(DIOFile::DIOFileTime& dTime, const SYSTEMTIME& sTime) {
  dTime.yearDay = sTime.wDay;
  dTime.weekDay = sTime.wDayOfWeek;
  dTime.hour = sTime.wHour;
  dTime.millisecond = sTime.wMilliseconds;
  dTime.minute = sTime.wMinute;
  dTime.month = sTime.wMonth;
  dTime.second = sTime.wSecond;
  dTime.year = sTime.wYear;
}

static bool DIOCopyFindData(DIOFile::DIOFileInfo& info) {
  f5::string buffer;
  DIOPrivateFindData* pData = info.privateFindData;

  info.baseFilename = pData->findData.cFileName;
  info.alternateFilename = pData->findData.cAlternateFileName;

  SYSTEMTIME sTime;

  FileTimeToSystemTime(&pData->findData.ftLastAccessTime, &sTime);
  DIOCopySystemTime(info.accessTime, sTime);

  
  FileTimeToSystemTime(&pData->findData.ftCreationTime, &sTime);
  DIOCopySystemTime(info.createTime, sTime);

  FileTimeToSystemTime(&pData->findData.ftLastWriteTime, &sTime);
  DIOCopySystemTime(info.writeTime, sTime);

  info.filesize = (pData->findData.nFileSizeHigh << 32) | 
                   pData->findData.nFileSizeLow;
  info.isDirectory = (pData->findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
  info.attributes  = (pData->findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)   != 0 ? DIOFile::DIO_ATTR_ARCHIVE   : 0;
  info.attributes |= (pData->findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)    != 0 ? DIOFile::DIO_ATTR_HIDDEN    : 0;
  info.attributes |= (pData->findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)  != 0 ? DIOFile::DIO_ATTR_READONLY  : 0;
  info.attributes |= (pData->findData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)    != 0 ? DIOFile::DIO_ATTR_SYSTEM    : 0;
  info.attributes |= (pData->findData.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) != 0 ? DIOFile::DIO_ATTR_TEMPORARY : 0;

  buffer = pData->mask;

  size_t pos = buffer.find_last_of('\\');

  if (pos != f5::string::base_type::npos) {
    buffer.resize(pos + 1);
  }

  buffer += info.baseFilename;

  char charPathBuffer[MAX_PATH];
  char* basePtr;
  if (GetFullPathName(buffer.c_str(), MAX_PATH, charPathBuffer, &basePtr)) {
    info.filename = basePtr;
    *basePtr = 0;
    pData->filepath = charPathBuffer;
    info.filepath = pData->filepath;
    return true;
  }
  return false;
}

static u32 __stdcall ExecuteThread(LPVOID lpThreadParameter) {
  EXInfo* exInfo = reinterpret_cast<EXInfo*>(lpThreadParameter);
  
  SECURITY_ATTRIBUTES procattr;
  procattr.nLength = sizeof(SECURITY_ATTRIBUTES);
  procattr.lpSecurityDescriptor = NULL;
  procattr.bInheritHandle = FALSE;
  
  SECURITY_ATTRIBUTES threadattr;
  threadattr.nLength = sizeof(SECURITY_ATTRIBUTES);
  threadattr.lpSecurityDescriptor = NULL;
  threadattr.bInheritHandle = TRUE;
  
  STARTUPINFO sinfo;
  memset(&sinfo, 0, sizeof(STARTUPINFO));
  sinfo.cb = sizeof(STARTUPINFO);
  sinfo.wShowWindow = FALSE;


  char* cmdBuffer = new char[exInfo->cmd.size() + 1];

  strcpy(cmdBuffer, exInfo->cmd.c_str());
  
  PROCESS_INFORMATION pinfo;
  memset(&pinfo, 0, sizeof(PROCESS_INFORMATION));

  if (CreateProcess(0, cmdBuffer, &procattr, &threadattr, TRUE, 0, NULL, NULL, &sinfo, &pinfo)) {
    exInfo->exitCode = -1;
    exInfo->error = DIOFile::DIO_ERROR_OK;

    while(GetExitCodeProcess(pinfo.hProcess, &exInfo->exitCode)) {
      Sleep(100);
      if (exInfo->exitCode != STILL_ACTIVE) {
        goto close;
      }
    }

    exInfo->error = DIOFile::DIO_ERROR_3;
close:
    CloseHandle(pinfo.hProcess);
  } else {
    exInfo->error = DIOFile::DIO_ERROR_2;
  }

  delete cmdBuffer;
  return 0;
}


DIOFile::DIOError _DIOFile::ExecuteWin95(const f5::string& cmd, u32& result, DIOFileExecuteCallback stdOut,DIOFileExecuteCallback errOut) {
  EXInfo exInfo;
  exInfo.cmd = cmd;
  DWORD threadID;

  HANDLE threadHandle = CreateThread(0, 0, ExecuteThread, &exInfo, 0, &threadID);
  if (threadHandle) {
    WaitForSingleObject(threadHandle, INFINITE);
    CloseHandle(threadHandle);
  } else {
    exInfo.error = DIOFile::DIO_ERROR_2;
  }

  if (exInfo.error == DIOFile::DIO_ERROR_OK) {
    result = exInfo.exitCode;
  }

  return exInfo.error;
}


DIOFile::DIOError _DIOFile::ExecuteWinNT(const f5::string& cmd, u32& result,DIOFileExecuteCallback stdOut,DIOFileExecuteCallback errOut) {
  EXInfo exInfo;
  char buffer[12];
  HANDLE stdReadHandle;
  HANDLE stdHandle;
  HANDLE errReadHandle;
  HANDLE errHandle;
  HANDLE oldStdHandle;
  HANDLE oldErrHandle;
  HANDLE handles[3];
  HANDLE hHandle;
  HANDLE hEvent;
  HANDLE threadHandle;
  DWORD threadID;
  SECURITY_ATTRIBUTES secAttr;
  OVERLAPPED oLappedStd;
  OVERLAPPED oLappedErr;
  bool stdPending;
  bool errPending;
  bool threadDead;
  bool waitResult;
  bool stdEOF;
  bool errEOF;
  char c;
  DWORD n;
  DWORD err;
  
  secAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  secAttr.lpSecurityDescriptor = NULL;
  secAttr.bInheritHandle = TRUE;

  sprintf(buffer, "%08x", GetCurrentProcessId());

  f5::string pipeName = f5::string("\\\\.\\pip\\DiskIOExecuteStdPipe") + buffer;

  stdReadHandle = CreateNamedPipe(pipeName.c_str(), (FILE_FLAG_OVERLAPPED | PIPE_ACCESS_INBOUND), PIPE_TYPE_BYTE, 1, 0, 0, 0, &secAttr);
  stdHandle = CreateFile(pipeName.c_str(), GENERIC_WRITE, 0, &secAttr, CREATE_ALWAYS | CREATE_NEW, FILE_FLAG_OVERLAPPED, NULL);

  pipeName = f5::string("\\\\.\\pip\\DiskIOExecuteErrPipe") + buffer;
  errReadHandle = CreateNamedPipe(pipeName.c_str(), (FILE_FLAG_OVERLAPPED | PIPE_ACCESS_INBOUND), PIPE_TYPE_BYTE, 1, 0, 0, 0, &secAttr);
  errHandle = CreateFile(pipeName.c_str(), GENERIC_WRITE, 0, &secAttr, CREATE_ALWAYS | CREATE_NEW, FILE_FLAG_OVERLAPPED, NULL);

  oldStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  oldErrHandle = GetStdHandle(STD_ERROR_HANDLE);

  SetStdHandle(STD_OUTPUT_HANDLE, stdHandle);
  SetStdHandle(STD_ERROR_HANDLE, errHandle);
  
  exInfo.cmd = cmd;

  threadID;
  threadHandle = CreateThread(0, 0, ExecuteThread, &exInfo, 0, &threadID);

  if (!threadHandle) {
    exInfo.error = DIOFile::DIO_ERROR_2;
    SetStdHandle(STD_OUTPUT_HANDLE, oldStdHandle);
    SetStdHandle(STD_ERROR_HANDLE, oldErrHandle);
    CloseHandle(stdHandle);
    CloseHandle(errHandle);

    goto closeIO;
  }
  
  if (!stdOut && !errOut) {
    WaitForSingleObject(threadHandle, INFINITE);
    SetStdHandle(STD_OUTPUT_HANDLE, oldStdHandle);
    SetStdHandle(STD_ERROR_HANDLE, oldErrHandle);
    CloseHandle(stdHandle);
    CloseHandle(errHandle);
    goto terminateThread;
  }

  handles[0] = threadHandle;
  hHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
  handles[1] = hHandle;
  hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  handles[2] = hEvent;

  stdPending = false;
  errPending = false;
  stdEOF = false;
  errEOF = false;
  threadDead = false;
  waitResult = true;
  while (true) {
    if (!threadDead && !waitResult) {
      threadDead = true;

      if (WaitForSingleObject(hHandle, 0)) {
        CancelIo(stdReadHandle);
        stdPending = false;
        ResetEvent(hEvent);
      }
    }

    if (stdOut) {
      if (!WaitForSingleObject(hHandle, 0)) {
        stdOut(c);
        stdPending = false;
      }

      if (!stdPending && !stdEOF) {

        while (ReadFile(stdReadHandle, &c, 1, &n, &oLappedStd)) {
          stdOut(c);
        }

        err = GetLastError();
        if (err != ERROR_HANDLE_EOF) {
          if (err != ERROR_IO_PENDING) {
            exInfo.error = DIOFile::DIO_ERROR_4;
            goto errorExit;
          }

          PeekNamedPipe(stdReadHandle, NULL, 0, 0, &n, 0);
          if (!threadDead || n) {
            stdPending = true;
          } else {
            CancelIo(stdReadHandle);
            ResetEvent(oLappedStd.hEvent);
            stdEOF = true;
          }
        }
      }
    }

    if (errOut) {
      if (!WaitForSingleObject(hEvent, 0)) {
        errOut(c);
        ResetEvent(hEvent);
        errPending = false;
      }

      if (!errPending && !errEOF) {
        while (ReadFile(errReadHandle, &c, 1, &n, &oLappedErr)) {
          stdOut(c);
        }

        err = GetLastError();
        if (err != ERROR_HANDLE_EOF) {
          if (err != ERROR_IO_PENDING) {
            exInfo.error = DIOFile::DIO_ERROR_4;
            goto errorExit;
          }

          PeekNamedPipe(errReadHandle, NULL, 0, 0, &n, 0);
          if (!threadDead || n) {
            stdPending = true;
          } else {
            CancelIo(errReadHandle);
            ResetEvent(oLappedErr.hEvent);
            errEOF = true;
          }
        }
      }
    }

    if (stdEOF && errEOF) {
      goto errorExit;
    }

    if (threadDead) {
      waitResult = WaitForMultipleObjects(2, &handles[1], 0, INFINITE) != 0;
    } else {
      waitResult = WaitForMultipleObjects(3, &handles[0], 0, INFINITE) != 0;
    }
  }


errorExit:
  SetStdHandle(STD_OUTPUT_HANDLE, oldStdHandle);
  SetStdHandle(STD_ERROR_HANDLE, oldErrHandle);
  CloseHandle(stdHandle);
  CloseHandle(errHandle);
  CloseHandle(hHandle);
  CloseHandle(hEvent);

terminateThread:
  TerminateThread(threadHandle, 0);
  CloseHandle(threadHandle);

closeIO:
  CloseHandle(stdReadHandle);
  CloseHandle(errReadHandle);

  if (exInfo.error == DIOFile::DIO_ERROR_OK) {
    result = exInfo.exitCode;
  }

  return exInfo.error;
}


DIOFile::DIOError DIOFile::Execute(const f5::string& cmd, u32& result, DIOFileExecuteCallback stdOut, DIOFileExecuteCallback errOut) {
  OSVERSIONINFO vinf;
  vinf.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
  GetVersionEx(&vinf);

  if (vinf.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
    return _DIOFile::ExecuteWin95(cmd, result, stdOut, errOut);
  }

  return _DIOFile::ExecuteWinNT(cmd, result, stdOut, errOut);
}

bool DIOFile::Rename(const f5::string& oldName, const f5::string& newName) {
  return MoveFileA(oldName.c_str(), newName.c_str()) != 0;
}

bool DIOFile::Copy(const f5::string& oldName, const f5::string& newName) {
  return CopyFileA(oldName.c_str(), newName.c_str(), TRUE) != 0;
}

bool DIOFile::Delete(const f5::string& name) {
  u32 flags = GetFileAttributesA(name.c_str());

  if (flags == 0xFFFFFFFF) {
    return false;
  }

  if (flags & FILE_ATTRIBUTE_DIRECTORY) {
    return RemoveDirectoryA(name.c_str()) != 0;
  }


  return DeleteFileA(name.c_str()) != 0;
}


bool DIOFile::ChangeDirectory(const f5::string& directory) {
  return SetCurrentDirectoryA(directory.c_str()) != 0;
}

bool DIOFile::CreateDirectoryA(const f5::string& name) {
  return ::CreateDirectoryA(name.c_str(), FALSE) != 0;
}

bool DIOFile::FindFirst(const f5::string& mask, DIOFileInfo& info) {
  DIOPrivateFindData* privInfo = new DIOPrivateFindData();


  if (privInfo) {

    info.privateFindData = privInfo;
    privInfo->mask = mask;

    privInfo->findHandle = FindFirstFileA(mask.c_str(), &privInfo->findData);
    if (privInfo->findHandle == (HANDLE)-1) {
      delete privInfo;
      return false;
    }

    bool ret = DIOCopyFindData(info);

    if (!ret) {
      delete privInfo;
      info.privateFindData = NULL;
    }

    return ret;
  }

  return false;
}

bool DIOFile::FindNext(DIOFileInfo& info) {

  if (FindNextFileA(info.privateFindData->findHandle, &info.privateFindData->findData)) {
    DIOCopyFindData(info);
    return true;
  }

  return false;
}
bool DIOFile::FindClose(DIOFileInfo& info) {
  DIOPrivateFindData* pData = info.privateFindData;

  if (::FindClose(pData->findHandle)) {
    delete pData;
    info.privateFindData = NULL;
    return true;
  }

  return false;
}

bool DIOFile::MakeRelativePath(f5::string& path, const f5::string& from, bool fromFolder, const f5::string& to, bool toFolder) {
  char charBuffer[MAX_PATH];

  if (PathRelativePathToA(charBuffer, 
                          from.c_str(), fromFolder ? FILE_ATTRIBUTE_DIRECTORY : 0, 
                          to.c_str(), toFolder ? FILE_ATTRIBUTE_DIRECTORY : 0)) {
    path = charBuffer;
    return true;
  }

  return false;
}

bool DIOFile::MakeRelativePath(f5::string& relPath, const f5::string& fromPath, const f5::string& toPath) {
  char charBuffer[MAX_PATH];

  bool ret = PathRelativePathToA(charBuffer, 
                                 fromPath.c_str(), FILE_ATTRIBUTE_DIRECTORY, 
                                 toPath.c_str(), FILE_ATTRIBUTE_DIRECTORY) == TRUE;

  if (ret) {
    relPath = charBuffer;
  } else {
    relPath.erase();
  }

  return ret;
}

bool DIOFile::ChopExtension(f5::string& name, f5::string& extension) {
  extension.erase();
  size_t pos = name.find_last_of('.');

  if (pos != f5::string::base_type::npos) {
    extension.str().insert(extension.str().end(), name.str().begin() + pos + 1, name.str().end());
    name.resize(pos);
  }
  return false;
}

bool DIOFile::CleanupPath(f5::string& path) {
  size_t i;

  for (i = path.find('/', 0); i != f5::string::base_type::npos;  i = path.find('/', i)) {
    path.str().at(i) = '\\';
  }

  size_t p = path.find(".\\", 0);

  while (true) {
    if (p == f5::string::base_type::npos) {
      break;
    }

    if (p < 1 || path.str()[p - 1] != '.') {
      path.erase(p, 1);
      p = path.find(".\\", p);
    } else {
      p = path.find(".\\", p + 2);
    }
  }


  size_t lp = 0;

  while (true) {
    size_t pa = path.find("\\\\", lp);

    if (pa == f5::string::base_type::npos) {
      break;
    }

    if (pa) {
      path.erase(pa, 1);
      lp = pa;
    } else {
      lp = 2;
    }
  }

  return true;
}

bool DIOFile::PathsIdentical(const f5::string& path1, const f5::string& path2) {
  f5::string p1 = path1;
  f5::string p2 = path2;
  CleanupPath(p1);
  CleanupPath(p2);

  if (p1.size() != p2.size()) {
    return false;
  }


  for (size_t i = 0; i < p1.size() ; ++i) {
    if (tolower(p1.str()[i]) != tolower(p2.str()[i])) {
      return false;
    }
  }
  return true;
}

bool DIOFile::ExpandPath(const f5::string& path, f5::string& buffer) {
  char charBuffer[MAX_PATH];
  char* basePtr;

  u32 len = GetFullPathNameA(path.c_str(), MAX_PATH, charBuffer, &basePtr);

  if (!len || len + 1 > MAX_PATH) {
    return false;
  }


  buffer = charBuffer;
  return true;
}


static void splitnames(const f5::string& full, f5::string& path, f5::string& base) {
  char drive[4];
  char dir[256];
  char fname[256];
  char extension[264];

  _splitpath(full.c_str(), drive, dir, fname, extension);

  path = f5::string(drive) + dir;
  base = f5::string(fname) + extension;
}

void DIOFile::SplitName(const f5::string& full, f5::string& path, f5::string& base) {
  splitnames(full, path, base);
  if (path.size() <= 2) {
    return;
  }


  while (*(path.str().end() - 1) != '\\' && *(path.str().end() - 2) != ':') {
    path.resize(path.size() - 1);
  }
}

bool DIOFile::CreatePath(const f5::string& path, bool isFilename) {
  std::list<f5::string> dirList;
  f5::string remain;
  f5::string base;
  f5::string _path;
  f5::string currentPath;

  _path = path;
  CleanupPath(_path);

  while (true) {
    SplitName(_path, remain, base);

    if (base.str().empty()) {
      break;
    }

    dirList.push_front(base);
    _path = remain;
  }

  if (isFilename) {
    dirList.pop_back();
  }

  currentPath = _path;

  ;

  for (std::list<f5::string>::const_iterator it = dirList.begin(); it != dirList.end(); ++it) {
    DIOFileInfo info;

    f5::string right = f5::string("\\") + *it;
    currentPath += right;

    if (FindFirst(currentPath, info)) {
      if (!info.isDirectory) {
        FindClose(info);
        return false;
      }

      FindClose(info);
    } else if (!CreateDirectoryA(currentPath)) {
      return false;
    }
  }

  return true;
}

f5::string DIOFile::CreateUniqueTempName() {
  char temp[MAX_PATH];
  char temp2[MAX_PATH];
  if (GetTempPathA(MAX_PATH, temp2)) {
    GetTempFileNameA(temp2, "dio", 0, temp);
    return f5::string(temp);
  }

  return f5::string("");
}


DIOFile* DIOFile::OpenFile(const f5::string& name, DIOOpenMode mode) {
  _DIOFile* file = new _DIOFile();
  u32 oMode;
  u32 aMode;

  if (file) {
    switch(mode) {
    case DIO_OPENMODE_READ:
      aMode = GENERIC_READ;
      oMode = OPEN_EXISTING;
      break;
    case DIO_OPENMODE_WRITE:
      aMode = GENERIC_READ | GENERIC_WRITE;
      oMode = CREATE_ALWAYS;
      break;
    case DIO_OPENMODE_APPEND:
    case DIO_OPENMODE_APPEND_AT_END:
      aMode = GENERIC_READ | GENERIC_WRITE;
      oMode = OPEN_EXISTING;
      break;
    default:
      break;
    }

    file->SetAccessMode(mode);
    HANDLE handle = CreateFileA(name.c_str(), aMode, 0, NULL, oMode, FILE_ATTRIBUTE_NORMAL, NULL);
    file->SetHandle(handle);
    if (file->GetHandle() == (HANDLE)-1) {
      delete file;
      file = NULL;
    } else if (mode == DIO_OPENMODE_APPEND_AT_END) {
      file->SetPosition((u32)0, DIO_SEEK_END);
    }
  }

  return file;
}

bool DIOFile::CloseFile(DIOFile*& file) {
  if (static_cast<_DIOFile*>(file)->GetHandle() == (HANDLE)-1) {
    return false;
  }

  if (!static_cast<_DIOFile*>(file)->GetImplicitLabels().empty()) {
    DIOLabelList::const_iterator it;
    u32 oldPos;
    file->GetPosition(oldPos);
    for (it = static_cast<_DIOFile*>(file)->GetImplicitLabels().begin(); 
         it != static_cast<_DIOFile*>(file)->GetImplicitLabels().end(); 
         ++it) {
      if (file->SetPosition((*it).GetOffset(), DIO_SEEK_BEGIN)) {
        file->WriteLabel((*it).GetName());
      }
    }

    file->SetPosition(oldPos, DIO_SEEK_BEGIN);
  }

  if (CloseHandle(static_cast<_DIOFile*>(file)->GetHandle())) {
    delete file;
    file = NULL;
    return true;
  }

  delete file;
  return false;
}