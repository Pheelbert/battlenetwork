#pragma once
#include "bnChip.h"
#include "bnChipType.h"
#include <list>

using std::list;

class ChipLibrary {
public:
  ChipLibrary();
  ~ChipLibrary();
  static ChipLibrary & GetInstance();
  void LoadLibrary();
  string valueOf(string _key, string _line);
  Chip* Next();
private:
  list<Chip> library;
};

