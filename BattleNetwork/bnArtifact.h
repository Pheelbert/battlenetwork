#pragma once
#include "bnEntity.h"
#include <Thor/Animations.hpp>
using thor::FrameAnimation;
using sf::Texture;

/* Artifacts do not attack and they are not living. They are tile-rooted animations purely for visual effect.*/
class Artifact : public Entity {
public:
  Artifact(void);
  Artifact(Field* _field, Team _team);
  virtual ~Artifact(void);

  virtual void Update(float _elapsed) = 0;
  virtual vector<Drawable*> GetMiscComponents() = 0;
  virtual int GetStateFromString(string _string) = 0;
  virtual void AddAnimation(int _state, FrameAnimation _animation, float _duration) = 0;

protected:
  Texture* texture;
  FrameAnimation animation;
};
