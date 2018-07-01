#pragma once
#include "bnCanodumbIdleState.h"
#include "bnCanodumbAttackState.h"
#include <iostream>

CanodumbIdleState::CanodumbIdleState() : AIState<Canodumb>() { ; }
CanodumbIdleState::~CanodumbIdleState() { ; }

void CanodumbIdleState::OnEnter(Canodumb& can) {
  can.SetAnimation(MOB_CANODUMB_IDLE_1);
}

void CanodumbIdleState::OnUpdate(float _elapsed, Canodumb& can) {
  can.StateChange<CanodumbAttackState>();
}

void CanodumbIdleState::OnLeave(Canodumb& can) {
}

