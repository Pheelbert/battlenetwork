#pragma once

#include "bnEntity.h"
#include "bnMeta.h"

template<class T>
class AIState
{
private:
  AIState<T>* nextState;

public:
  AIState() { ; }
  AIState(const AIState<T>& rhs) = default;
  AIState(AIState<T>&& ref) = default;

  template<class U>
  void ChangeState() {
    _DerivedFrom<U, AIState<T>>();

    nextState = new U();
  }

  AIState<T>* Update(float _elapsed, T& context) {
    nextState = nullptr;

    OnUpdate(_elapsed, context);
    if (nextState) {
      return nextState;
    }

    return nullptr;
  }

  virtual ~AIState() { ; }

  virtual void OnEnter(T& context) = 0;
  virtual void OnUpdate(float _elapsed, T& context) = 0;
  virtual void OnLeave(T& context) = 0;
};

