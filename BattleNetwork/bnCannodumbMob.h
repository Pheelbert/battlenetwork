#pragma once
#include "bnMobFactory.h"
#include "bnMettaur.h"
#include "bnMettaurIdleState.h"
// #include "bnCannodumb.h"

class CannodumbMob :
  public MobFactory
{
public:
  CannodumbMob(Field* field);
  ~CannodumbMob();

  Mob* Build();
};
