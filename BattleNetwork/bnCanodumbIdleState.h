#pragma once
#include "bnAIState.h"
#include "bnCanodumb.h"

class CanodumbIdleState : public AIState<Canodumb>
{
public:
  CanodumbIdleState();
  ~CanodumbIdleState();

  void OnEnter(Canodumb& can);
  void OnUpdate(float _elapsed, Canodumb& can);
  void OnLeave(Canodumb& can);
};

