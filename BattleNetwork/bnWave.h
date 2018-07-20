#pragma once
#include "bnSpell.h"
#include "bnAnimate.h"
class Wave : public Spell {
protected:
  Animate animator;
  FrameList animation;
public:
  Wave(Field* _field, Team _team);
  virtual ~Wave(void);

  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction);
  virtual void Attack(Entity* _entity);
  virtual vector<Drawable*> GetMiscComponents();
};