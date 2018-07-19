#include "bnChip.h"
#include <iostream>
#include <algorithm>

Chip::Chip(unsigned id, unsigned icon, char code, unsigned damage, Element element, string sname, string desc, unsigned rarity) :
  ID(id), icon(icon), code(code), damage(damage), element(element) {
  this->shortname.assign(sname);
  this->description.assign(desc);
  this->rarity = std::max(1, (int)rarity);
  this->rarity = std::min((int)rarity, 5);
}

Chip::Chip() {
  description = "";
  shortname = "";
}

Chip::Chip(const Chip & copy) {
  ID = copy.ID;
  icon = copy.icon;
  code = copy.code;
  damage = copy.damage;
  shortname = copy.shortname;
  description = copy.description;
  element = copy.element;
  rarity = copy.rarity;
}

Chip::~Chip() {
  if (!description.empty()) {
    description.clear();
  }

  if (!shortname.empty()) {
    shortname.clear();
  }
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

const Element Chip::GetElement()
{
  return element;
}

const unsigned Chip::GetRarity()
{
  return rarity;
}
