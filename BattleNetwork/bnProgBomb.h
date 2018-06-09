#pragma once
#include "bnSpell.h"

class ProgBomb : public Spell {
private:
  Tile * target;
  float posX;
  float posY;
  float velX;
  float velY;
  float arcDuration;
  float arcProgress;
public:
  ProgBomb(Field* _field, Team _team, Tile* _target, float _duration);
  virtual ~ProgBomb(void);

  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction);
  virtual void Attack(Entity* _entity);
  virtual vector<Drawable*> GetMiscComponents();
  virtual void AddAnimation(int _state, FrameAnimation _animation, float _duration);

  void PrepareThrowPath();
};