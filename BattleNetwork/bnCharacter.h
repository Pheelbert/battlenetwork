#pragma once
#include "bnAnimationComponent.h"
#include "bnAnimation.h"
#include "bnEntity.h"

#include <string>
using std::string;

class Character : public Entity {
  friend class Field;

public:
  Character();
  virtual ~Character();

  virtual const bool Hit(int damage);
  virtual const float GetHitHeight() const;
  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction);
  virtual vector<Drawable*> GetMiscComponents();
  virtual void AddAnimation(string _state, FrameList _frameList, float _duration);
  virtual void SetAnimation(string _state);
  virtual void SetCounterFrame(int frame);
  virtual int GetHealth();
  virtual int* GetAnimOffset();

  void SetHealth(int _health);

  void TryDelete();
  void ToggleCounter(bool on = true);
  void Stun(double maxCooldown);
  bool IsCountered();
  // For mob UI
  const std::string GetName() const;

protected:
  int health;
  bool counterable;
  std::string name;
  double stunCooldown;
};