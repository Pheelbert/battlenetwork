#pragma once
#include "bnEntity.h"
using sf::Texture;

/* Artifacts do not attack and they are not living. They are tile-rooted animations purely for visual effect.*/
class Artifact : public Entity {
public:
  Artifact(void);
  Artifact(Field* _field, Team _team);
  virtual ~Artifact(void);

  virtual void Update(float _elapsed) = 0;

protected:
  Texture* texture;
};
