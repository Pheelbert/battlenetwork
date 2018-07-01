#pragma once
#include "bnMobFactory.h"
#include "bnCanodumb.h"
#include "bnCanodumbIdleState.h"

class CanodumbMob :
  public MobFactory
{
public:
  CanodumbMob(Field* field);
  ~CanodumbMob();

  Mob* Build();
};
