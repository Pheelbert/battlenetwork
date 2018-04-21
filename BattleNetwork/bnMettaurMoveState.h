#pragma once
#include "bnAIState.h"
#include "bnMettaur.h"
#include "bnTile.h"
#include "bnField.h"

class MettaurMoveState : public AIState<Mettaur>
{
private:
  Direction nextDirection;
public:
  MettaurMoveState();
  ~MettaurMoveState();

  void OnEnter(Mettaur& met);
  void OnUpdate(float _elapsed, Mettaur& met);
  void OnLeave(Mettaur& met);
};

