#pragma once

#include <map>
#include <vector>
#include <functional>
#include <atomic>
#include <SFML/Graphics.hpp>

/*
  Use this singleton to register custom navis and have them automatically appear on the select, overworld, and battle scenes
*/

class Player; // forward decl

class NaviRegistration {
public:
  class NaviInfo {
    friend class NaviRegistration;

    Player* navi;
    sf::Sprite symbol;
    std::string special;
    int atk;
    int chargedAtk;
    double speed;
    int hp;
    bool isSword;

    std::function<void()> loadNaviClass;
  public:
    NaviInfo();
    ~NaviInfo();

    template<class T> NaviInfo& SetNaviClass();
    NaviInfo& SetSymbol(sf::Texture& symbol);
    NaviInfo& SetSpecial(const std::string&& special);
    NaviInfo& SetAttack(const int atk);
    NaviInfo& SetChargedAttack(const int atk);
    NaviInfo& SetSpeed(const double speed);
    NaviInfo& SetHP(const int HP);
    NaviInfo& SetIsSword(const bool enabled);
  };

private:
  std::vector<const NaviInfo*> roster;

public:
  static NaviRegistration &GetInstance();
  ~NaviRegistration();

  NaviInfo* AddSpot();
  void Register(const NaviInfo* info);
  const NaviInfo& At(int index);
  const unsigned Size();
  void LoadAllNavis(std::atomic<int>& progress);
  
};

#define NAVIS NaviRegistration::GetInstance()

// Deffered loading design pattern
template<class T>
inline NaviRegistration::NaviInfo & NaviRegistration::NaviInfo::SetNaviClass()
{
  loadNaviClass = [this]() { navi = new T(); };
  return *this;
}
