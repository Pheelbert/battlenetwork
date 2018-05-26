#pragma once
#include "bnMobFactory.h"
#include "bnMettaur.h"
#include "bnMettaurIdleState.h"

class TwoMettaurMob :
  public MobFactory
{
public:
  TwoMettaurMob(Field* field);
  ~TwoMettaurMob();

  Mob* Build();
};

