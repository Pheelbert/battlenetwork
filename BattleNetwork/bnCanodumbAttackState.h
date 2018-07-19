#pragma once
#include "bnAIState.h"
#include "bnCanodumb.h"

class CanodumbAttackState : public AIState<Canodumb>
{
public:
  CanodumbAttackState();
  ~CanodumbAttackState();

  void OnEnter(Canodumb& can);
  void OnUpdate(float _elapsed, Canodumb& can);
  void OnLeave(Canodumb& can);
};

