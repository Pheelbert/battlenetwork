#pragma once
#include "bnAIState.h"
#include "bnMettaur.h"

class MettaurAttackState : public AIState<Mettaur>
{
public:
  MettaurAttackState();
  ~MettaurAttackState();

  void OnEnter(Mettaur& met);
  void OnUpdate(float _elapsed, Mettaur& met);
  void OnLeave(Mettaur& met);

  // specialized state functions
  void DoAttack(Mettaur& met);
};

