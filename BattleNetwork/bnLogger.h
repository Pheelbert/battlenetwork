#pragma once
#include <iostream>
using std::cerr;
using std::endl;
#include <string>
using std::string;
using std::to_string;

class Logger {
public:
  static void Log(string _message) {
    cerr << _message << endl;
  }

  static string ToString(float _number) {
    return to_string(_number);
  }

private:
  Logger();
  ~Logger();
};