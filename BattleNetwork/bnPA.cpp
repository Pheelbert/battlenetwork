#include "bnPA.h"
#include "bnTextureResourceManager.h"
#include <assert.h>

PA::PA()
{
  advanceChipRef = nullptr;
}


PA::~PA()
{
  if (advanceChipRef) {
    // PA manages the memory it creates with `new`
    delete advanceChipRef;
    advanceChipRef = nullptr;
  }

  advances.clear();
}

void PA::LoadPA()
{
  advances.clear();

  // TODO: put this utility in an input stream class and inhert from that
  string data = TextureResourceManager::GetInstance().LoadDataFromFile("resources/database/PA.txt");

  int endline = 0;
  std::vector<PA::PAData::Required> currSteps;
  std::string currPA;
  std::string damage;
  std::string icon;
  std::string type;

  do {
    endline = (int)data.find("\n");
    string line = data.substr(0, endline);

    if (line.find("PA") != string::npos) {
      currPA = valueOf("name", line);
      damage = valueOf("damage", line);
      icon = valueOf("iconIndex", line);
      type = valueOf("type", line);

      if (!currSteps.empty()) {
        std::cout << "PA entry 1: " << currPA << " " << (unsigned)atoi(icon.c_str()) << " " << (unsigned)atoi(damage.c_str()) << " " << type << endl;
        advances.push_back(PA::PAData({ currPA, (unsigned)atoi(icon.c_str()), (unsigned)atoi(damage.c_str()), type, currSteps }));
        currSteps.clear();
      }
      
    } else if (line.find("Chip") != string::npos) {
      string name = valueOf("name", line);
      string code = valueOf("code", line);

      currSteps.push_back(PA::PAData::Required({ name,code[0] }));

      std::cout << "chip step: " << name << " " << code[0] << endl;

    }

    data = data.substr(endline + 1);
  } while (endline > -1);

  std::cout << "PA entry 2: " << currPA << " " << (unsigned)atoi(icon.c_str()) << " " << (unsigned)atoi(damage.c_str()) << " " << type << endl;
  advances.push_back(PA::PAData({ currPA, (unsigned)atoi(icon.c_str()), (unsigned)atoi(damage.c_str()), type, currSteps }));
}

std::string PA::valueOf(std::string _key, std::string _line){
  int keyIndex = (int)_line.find(_key);
  assert(keyIndex > -1 && "Key was not found in PA file.");
  string s = _line.substr(keyIndex + _key.size() + 2);
  return s.substr(0, s.find("\""));
}

const PASteps PA::GetMatchingSteps()
{
  PASteps result;

  for (int i = 0; i < iter->steps.size(); i++) {
    result.push_back(std::make_pair(iter->steps[i].chipShortName, iter->steps[i].code));
  }

  return result;
}

Chip * PA::GetAdvanceChip()
{
   return advanceChipRef;
}

bool PA::FindPA(Chip ** input, unsigned size)
{
  if (size == 0) {
    return false;
  }

  for (iter = advances.begin(); iter != advances.end(); iter++) {
    bool match = true;

    if (iter->steps.size() != size) {
      return false;
    }

    for (unsigned i = 0; i < size; i++) {
      if (iter->steps[i].code != input[i]->GetCode()) {
        match = false;
        break; // stop loop
      }
    }

    if (match) {
      // Load the PA chip
      if (advanceChipRef) { delete advanceChipRef; }

      if (iter != advances.end()) {
        advanceChipRef = new Chip(0, iter->icon, 0, iter->damage, iter->name, "Program Advance");
      }
      else {
        advanceChipRef = nullptr;
      }

      return true;
    }

    // else keep looking
  }

  return false;
}

