#pragma once
#include <Thor/Animations.hpp>
using thor::FrameAnimation;
using thor::AnimationMap;
using sf::IntRect;

#include "bnEntity.h"
#include "bnMobState.h"
#include "bnAI.h"
#include "bnTextureType.h"
#include "bnMobHealthUI.h"
#include "bnAnimationComponent.h"

class Mettaur : public Entity, public AI<Mettaur> {
  friend class MettaurIdleState;
  friend class MettaurMoveState;
  friend class MettaurAttackState;

public:
  Mettaur(void);
  virtual ~Mettaur(void);

  virtual void Update(float _elapsed);
  virtual void RefreshTexture();
  virtual vector<Drawable*> GetMiscComponents();
  virtual int GetStateFromString(string _string);
  virtual void SetAnimation(int _state, std::function<void()> onFinish = nullptr);
  virtual int GetHealth() const;
  virtual TextureType GetTextureType() const;

  void SetHealth(int _health);
  int* GetAnimOffset();
  int Hit(int _damage);
  float GetHitHeight() const;

private:
  const bool IsMettaurTurn() const;

  void NextMettaurTurn();

  //Old static
  float blinker;
  float x1, y1, x2, y2;
  sf::Clock clock;

  static vector<int> metIDs;
  static int currMetIndex;
  int metID;

  MobState state;

  AnimationComponent animationComponent;

  float hitHeight;
  TextureType textureType;
  MobHealthUI* healthUI;
};