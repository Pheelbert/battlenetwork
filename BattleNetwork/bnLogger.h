#pragma once
#include <iostream>
using std::cerr;
using std::endl;
#include <string>
using std::string;
using std::to_string;
#include <cstdarg>
#include <queue>

class Logger {
public:
  static void Log(string _message) {
    cerr << _message << endl;
  }

  static void Logf(const char* fmt, ...) {
    int size = 512;
    char* buffer = 0;
    buffer = new char[size];
    va_list vl;
    va_start(vl, fmt);
    int nsize = vsnprintf(buffer, size, fmt, vl);
    if (size <= nsize) {
      delete[] buffer;
      buffer = 0;
      buffer = new char[nsize + 1];
      nsize = vsnprintf(buffer, size, fmt, vl);
    }
    std::string ret(buffer);
    va_end(vl);
    delete[] buffer;
    cerr << ret << endl;
  }

  static string ToString(float _number) {
    return to_string(_number);
  }

private:
  Logger();
  ~Logger() = default;
};