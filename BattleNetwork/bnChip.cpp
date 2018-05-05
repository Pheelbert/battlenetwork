#include "bnChip.h"
#include <iostream>

Chip::Chip(unsigned id, unsigned icon, char code, unsigned damage, string sname, string desc) :
  ID(id), icon(icon), code(code), damage(damage), shortname(sname), description(desc) {
}

Chip::Chip(const Chip & copy) {
  ID = copy.ID;
  icon = copy.icon;
  code = copy.code;
  damage = copy.damage;
  shortname = copy.shortname;
  description = copy.description;
}

Chip::~Chip() {
  std::cout << "chip " << shortname << " deleted" << std::endl;
}

const string Chip::GetDescription() {
  return description;
}

const string Chip::GetShortName() {
  return shortname;
}

const char Chip::GetCode() {
  return code;
}

const unsigned Chip::GetDamage() {
  return damage;
}

const unsigned Chip::GetIconID()
{
  return icon;
}

const unsigned Chip::GetID() {
  return ID;
}
