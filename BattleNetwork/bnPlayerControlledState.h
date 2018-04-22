#pragma once
#include "bnAIState.h"
class Tile;
class Player;
class ControllableComponent;

class PlayerControlledState : public AIState<Player>
{
private:
  ControllableComponent * controllableComponent;

  //Cooldowns
  float moveKeyPressCooldown;
  float attackKeyPressCooldown;
  float attackToIdleCooldown;

public:
  PlayerControlledState();
  ~PlayerControlledState();


  void OnEnter(Player& player);
  void OnUpdate(float _elapsed, Player& player);
  void OnLeave(Player& player);
};

