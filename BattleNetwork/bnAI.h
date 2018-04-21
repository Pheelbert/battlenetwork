#pragma once
#include "bnAIState.h"
#include "bnEntity.h"

template<typename T>
class AI {
private:
  AIState<T>* stateMachine;
  Entity* target;
  T* ref;
public:
  AI(T* _ref) { stateMachine = nullptr; ref = _ref; }
  ~AI() { if (stateMachine) { delete stateMachine; ref = nullptr; target = nullptr; } }

  void StateChange(AIState<T>* _state) {
    if (stateMachine) {
      stateMachine->OnLeave(*ref);
      delete stateMachine;
    }

    stateMachine = _state;
    stateMachine->OnEnter(*ref);
  }

  void StateUpdate(float _elapsed) {
    if (stateMachine) {
      stateMachine->OnUpdate(_elapsed, *ref);
    }
  }

public:
  void SetTarget(Entity* _target) {
    target = _target;
  }

  Entity* GetTarget() const { return target;  }
};

