#pragma once
#include "bnCanodumbIdleState.h"
#include "bnCanodumbAttackState.h"
#include <iostream>

CanodumbAttackState::CanodumbAttackState() : AIState<Canodumb>() { ; }
CanodumbAttackState::~CanodumbAttackState() { ; }

void CanodumbAttackState::OnEnter(Canodumb& can) {
  auto onFinish = [&can]() { can.StateChange<CanodumbIdleState>(); };
  can.SetAnimation(MOB_CANODUMB_SHOOT_1, onFinish);
}

void CanodumbAttackState::OnUpdate(float _elapsed, Canodumb& can) {
  // can.StateChange<CannodumbAttackState>();
}

void CanodumbAttackState::OnLeave(Canodumb& can) {
}

