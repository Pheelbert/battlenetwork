#pragma once
#include "bnCharacter.h"
#include "bnMobState.h"
#include "bnAI.h"
#include "bnTextureType.h"
#include "bnMobHealthUI.h"
#include "bnAnimationComponent.h"

class Canodumb : public Character, public AI<Canodumb> {
  friend class CanodumbIdleState;
  friend class CanodumbMoveState;
  friend class CanodumbAttackState;
  friend class CanodumbCursor;

public:
  Canodumb(Rank _rank=_1);
  virtual ~Canodumb(void);

  virtual void Update(float _elapsed);
  virtual void RefreshTexture();
  virtual vector<Drawable*> GetMiscComponents();
  virtual void SetAnimation(string _state, std::function<void()> onFinish = nullptr);
  virtual void SetCounterFrame(int frame);
  virtual int GetHealth() const;
  virtual TextureType GetTextureType() const;

  void SetHealth(int _health);
  int* GetAnimOffset();
  virtual const bool Hit(int _damage);
  virtual const float GetHitHeight() const;

private:
  sf::Shader* whiteout;
  sf::Shader* stun;

  string state;

  AnimationComponent animationComponent;

  float hitHeight;
  TextureType textureType;
  MobHealthUI* healthUI;
};