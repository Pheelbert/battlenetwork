#pragma once

#include "bnCharacter.h"
#include "bnPlayerState.h"
#include "bnTextureType.h"
#include "bnPlayerHealthUI.h"
#include "bnChargeComponent.h"
#include "bnAnimationComponent.h"
#include "bnAI.h"
#include "bnPlayerControlledState.h"
#include "bnPlayerIdleState.h"
#include "bnPlayerHitState.h"

using sf::IntRect;

class Player : public Character, public AI<Player> {
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

  int GetHealth() const;
  void SetHealth(int _health);
  virtual const bool Hit(int _damage);
  int GetMoveCount() const;
  int GetHitCount() const;

  PlayerHealthUI* GetHealthUI() const;

  void SetCharging(bool state);

  virtual void SetAnimation(string _state, std::function<void()> onFinish = nullptr);
private:
  int health;
  int moveCount;
  int hitCount;

  TextureType textureType;
  string state;
  PlayerHealthUI* healthUI;

  //-Animation-
  float animationProgress;

  ChargeComponent chargeComponent;
  AnimationComponent animationComponent;
};