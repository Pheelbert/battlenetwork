#pragma once
#include <Thor/Animations.hpp>
using thor::FrameAnimation;
using thor::AnimationMap;
using sf::IntRect;

#include "bnEntity.h"
#include "bnMobState.h"
#include "bnTextureType.h"
#include "bnMobHealthUI.h"
#include "bnProgsManIdleState.h"
#include "bnAnimationComponent.h"
#include "bnAI.h"

class ProgsMan : public Entity, public AI<ProgsMan> {
public:
  friend class ProgsManIdleState;
  friend class ProgsManMoveState;
  friend class ProgsManAttackState;

  ProgsMan(void);
  virtual ~ProgsMan(void);

  virtual void Update(float _elapsed);
  virtual void RefreshTexture();
  virtual vector<Drawable*> GetMiscComponents();
  virtual void SetAnimation(int _state, std::function<void()> onFinish = nullptr);
  virtual int GetStateFromString(string _string);
  virtual int GetHealth() const;
  virtual TextureType GetTextureType() const;

  MobState GetMobState() const;
  void SetHealth(int _health);
  int Hit(int _damage);
  float GetHitHeight() const;
  int* GetAnimOffset();
  void Attack();

  void SetTarget(Entity*);

private:
  sf::Clock clock;

  //Cooldowns
  float cooldown;
  float attackCooldown;
  float waitCooldown;

  //Animation
  float attackDelay;
 
  AnimationComponent animationComponent;

  float hitHeight;
  Direction direction;
  MobState state;
  TextureType textureType;
  MobHealthUI* healthUI;
  AnimationMap<Sprite, MobState> animator;
  sf::Shader* whiteout;

  // AI
  Entity* target;
};