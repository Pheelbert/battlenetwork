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
  std::vector<std::function<void(Entity*)>> invokers;
  bool nextReady;
  Field* field;
public:
  Mob(Field* _field) {
    nextReady = true;
    field = _field;
  }

  ~Mob() {
    // spawn.clear();
  }

  Field* GetField() {
    return field;
  }

  const bool NextMobReady() {
    return (nextReady && !IsSpawningDone());
  }

  const bool IsSpawningDone() {
    return (iter == spawn.end() && nextReady);
  }

  const bool IsCleared() {
    for (int i = 0; i < spawn.size(); i++) {
      if (!spawn[i]->mob->IsDeleted())
        return false;
    }

    return true;
  }

  void DefaultState() {
    for (int i = 0; i < invokers.size(); i++) {
      invokers[i](spawn[i]->mob);
    }

    invokers.clear();
  }

  MobData* GetNextMob() {
    this->nextReady = false;
    MobData* mob = *(iter);
    iter++;
    return mob;
  }

  template<typename T, typename DefaultState>
  Mob* Spawn(int tileX, int tileY);
};

template<typename T, typename DefaultState>
Mob* Mob::Spawn(int tileX, int tileY) {
  // TODO: assert that tileX and tileY exist in field

  _DerivedFrom<T, Entity>();
  _DerivedFrom<T, AI<T>>();

  MobData* data = new MobData();
  T* mob = new T();
  auto onFinish = [&]() { this->nextReady = true; };
  mob->StateChange<PixelInState<T>, FinishNotifier>(onFinish);
  data->mob = mob;
  data->tileX = tileX;
  data->tileY = tileY;

  // This retains the current entity type and stores it in a function. We do this to transform the 
  // unknown type back later and can call the proper state change
  auto defaultStateInvoker = [](Entity* mob) { T* cast = dynamic_cast<T*>(mob); if (cast) { cast->StateChange<DefaultState>(); } };
  invokers.push_back(defaultStateInvoker);

  spawn.push_back(data);

  iter = spawn.begin();

  return this;
}