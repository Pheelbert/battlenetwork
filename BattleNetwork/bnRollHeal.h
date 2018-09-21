#pragma once

#include "bnSpell.h"

class RollHeal : public Spell {
public:
  RollHeal(Field* _field, Tile* _tile, Team _team, int heal);
  virtual ~RollHeal(void);

  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction);
  virtual void Attack(Entity* _entity);
  virtual vector<Drawable*> GetMiscComponents();

private:
  int heal;
};