#pragma once
#include "bnMobFactory.h"
class ProgsManBossFight :
  public MobFactory
{
public:
  ProgsManBossFight(Field* field);
  ~ProgsManBossFight();

  Mob* Build();
};

