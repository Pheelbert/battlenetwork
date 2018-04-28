#pragma once
#include "bnMeta.h"
#include "bnAIState.h"
#include "bnEntity.h"

template<typename T>
class AI {
private:
  AIState<T>* stateMachine;
  AIState<T>* defaultState;

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

  template<typename U>
  void StateChange() {
    _DerivedFrom<U, AIState<T>>();

    if (lock == AI<T>::StateLock::Locked) {
      return;
    }

    if (stateMachine) {
      stateMachine->OnLeave(*ref);
      delete stateMachine;
    }

    stateMachine = new U();
    stateMachine->OnEnter(*ref);
  }

  /*
    For states that require arguments, pass the argument type in for V 
    e.g. 

    struct ThrowBombStateArgs {
       float throwSpeed;
       int damage;
       bool stun;
    }

    ... some time later ...

    this->StateChange<PlayerThrowBombState, ThrowBombStateArgs>({200.f, 300, true});

    --------------------------------------------------------------------

    For single arguments just pass in primitive for V 
    e.g.

    this->StateChange<PlayerHitState>(20.f);
  */
  template<typename U, typename V>
  void StateChange(V args) {
    _DerivedFrom<U, AIState<T>>();

    if (lock == AI<T>::StateLock::Locked) {
      return;
    }

    if (stateMachine) {
      stateMachine->OnLeave(*ref);
      delete stateMachine;
    }

    stateMachine = new U(args);
    stateMachine->OnEnter(*ref);
  }

  void SetDefaultState(AIState<T>* state) {
    defaultState = state;
  }

  AIState<T>* GetDefaultState() {
    return defaultState;
  }

  void StateChange(AIState<T>* state) {
    if (stateMachine == state) {
      std::cout << "they are the same\n";
    }

    if (!state) {
      std::cout << "state is null\n";
    }

    if (stateMachine) {
      stateMachine->OnLeave(*ref);
      delete stateMachine;
    }

    stateMachine = state;
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

