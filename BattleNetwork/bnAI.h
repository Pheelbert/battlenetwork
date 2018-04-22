#pragma once
#include "bnAIState.h"
#include "bnEntity.h"

template<typename T>
class AI {
private:
  AIState<T>* stateMachine;
  Entity* target;
  T* ref;
  int lock;

protected:
  enum StateLock {
    Locked,
    Unlocked
   };


public:
  void Lock() {
    lock = AI<T>::StateLock::Locked;
  }

  void Unlock() {
    lock = AI<T>::StateLock::Unlocked;
  }

  AI(T* _ref) { stateMachine = nullptr; ref = _ref; lock = AI<T>::StateLock::Unlocked; }
  ~AI() { if (stateMachine) { delete stateMachine; ref = nullptr; target = nullptr; } }

  void StateChange(AIState<T>* _state) {
    if (lock == AI<T>::StateLock::Locked) {
      if (_state) {
        delete _state;
      }

      return;
    }

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

  void SetTarget(Entity* _target) {
    target = _target;
  }

  Entity* GetTarget() const { return target;  }
};

