#pragma once

#include <string>

// these asci codes are based off a specific true type font 
// and will not work with other fonts
#define EX(str) std::string(str)+";"
#define SP(str) std::string(str)+"<"
