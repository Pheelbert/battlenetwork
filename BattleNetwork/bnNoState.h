#pragma once
#include "bnMeta.h"
#include "bnEntity.h"
#include "bnAIState.h"

/*
This state can be used by any Entity in the engine.
It uses constraints to ensure the type passed in Any
is a subclass of Entity.

This state exists to keep the AIState non-null and
minimize run-time errors. This state exists as filler
for when no state is supplied for an AI. This practice
is not recomended.
*/
template<typename Any>
class NoState : public AIState<Any>
{
public:
  NoState();
  ~NoState();

  void OnEnter(Any& e);
  void OnUpdate(float _elapsed, Any& e);
  void OnLeave(Any& e);
};

template<typename Any>
NoState<Any>::NoState() : AIState<Any>() {
  // Enforce template constraints on class
  _DerivedFrom<Any, Entity>();
}

template<typename Any>
NoState<Any>::~NoState() { }

template<typename Any>
void NoState<Any>::OnEnter(Any& e) { }

template<typename Any>
void NoState<Any>::OnUpdate(float _elapsed, Any& e) { }

template<typename Any>
void NoState<Any>::OnLeave(Any& e) { }