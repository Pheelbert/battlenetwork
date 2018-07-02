#pragma once
#include "bnCharacter.h"
#include "bnMobState.h"
#include "bnAI.h"
#include "bnTextureType.h"
#include "bnMobHealthUI.h"
#include "bnAnimationComponent.h"

class Mettaur : public Character, public AI<Mettaur> {
  friend class MettaurIdleState;
  friend class MettaurMoveState;
  friend class MettaurAttackState;

public:
  Mettaur(Rank _rank);
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
  virtual const bool Hit(int _damage);
  virtual const float GetHitHeight() const;

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