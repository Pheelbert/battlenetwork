#pragma once
#include "bnCanodumbIdleState.h"
#include "bnCanodumbAttackState.h"
#include <iostream>

CanodumbAttackState::CanodumbAttackState() : AIState<Canodumb>() { ; }
CanodumbAttackState::~CanodumbAttackState() { ; }

void CanodumbAttackState::OnEnter(Canodumb& can) {
  auto onFinish = [&can]() { can.StateChange<CanodumbIdleState>(); };

  switch (can.GetRank()) {
  case Canodumb::Rank::_1:
    can.SetAnimation(MOB_CANODUMB_SHOOT_1, onFinish);
    break;
  case Canodumb::Rank::_2:
    can.SetAnimation(MOB_CANODUMB_SHOOT_2, onFinish);
    break;
  case Canodumb::Rank::_3:
    can.SetAnimation(MOB_CANODUMB_SHOOT_3, onFinish);
    break;
  }
}

void CanodumbAttackState::OnUpdate(float _elapsed, Canodumb& can) {
}

void CanodumbAttackState::OnLeave(Canodumb& can) {
}

