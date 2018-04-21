#pragma once
#include "bnAIState.h"
#include "bnMettaur.h"

class MettaurIdleState : public AIState<Mettaur>
{
  float cooldown;
public:
  MettaurIdleState();
  ~MettaurIdleState();

  void OnEnter(Mettaur& met);
  void OnUpdate(float _elapsed, Mettaur& met);
  void OnLeave(Mettaur& met);
};

