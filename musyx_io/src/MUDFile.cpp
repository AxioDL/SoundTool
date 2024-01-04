#include <mud/MUDFile.h>


DIOFile* MUDFile::OpenFile(const f5::string& name, DIOFile::DIOOpenMode mode) {
  DIOFile* file = DIOFile::OpenFile(name, mode);
  u32 header;
  
  if (file) {
    switch(mode) {
    case DIOFile::DIO_OPENMODE_WRITE:
      if (file->Write((u32)SBIG('MUSX')) && file->Write((u64)0)) {
        return file;
      }
      break;
    case DIOFile::DIO_OPENMODE_APPEND_AT_END:
      if (file->SetPosition((u32)0, DIOFile::DIO_SEEK_BEGIN) &&
        file->Read(header) && header == SBIG('MUSX') && file->SetPosition((u32)0, DIOFile::DIO_SEEK_END)) {
        return file;
      }
      break;
    default:
      if (file->Read(header) && header == SBIG('MUSX') && file->SetPosition((u32)12, DIOFile::DIO_SEEK_BEGIN)) {
        return file;
      }
      break;
    }


    DIOFile::CloseFile(file);
    if (mode == DIOFile::DIO_OPENMODE_WRITE) {
      DIOFile::Delete(name);
    }
  }

  return NULL;
}

bool MUDFile::CloseFile(DIOFile*& file) {

  assert(file != NULL);

  if (file->GetAccessMode() != DIOFile::DIO_OPENMODE_READ) {
    u64 size;
    if (!file->GetFileSize(size)) {
      return false;
    }

    if (!file->SetPosition(4ul, DIOFile::DIO_SEEK_BEGIN)) {
      return false;
    }

    file->Write(size);
  }

  return DIOFile::CloseFile(file);
}

bool MUDFile::StartChunk(DIOFile* file, const char* s, u64& pos) {
  assert(file != NULL);

  if (file->Write(*(u32*)s) && file->GetPosition(pos)) {
    return file->Write((u64)0);
  }

  return false;
}

bool MUDFile::EndChunk(DIOFile* file, u64 pos) {
  assert(file != NULL);
  u64 curPos;
  if (file->GetPosition(curPos) && file->SetPosition(pos, DIOFile::DIO_SEEK_BEGIN) && file->Write(curPos - (pos + 8))) {
    return file->SetPosition(curPos, DIOFile::DIO_SEEK_BEGIN);
  }


  return false;
}