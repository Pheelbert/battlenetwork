#pragma once

#include <string>

class BattleItem {
private:
  int cardID;
  std::string name;
public:
  BattleItem(std::string name, int id) : name(name), cardID(id) { ; }
  BattleItem(const BattleItem& rhs) { cardID = rhs.cardID; name = rhs.name; }
  int GetID() { return cardID; }
  const std::string GetName() { return name;  }
};