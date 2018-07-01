#pragma once
#include "bnAIState.h"
#include "bnCanodumb.h"

class CanodumbCursor;

class CanodumbIdleState : public AIState<Canodumb>
{
  CanodumbCursor* cursor;

public:
  CanodumbIdleState();
  ~CanodumbIdleState();

  void OnEnter(Canodumb& can);
  void OnUpdate(float _elapsed, Canodumb& can);
  void OnLeave(Canodumb& can);
};

