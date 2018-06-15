#pragma once
#include <string>
#include <sstream>
#include <fstream>

class FileUtil {
public:
  static std::string Read(std::string _path) {
    std::ifstream in(_path);
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string contents(buffer.str());
    return contents;
  }
};