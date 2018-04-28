#pragma once
#include "bnMobFactory.h"
#include "bnMettaur.h"
#include "bnMettaurIdleState.h"

class RandomMettaurMob :
  public MobFactory
{
public:
  RandomMettaurMob(Field* field);
  ~RandomMettaurMob();

  Mob* Build();
};

