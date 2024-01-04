#include <mud/MUDServiceInOutInterface.h>
#include <mud/MUDServiceInOut.h>


MUDServiceInOutInterface* MUDServiceInOutInterface::Initialize() {
  if (MUDServiceInOut::self) {
    ++MUDServiceInOut::refCount;
    return MUDServiceInOut::self;
  } else {
    return MUDServiceInOut::self = new MUDServiceInOut();
  }
}

void MUDServiceInOutInterface::Shutdown() {
    --MUDServiceInOut::refCount;
  if ( !MUDServiceInOut::refCount )
  {
    if ( MUDServiceInOut::self ) {
      delete MUDServiceInOut::self;
    }

    MUDServiceInOut::self = NULL;
  }
}

MUDServiceInOutInterface::scrMessage(const char* format, ...) {
  char buffer[1024];
  va_list va;
  va_start(format, va);
  vsprintf(buffer, format, va);
  va_end(va);

  MUDServiceInOut::GetSelf()->GetStdOut()(buffer);
}

MUDServiceInOutInterface::scrVerbose(const char* format, ...) {
  if (MUDServiceInOut::GetSelf()->IsVerbose()) {
    char buffer[1024];
    va_list va;
    va_start(format, va);
    vsprintf(buffer, format, va);
    va_end(va);

    MUDServiceInOut::GetSelf()->GetStdOut()(buffer);
  }
}

MUDServiceInOutInterface::scrError(const char* format, ...) {
  char buffer[1024];
  va_list va;
  va_start(format, va);
  vsprintf(buffer, format, va);
  va_end(va);

  MUDServiceInOut::GetSelf()->GetErrOut()(buffer);
}