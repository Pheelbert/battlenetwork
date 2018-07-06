#pragma once

#include <string>
#include "bnElements.h"

using std::string;

// TODO: Write Chip component class to attach to Player
// Loaded chips from chip select GUI pipes into this component
class Chip {
public:
  Chip(unsigned id, unsigned icon, char code, unsigned damage, Element element, string sname, string desc);
  Chip(const Chip& copy);
  Chip();
  ~Chip();
  const string GetDescription();
  const string GetShortName();
  const char GetCode();
  const unsigned GetDamage();
  const unsigned GetIconID();
  const unsigned GetID();
  const Element GetElement();
private:
  unsigned ID;
  unsigned icon;
  unsigned damage;
  char code;
  string shortname;
  string description;
  Element element;
};

