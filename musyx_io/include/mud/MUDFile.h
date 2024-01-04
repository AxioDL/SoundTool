#ifndef _MUDFILE_H_
#define _MUDFILE_H_

#include <diskio.h>

class MUDFile {
public:
  static DIOFile* OpenFile(const f5::string& name, DIOFile::DIOOpenMode mode);
  static bool CloseFile(DIOFile*& file);
  static bool StartChunk(DIOFile* file, const char* s, u64& pos);
  static bool EndChunk(DIOFile* file, u64);
};

#endif // _MUDFILE_H_