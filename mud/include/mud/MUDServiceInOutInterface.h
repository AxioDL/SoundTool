#ifndef _MUDSERVICEINOUTINTERFACE_H_
#define _MUDSERVICEINOUTINTERFACE_H_


#include <f5/f5classes.h>

class MUDServiceInOutInterface {
public:
  virtual void SetVerbosity(bool on)=0;
  virtual void MessageF(const f5::string& format,...)=0;
  virtual void ErrorF(const f5::string& format,...)=0;
  virtual void VerboseF(const f5::string& format,...)=0;
  virtual void Message(char c)=0;
  virtual void Message(const f5::string& text)=0;
  virtual void Error(char c)=0;
  virtual void Error(const f5::string& text)=0;
  virtual void Verbose(char c)=0;
  virtual void Verbose(const f5::string& text)=0;

  static scrMessage(const char* format, ...);
  static scrVerbose(const char* format, ...);
  static scrError(const char* format, ...);

  static MUDServiceInOutInterface* Initialize();
  static void Shutdown();
};

#endif // _MUDSERVICEINOUTINTERFACE_H_