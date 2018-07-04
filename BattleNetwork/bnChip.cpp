#include "bnChip.h"
#include <iostream>

#undef  _ITERATOR_DEBUG_LEVEL
#define _ITERATOR_DEBUG_LEVEL 0

Chip::Chip(unsigned id, unsigned icon, char code, unsigned damage, Element element, string sname, string desc) :
  ID(id), icon(icon), code(code), damage(damage), element(element) {
  this->shortname = sname;
  this->description = "";
}

Chip::Chip(const Chip & copy) {
  ID = copy.ID;
  icon = copy.icon;
  code = copy.code;
  damage = copy.damage;
  shortname = copy.shortname;
  description = "";
  element = copy.element;
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
