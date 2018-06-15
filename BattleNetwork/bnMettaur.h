#pragma once
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
  virtual void SetAnimation(string _state, std::function<void()> onFinish = nullptr);
  virtual void SetCounterFrame(int frame);
  virtual int GetHealth() const;
  virtual TextureType GetTextureType() const;

  void SetHealth(int _health);
  int* GetAnimOffset();
  int Hit(int _damage);
  float GetHitHeight() const;

private:
  const bool IsMettaurTurn() const;

  void NextMettaurTurn();

  sf::Shader* whiteout;
  sf::Shader* stun;

  static vector<int> metIDs;
  static int currMetIndex;
  int metID;

  string state;

  AnimationComponent animationComponent;

  float hitHeight;
  TextureType textureType;
  MobHealthUI* healthUI;
};