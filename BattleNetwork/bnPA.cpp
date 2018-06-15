#include "bnPA.h"
#include "bnLogger.h"
#include "bnFileUtil.h"
#include <assert.h>
#include <iostream>

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
  string data = FileUtil::Read("resources/database/PA.txt");

  int endline = 0;
  std::vector<PA::PAData::Required> currSteps;
  std::string currPA;
  std::string damage;
  std::string icon;
  std::string type;

  do {
    endline = (int)data.find("\n");
    string line = data.substr(0, endline);

    while (line.compare(0, 1, " ") == 0)
      line.erase(line.begin()); // remove leading whitespaces
    while (line.size() > 0 && line.compare(line.size() - 1, 1, " ") == 0)
      line.erase(line.end() - 1); // remove trailing whitespaces

    if (line[0] == '#') {
      // Skip comments
      data = data.substr(endline + 1);
      continue;
    }

    if (line.find("PA") != string::npos) {
      if (!currSteps.empty()) {
        if (currSteps.size() > 1) {
          std::cout << "PA entry 1: " << currPA << " " << (unsigned)atoi(icon.c_str()) << " " << (unsigned)atoi(damage.c_str()) << " " << type << endl;
          advances.push_back(PA::PAData({ currPA, (unsigned)atoi(icon.c_str()), (unsigned)atoi(damage.c_str()), type, currSteps }));
          currSteps.clear();
        }
        else {
          //std::cout << "Error. PA " + currPA + " only has 1 required chip for recipe. PA's must have 2 or more chips. Skipping entry.\n";
          Logger::Log("Error. PA \"" + currPA + "\": only has 1 required chip for recipe. PA's must have 2 or more chips. Skipping entry.");
          currSteps.clear();
        }
      }
      
      currPA = valueOf("name", line);
      damage = valueOf("damage", line);
      icon = valueOf("iconIndex", line);
      type = valueOf("type", line);
    } else if (line.find("Chip") != string::npos) {
      string name = valueOf("name", line);
      string code = valueOf("code", line);

      currSteps.push_back(PA::PAData::Required({ name,code[0] }));

      //std::cout << "chip step: " << name << " " << code[0] << endl;

    }

    data = data.substr(endline + 1);
  } while (endline > -1);

  if (currSteps.size() > 1) {
    //std::cout << "PA entry 2: " << currPA << " " << (unsigned)atoi(icon.c_str()) << " " << (unsigned)atoi(damage.c_str()) << " " << type << endl;
    advances.push_back(PA::PAData({ currPA, (unsigned)atoi(icon.c_str()), (unsigned)atoi(damage.c_str()), type, currSteps }));
    currSteps.clear();
  }
  else {
    //std::cout << "Error. PA " + currPA + " only has 1 required chip for recipe. PA's must have 2 or more chips. Skipping entry.\n";
    Logger::Log("Error. PA \"" + currPA + "\": only has 1 required chip for recipe. PA's must have 2 or more chips. Skipping entry.");
    currSteps.clear();
  }
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
  std::cout << "size: " << size << "\n";

  if (size == 0) {
    return false;
  }

  for (iter = advances.begin(); iter != advances.end(); iter++) {
    bool match = false;
    
    std::cout << "iter->steps.size() " << iter->steps.size() << "\n";

    if (iter->steps.size() > size) {
      continue; // try next 
    }

    for (unsigned i = 0; i < iter->steps.size(); i++) {
      char code = input[i]->GetCode();

      if (code == '=') { code = '*'; } // Transform back from compatible font char

      std::cout << "iter->steps[i].code " << iter->steps[i].code << "\n";
      std::cout << "code " << code << "\n";
      std::cout << "iter->steps[i].chipShortName " << iter->steps[i].chipShortName << "\n";
      std::cout << "input[i]->GetShortName() " << input[i]->GetShortName() << "\n";

      if (iter->steps[i].code != code) {
        match = false;
        break; // stop loop
      }
      // Ensure that the chip code and name matches as those are the best way to identify chips
      else if (iter->steps[i].chipShortName != input[i]->GetShortName()) {
        match = false;
        break; // stop loop
      }
      else {
        match = true;
        // We do not break here. If it is a match all across the steps, then the for loop ends 
        // and match stays == true
      }
    }

    if (match) {
      // Load the PA chip
      if (advanceChipRef) { delete advanceChipRef; }

       advanceChipRef = new Chip(0, iter->icon, 0, iter->damage, iter->name, "Program Advance");

      return true;
    }

    // else keep looking
  }

  return false;
}

