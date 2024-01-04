#include <mud/MUDServiceInOut.h>


MUDServiceInOut* MUDServiceInOut::self = NULL;
u32 MUDServiceInOut::refCount = 0;

MUDServiceInOut::MUDServiceInOut() {
  verbose = false;
  ++refCount;
  stdOut = _StdOut;
  errOut = _ErrOut;
}

void MUDServiceInOut::_StdOut(const f5::string& s) {
  printf(s.c_str());
}

void MUDServiceInOut::_ErrOut(const f5::string& s) {
  fprintf(stderr, s.c_str());
}

void MUDServiceInOut::SetVerbosity(bool on) {
  verbose = on;
}

void MUDServiceInOut::MessageF(const f5::string& format,...) {
  char buffer[1024];
  va_list va;
  va_start(va, format);
  vsprintf(buffer, format.c_str(), va);
  va_end(va);

  MUDServiceInOut::GetSelf()->GetStdOut()(buffer);
}
void MUDServiceInOut::ErrorF(const f5::string& format,...) {
  char buffer[1024];
  va_list va;
  va_start(va, format);
  vsprintf(buffer, format.c_str(), va);
  va_end(va);

  MUDServiceInOut::GetSelf()->GetErrOut()(buffer);
}
void MUDServiceInOut::VerboseF(const f5::string& format,...) {
  if (verbose) {
    char buffer[1024];
    va_list va;
    va_start(va, format);
    vsprintf(buffer, format.c_str(), va);
    va_end(va);

    MUDServiceInOut::GetSelf()->GetStdOut()(buffer);
  }
}

void MUDServiceInOut::Message(char c) {
  f5::string text;
  wchar_t t[1];
  t[0] = c;
  text.str().insert(t, 1);
  MUDServiceInOut::GetSelf()->GetStdOut()(text.c_str());
}

void MUDServiceInOut::Message(const f5::string& text) {
    MUDServiceInOut::GetSelf()->GetStdOut()(text.c_str());
}

void MUDServiceInOut::Error(char c) {
  f5::string text;
  wchar_t t[1];
  t[0] = c;
  text.str().insert(t, 1);
  MUDServiceInOut::GetSelf()->GetErrOut()(text.c_str());
}

void MUDServiceInOut::Error(const f5::string& text) {
    MUDServiceInOut::GetSelf()->GetErrOut()(text.c_str());
}

void MUDServiceInOut::Verbose(char c) {
  if (verbose) {
    f5::string text;
    wchar_t t[1];
    t[0] = c;
    text.str().insert(t, 1);
    MUDServiceInOut::GetSelf()->GetStdOut()(text.c_str());
  }
}

void MUDServiceInOut::Verbose(const f5::string& text) {
  if (verbose) {
    MUDServiceInOut::GetSelf()->GetStdOut()(text.c_str());
  }
}
