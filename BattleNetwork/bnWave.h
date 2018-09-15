#pragma once
#include "bnSpell.h"
#include "bnAnimate.h"
class Wave : public Spell {
protected:
  Animate animator;
  FrameList animation;
  double speed;
public:
  Wave(Field* _field, Team _team, double speed = 1.0);
  virtual ~Wave(void);

  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction);
  virtual void Attack(Entity* _entity);
  virtual vector<Drawable*> GetMiscComponents();
};