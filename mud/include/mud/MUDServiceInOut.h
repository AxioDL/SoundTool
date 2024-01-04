#ifndef _MUDSERVICEINOUT_H_
#define _MUDSERVICEINOUT_H_

#include <f5/types.h>

#include <mud/MUDServiceInOutInterface.h>


typedef void (*MUDServicePrintFunc)(const f5::string& s);

class MUDServiceInOut : public MUDServiceInOutInterface {
  friend class MUDServiceInOutInterface;

public:
  MUDServiceInOut();
  bool IsVerbose() const { return verbose; }
  void SetVerbosity(bool on);
  void MessageF(const f5::string& format,...);
  void ErrorF(const f5::string& format,...);
  void VerboseF(const f5::string& format,...);
  void Message(char c);
  void Message(const f5::string& text);
  void Error(char c);
  void Error(const f5::string& text);
  void Verbose(char c);
  void Verbose(const f5::string& text);

  static MUDServiceInOut* GetSelf() { return self; }

  MUDServicePrintFunc GetStdOut() { return stdOut; }
  MUDServicePrintFunc GetErrOut() { return errOut; }

private:
  static MUDServiceInOut* self;
  static u32 refCount;

  static void _StdOut(const f5::string& s);
  static void _ErrOut(const f5::string& s);

  MUDServicePrintFunc stdOut;
  MUDServicePrintFunc errOut;
  bool verbose;
};

#endif // _MUDSERVICEINOUT_H_