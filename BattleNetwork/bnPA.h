#pragma once

#include <string>
#include <vector>
#include "bnChip.h"

/* Program Advanced class parses a PA input file into a lookup table.
   Then the class accepts chips as input during game play and replaces
   matching PA sets with a new unlisted chip.
   
   This takes place during the transition from chip custom select screen
   and battle. The names of each chip in the PA is listed one at a time,
   then the PA name is displayed and the battle continues.
   */

typedef std::pair<std::string, char> PAStep;
typedef std::vector<PAStep> PASteps;

class PA
{
  struct PAData {
    std::string name;
    unsigned icon;
    unsigned damage;
    std::string type;

    struct Required {
      std::string chipShortName;
      char code;
    };

    std::vector<Required> steps;
  };

  std::vector<PAData> advances;
  std::vector<PAData>::iterator iter;
  Chip* advanceChipRef;
public:
  PA();
  ~PA();

  void LoadPA();
  std::string valueOf(std::string _key, std::string _line);
  bool FindPA(Chip** input, unsigned size);
  const PASteps GetMatchingSteps();
  Chip* GetAdvanceChip();
};

