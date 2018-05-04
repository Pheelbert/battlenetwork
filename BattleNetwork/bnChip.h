#pragma once

#include <string>

using std::string;

// TODO: Write Chip component class to attach to Player
// Loaded chips from chip select GUI pipes into this component
class Chip {
public:
  Chip(unsigned id, unsigned icon, char code, unsigned damage, string sname, string desc);
  Chip(const Chip& copy);
  ~Chip();
  const string GetDescription();
  const string GetShortName();
  const char GetCode();
  const unsigned GetDamage();
  const unsigned GetIconID();
  const unsigned GetID();
private:
  unsigned ID;
  unsigned icon;
  unsigned damage;
  char code;
  string shortname;
  string description;
};

