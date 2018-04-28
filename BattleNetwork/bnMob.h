#pragma once
#include "bnEntity.h"
#include "bnPixelInState.h"
#include "bnMeta.h"
#include <vector>

class Mob
{
public:
  struct MobData {
    Entity* mob;
    int tileX;
    int tileY;
  };

private:
  std::vector<MobData*> spawn;
  std::vector<MobData*>::iterator iter;
  bool nextReady;

public:
  Mob() {
    nextReady = true;
  }

  ~Mob() {
    spawn.clear();
  }

  const bool NextMobReady() {
    return (nextReady && !IsDone());
  }

  const bool IsDone() {
    return (iter == spawn.end() && nextReady);
  }

  MobData* GetNextMob() {
    this->nextReady = false;
    MobData* mob = *(iter);
    iter++;
    return mob;
  }

  template<typename T>
  Mob* Add(int tileX, int tileY);

  template<typename T, typename U>
  Mob* Add(U args, int tileX, int tileY);
};

template<typename T>
Mob* Mob::Add(int tileX, int tileY) {
  _DerivedFrom<T, Entity>();
  _DerivedFrom<T, AI<T>>();

  MobData* data = new MobData();
  T* mob = new T();
  auto onFinish = [&]() { this->nextReady = true; };
  mob->StateChange<PixelInState<T>, FinishNotifier>(onFinish);

  data->mob = mob;
  data->tileX = tileX;
  data->tileY = tileY;
  spawn.push_back(data);

  iter = spawn.begin();

  return this;
}

template<typename T, typename U>
Mob* Mob::Add(U args, int tileX, int tileY) {
  _DerivedFrom<T, Entity>();
  _DerivedFrom<T, AI<T>>();

  MobData* data = new MobData();
  T* mob = new T(args);
  auto onFinish = [&]() { this->nextReady = true; };
  mob->StateChange<PixelInState<T>, FinishNotifier>(onFinish);

  data->mob = mob;
  data->tileX = tileX;
  data->tileY = tileY;
  spawn.push_back(data);

  iter = spawn.begin();

  return this;
}