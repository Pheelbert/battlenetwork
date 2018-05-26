#pragma once
#include <Thor/Animations.hpp>
using thor::FrameAnimation;
using thor::Animator;
using sf::IntRect;

#include "bnEntity.h"
#include "bnPlayerState.h"
#include "bnTextureType.h"
#include "bnPlayerHealthUI.h"
#include "bnChargeComponent.h"
#include "bnAnimationComponent.h"
#include "bnSelectedChipsUI.h"
#include "bnAI.h"
#include "bnPlayerControlledState.h"
#include "bnPlayerIdleState.h"
#include "bnPlayerHitState.h"

class Player : public Entity, public AI<Player> {
public:
  friend class PlayerControlledState;
  friend class PlayerIdleState;

  Player(void);
  virtual ~Player(void);

  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction);
  void AdoptNextTile();
  void Attack(float _charge);
  virtual vector<Drawable*> GetMiscComponents();
  void RefreshTexture();

  int GetHealth() const;
  void SetHealth(int _health);
  int Hit(int _damage);

  PlayerHealthUI* GetHealthUI() const;
  SelectedChipsUI * GetChipsUI() const;

  virtual int GetStateFromString(string _string);
  virtual void SetAnimation(int _state, std::function<void()> onFinish = nullptr);
private:
  int health;

  TextureType textureType;
  PlayerState state;
  PlayerHealthUI* healthUI;
  SelectedChipsUI* chipsUI;

  //-Animation-
  float animationProgress;

  ChargeComponent chargeComponent;
  AnimationComponent animationComponent;
};