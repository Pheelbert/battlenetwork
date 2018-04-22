#pragma once

#include "bnEntity.h"

template<class T>
class AIState
{
public:
  AIState() { ; }
  virtual ~AIState() { ; }

  virtual void OnEnter(T& context) = 0;
  virtual void OnUpdate(float _elapsed, T& context) = 0;
  virtual void OnLeave(T& context) = 0;
};

