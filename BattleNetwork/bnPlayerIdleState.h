#pragma once
#include "bnAIState.h"
class Player;

class PlayerIdleState : public AIState<Player>
{
private:

public:
  PlayerIdleState();
  ~PlayerIdleState();


  void OnEnter(Player& player);
  void OnUpdate(float _elapsed, Player& player);
  void OnLeave(Player& player);
};

