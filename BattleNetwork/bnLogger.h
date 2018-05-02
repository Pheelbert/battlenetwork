#pragma once
#include <iostream>
using std::cout;
using std::endl;
using std::cin;
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
  static void Failf(const string& _format, Args... args) {
    fprintf_s(stderr, ("ERROR: " + _format).c_str(), args...);
    cin.ignore(); // Allows us to see the error before the window closes
    exit(EXIT_FAILURE);
  }

private:
  Logger();
  ~Logger() = default;
};