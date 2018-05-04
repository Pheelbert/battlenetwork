#include "bnChipLibrary.h"
#include "bnTextureResourceManager.h"
#include <assert.h>

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


  // TODO: put this in an input stream class
  string data = TextureResourceManager::GetInstance().LoadDataFromFile("resources/database/library.txt");

  int endline = 0;

  do {
    endline = (int)data.find("\n");
    string line = data.substr(0, endline);

    if (line.find("Chip") != string::npos) {
      string cardID = valueOf("cardIndex", line);
      string iconID = valueOf("iconIndex", line);
      string name   = valueOf("name", line);
      string damage = valueOf("damage", line);
      string type = valueOf("type", line);
      string codes = valueOf("codes", line);

      library.push_back(Chip(atoi(cardID.c_str()), atoi(iconID.c_str()), codes[0], atoi(damage.c_str()), name, "description"));
    }

    data = data.substr(endline + 1);
  } while (endline > -1);
}

string ChipLibrary::valueOf(string _key, string _line) {
  int keyIndex = (int)_line.find(_key);
  assert(keyIndex > -1 && "Key was not found in library file.");
  string s = _line.substr(keyIndex + _key.size() + 2);
  return s.substr(0, s.find("\""));
}