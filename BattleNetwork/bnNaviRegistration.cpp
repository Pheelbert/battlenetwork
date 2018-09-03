#include "bnNaviRegistration.h"
#include "bnPlayer.h"
#include "bnLogger.h"
#include <exception>
#include <atomic>

NaviRegistration::NaviInfo::NaviInfo() : symbol()
{
  navi = nullptr;
  special = "None";
  atk = 1;
  chargedAtk = 1;
  speed = 1;
  hp = 1;
  isSword = false;
}

NaviRegistration::NaviInfo::~NaviInfo()
{
  if (navi != nullptr) {
    delete navi;
  }
}

NaviRegistration::NaviInfo& NaviRegistration::NaviInfo::SetSymbol(sf::Texture& symbol)
{
  this->symbol = sf::Sprite(symbol);

  // Symbols are 15x15
  this->symbol.setTextureRect(sf::IntRect(0, 0, 15, 15));

  return *this;
}

NaviRegistration::NaviInfo& NaviRegistration::NaviInfo::SetSpecial(const std::string && special)
{
  this->special = special;
  return *this;
}

NaviRegistration::NaviInfo& NaviRegistration::NaviInfo::SetAttack(const int atk)
{
  this->atk = atk;
  return *this;
}

NaviRegistration::NaviInfo& NaviRegistration::NaviInfo::SetChargedAttack(const int atk)
{
  this->chargedAtk = atk;
  return *this;
}

NaviRegistration::NaviInfo& NaviRegistration::NaviInfo::SetSpeed(const double speed)
{
  this->speed = speed;
  return *this;
}

NaviRegistration::NaviInfo& NaviRegistration::NaviInfo::SetHP(const int HP)
{
  hp = HP;
  return *this;
}

NaviRegistration::NaviInfo& NaviRegistration::NaviInfo::SetIsSword(const bool enabled)
{
  isSword = enabled;
  return *this;
}

NaviRegistration & NaviRegistration::GetInstance()
{
 static NaviRegistration singleton; return singleton; 
}

NaviRegistration::~NaviRegistration()
{
  for (int i = 0; i < Size(); i++) {
    delete roster[i];
  }

  roster.clear();
}

NaviRegistration::NaviInfo * NaviRegistration::AddSpot()
{
  return new NaviRegistration::NaviInfo();
}

void NaviRegistration::Register(const NaviInfo * info)
{
  roster.push_back(info);
}

const NaviRegistration::NaviInfo & NaviRegistration::At(int index)
{
  if (index < 0 || index > Size())
    throw std::runtime_error("Roster index out of bounds");

  return *roster.at(index);
}

const unsigned NaviRegistration::Size()
{
  return roster.size();
}

void NaviRegistration::LoadAllNavis(std::atomic<int>& progress)
{
  for (int i = 0; i < Size(); i++) {
    roster[i]->loadNaviClass();

    Logger::GetMutex()->lock();
    Logger::Logf("Loaded navi: %s", roster[i]->navi->GetName());
    Logger::GetMutex()->unlock();

    progress++;
  }
}
