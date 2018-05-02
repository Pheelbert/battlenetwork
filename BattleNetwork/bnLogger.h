#pragma once
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
using std::to_string;
#include <cstdarg>

class Logger {
public:
  static void Log(string _message) {
    cout << _message << endl;
  }

  template<typename ... Args>
  static void Logf(const char* _format, Args... args) {
    fprintf_s(stdout, _format, args...);
  }

  template<typename ... Args>
  static void Failf(const char* _format, Args... args) {
    fprintf_s(stderr, _format, args...);
    exit(EXIT_FAILURE);
  }

private:
  Logger();
  ~Logger() = default;
};