#include "bnChipFolder.h"
#include <assert.h>
#include <sstream>
#include <algorithm>

ChipFolder::ChipFolder() {
  folderSize = initialSize = ChipLibrary::GetInstance().GetSize();
  folderList = new Chip*[folderSize];

  for (int i = 0; i < folderSize; i++) {
    int random = rand() % ChipLibrary::GetInstance().GetSize();

    // For now, the folder contains random parts from the entire library
    ChipLibrary::Iter iter = ChipLibrary::GetInstance().Begin();

    while (random-- > 0) {
      iter++;
    }

    folderList[i] = new Chip(*(iter));
  }
}


ChipFolder::~ChipFolder() {
  for (int i = folderSize-1; i >= 0; i--) {
    delete folderList[i];
  }

  delete[] folderList;
}

Chip* ChipFolder::Next() {
  if (--folderSize < 0) {
    return nullptr;
  }

  return folderList[folderSize];
}