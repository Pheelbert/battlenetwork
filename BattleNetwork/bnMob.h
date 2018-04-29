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
    unsigned index;
  };

private:
  std::vector<MobData*> spawn;
  std::vector<MobData*>::iterator iter;
  std::vector<std::function<void(Entity*)>> defaultStateInvokers;
  std::vector<std::function<void(Entity*)>> pixelStateInvokers;
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
    for (int i = 0; i < defaultStateInvokers.size(); i++) {
      defaultStateInvokers[i](spawn[i]->mob);
    }

    defaultStateInvokers.clear();
  }

  MobData* GetNextMob() {
    this->nextReady = false;
    MobData* data = *(iter);
    iter++;
    pixelStateInvokers[data->index](data->mob);
    return data;
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

  data->mob = mob;
  data->tileX = tileX;
  data->tileY = tileY;
  data->index = spawn.size();

  // This retains the current entity type and stores it in a function. We do this to transform the 
  // unknown type back later and can call the proper state change
  auto pixelStateInvoker = [this](Entity* mob) {
    T* cast = dynamic_cast<T*>(mob); 
    
    if (cast) {
      auto onFinish = [this]() { this->nextReady = true; };
      cast->StateChange<PixelInState<T>, FinishNotifier>(onFinish);
    }  
  };

  pixelStateInvokers.push_back(pixelStateInvoker);

  auto defaultStateInvoker = [](Entity* mob) { T* cast = dynamic_cast<T*>(mob); if (cast) { cast->StateChange<DefaultState>(); } };
  defaultStateInvokers.push_back(defaultStateInvoker);

  spawn.push_back(data);

  iter = spawn.begin();

  return this;
}