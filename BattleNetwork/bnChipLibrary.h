#pragma once
#include "bnChip.h"
#include "bnChipType.h"
#include <list>

using std::list;

class ChipLibrary {
public:
  typedef list<Chip>::const_iterator Iter;

  ChipLibrary();
  ~ChipLibrary();
  static ChipLibrary & GetInstance();

  const Iter Begin() const;
  const Iter End() const;
  const unsigned GetSize() const;
protected:
  void LoadLibrary();

private:
  list<Chip> library;
  string valueOf(string _key, string _line);
};

