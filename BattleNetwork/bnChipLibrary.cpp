#include "bnChipLibrary.h"


ChipLibrary::ChipLibrary() {
}


ChipLibrary::~ChipLibrary() {
}

ChipLibrary& ChipLibrary::GetInstance() {
  static ChipLibrary instance;
  return instance;
}

Chip* ChipLibrary::Next() {
  if (library.empty()) {
    return nullptr;
  }

  int random = rand() % library.size();
  Chip* next = nullptr;

  list<Chip>::iterator it = library.begin();

  for (int i = 0; i < random; i++) {
    it++;
  }

  next = new Chip(*it);
  library.erase(it);

  return next;
}

// Used as the folder in battle
void ChipLibrary::LoadLibrary() {
  // NOTE: The IDs (first param) be indexed by the frames in the chip cards texture!
  // SEE:  resources/ui/chip_cards.png
  library.insert(library.begin(), Chip((unsigned)0, 'A', 20, "Cannon\0", "Hits anything directly in front\0"));
  library.insert(library.begin(), Chip((unsigned)0, 'A', 20, "Cannon\0", "Hits anything directly in front\0"));
  library.insert(library.begin(), Chip((unsigned)15, 'J', 80, "ShrtSwrd\0", "Attacks 2 steps forward front\0"));
  library.insert(library.begin(), Chip(ChipType::CRCKPNL, '=', 0, "CrckPnl\0", "Cracks 3 panels wide\0"));
  library.insert(library.begin(), Chip(ChipType::HP10, 'F', 10, "HP+10\0", "Restores 10 HP\0"));
  library.insert(library.begin(), Chip(ChipType::HP10, 'E', 10, "HP+10\0", "Restores 10 HP\0"));
  library.insert(library.begin(), Chip(ChipType::HP10, 'A', 10, "HP+10\0", "Restores 10 HP\0"));
  library.insert(library.begin(), Chip(ChipType::INVSBLE, 'I', 0, "Invsble\0", "Hide for an amount of time\0"));
  library.insert(library.begin(), Chip(ChipType::BARR, 'B', 10, "Barr\0", "Prevent damage\0"));
}
