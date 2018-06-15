#pragma once
#include <SFML\Graphics.hpp>
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
  virtual void SetAnimation(string _state, std::function<void()> onFinish = nullptr);
  virtual int GetHealth() const;
  virtual TextureType GetTextureType() const;

  void SetHealth(int _health);
  int Hit(int _damage);
  float GetHitHeight() const;
  int* GetAnimOffset();

private:
  sf::Clock clock;
 
  AnimationComponent animationComponent;

  float hitHeight;
  string state;
  TextureType textureType;
  MobHealthUI* healthUI;
  sf::Shader* whiteout;
};