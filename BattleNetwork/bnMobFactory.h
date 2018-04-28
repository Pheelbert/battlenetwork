#pragma once
#include "bnMob.h"

class MobFactory
{
protected:
  Field * field;
public:
  MobFactory(Field* _field) { field = _field; }
  virtual ~MobFactory() { ;  }

  virtual Mob* Build() = 0;
};

