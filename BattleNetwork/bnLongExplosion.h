#pragma once
#include "bnSpell.h"
#include "bnField.h"

/*
TODO:
  Write a subclass of Entity called "Artifact".
  Artifacts are just effects and animations that appear in battle.
  They affiliate with no action, attack, or team

  e.g. class LongExplosion : public Artifact { .... }
 */

class LongExplosion : public Spell
{
private:
  //Animation
  float explosionProgress;
  float explosionProgress2;
  Sprite explosion;
  Sprite explosion2;
  FrameAnimation explode;
  float x1, x2, y1, y2;
public:
  LongExplosion(Field* _field, Team _team);
  ~LongExplosion();

  virtual void Update(float _elapsed);
  virtual int GetStateFromString(string _string);
  virtual void AddAnimation(int _state, FrameAnimation _animation, float _duration);
  virtual bool Move(Direction _direction) { return false; }
  virtual void Attack(Entity* _entity) { ; }
  vector<Drawable*> GetMiscComponents();

};

