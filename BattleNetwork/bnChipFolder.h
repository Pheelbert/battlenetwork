#pragma once
#include "bnChip.h"
#include "bnChipLibrary.h"
#include <list>

using std::list;

class ChipFolder {
private:
  // For now, the folder contains the entire library
  ChipLibrary::Iter iter;

public:
  ChipFolder();
  ~ChipFolder();
  Chip* Next();
};

