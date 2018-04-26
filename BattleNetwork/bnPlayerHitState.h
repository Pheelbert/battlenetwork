#pragma once
#include "bnAI.h"
class Player;
class PlayerHitState : public AIState<Player>
{
private:
  float cooldown;

public:

  PlayerHitState(float);
  ~PlayerHitState();

  void OnEnter(Player& player);
  void OnUpdate(float _elapsed, Player& player);
  void OnLeave(Player& player);
};

