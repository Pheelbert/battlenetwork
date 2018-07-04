#pragma once
#include "bnChip.h"
#include "bnChipLibrary.h"
#include <list>

using std::list;

class ChipFolder {
private:
  Chip** folderList;
  int folderSize;
  int initialSize;

public:
  ChipFolder();
  ~ChipFolder();
  Chip* Next();
};

