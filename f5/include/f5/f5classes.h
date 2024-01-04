#ifndef _F5STRING_H_
#define _F5STRING_H_

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <assert.h>

#include <string>
#include <f5/types.h>

namespace f5 {
class string{
public:
  typedef std::basic_string<wchar_t> base_type;

  string() {
    Init();
  }
  string(char s) {
    char tmp[1];
    tmp[0] = s;
    *this = tmp;
    Init();
  }
  string(const char* s) {
    *this = s;
    Init();
  }
	string(const std::basic_string<wchar_t>& ws) : wstr(ws) {
		Init();
	}
#if 1
  ~string() {
    if (cBuffer != NULL) {
      delete cBuffer;
    }
    cBuffer = NULL;
    cBufferLen = 0;
  }
#endif
	void Init() {
		cBuffer = NULL;
		cBufferLen = 0;
	}

  const wchar_t* w_str() const {
    return wstr.c_str();
  }

  const char* c_str() const {
#if _WIN32
    size_t len;
    len = WideCharToMultiByte(0, 0, w_str(), -1, 0, 0, NULL, FALSE);
    assert(len!=0);

    if (cBufferLen != len) {
      delete cBuffer;
      cBuffer = new char[len];

      assert(cBuffer!=NULL);
      cBufferLen = len;
    }

    WideCharToMultiByte(0, 0, w_str(), -1, cBuffer, len, NULL, FALSE);
#endif
    return cBuffer;
  }

  unsigned int find(const char* s, size_t pos = base_type::npos) {
    return wstr.find(string(s).w_str(), pos);
  }

  unsigned int find(char c, size_t pos = base_type::npos) {
    return wstr.find(string(c).w_str(), pos);
  }

  unsigned int find_first_of(const char* s, size_t pos = base_type::npos) const {
    return wstr.find_first_of(string(s).w_str(), pos);
  }

  unsigned int find_first_of(const char c, size_t pos = base_type::npos) const {
    return wstr.find_first_of(string(c).w_str(), pos);
  }

  unsigned int find_last_of(const char* s, size_t pos = base_type::npos) const {
    return wstr.find_last_of(string(s).w_str(), pos);
  }

  unsigned int find_last_of(const char c, size_t pos = base_type::npos) const {
    return wstr.find_last_of(string(c).w_str(), pos);
  }


  string operator+(const char* right) const {
    return string(wstr).wstr.append(string(right).wstr);
  }

  string operator+(const string& right) const {
    return string(wstr).wstr.append(right.wstr);
  }

  string& operator+=(const string& right) {
    wstr.append(right.wstr);
    return *this;
  }


  string& operator=(const char* s) {
#if _WIN32
    wchar_t* wcBuffer;
    size_t len = MultiByteToWideChar(0, 0, (LPCSTR)s, -1, NULL, NULL);

    assert(len!=0);

    wcBuffer = new wchar_t[len];

    MultiByteToWideChar(0, 0, (LPCSTR)s, -1, (LPWSTR)wcBuffer, len);
    wstr.assign(wcBuffer);
    delete wcBuffer;
#endif
    return *this;
  }

  string& operator=(const string& s) {
    wstr.assign(s.wstr);
    return *this;
  }

  size_t size() const { return wstr.size(); }

  void resize(size_t newsz) {
    wstr.resize(newsz);
  }

  void resize(size_t newsz, char c) {
    wstr.resize(newsz, c);
  }

  f5::string& erase(size_t pos = 0, size_t len = base_type::npos) {
    wstr.erase(pos, len);
    return *this;
  }

  base_type& str() { return wstr; }
  const base_type& str() const { return wstr; }
private:
  std::basic_string<wchar_t> wstr;
	mutable char* cBuffer;
	mutable size_t cBufferLen;
};
}

#endif //_F5STRING_H_