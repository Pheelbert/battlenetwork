#include "bnChipFolder.h"
#include <assert.h>
#include <sstream>
#include <algorithm>

ChipFolder::ChipFolder() {
  iter = ChipLibrary::GetInstance().Begin();
}


ChipFolder::~ChipFolder() {
}

Chip* ChipFolder::Next() {
  //int random = rand() % ChipLibrary::GetInstance().GetSize();

  if (iter == ChipLibrary::GetInstance().End()) {
    return nullptr;
  }

  Chip* next = new Chip(*iter);

  iter++;

  return next;
}