#pragma once
#include "bnMettaurIdleState.h"
#include "bnMettaurMoveState.h"
#include <iostream>

MettaurIdleState::MettaurIdleState() : cooldown(500), AIState<Mettaur>() { ; }
MettaurIdleState::~MettaurIdleState() { ; }

void MettaurIdleState::OnEnter(Mettaur& met) {
  met.SetAnimation(MobState::MOB_IDLE);
}

void MettaurIdleState::OnUpdate(float _elapsed, Mettaur& met) {
  if (!met.IsMettaurTurn())
    return;

  cooldown -= _elapsed;

  if (cooldown < 0) {
    met.StateChange<MettaurMoveState>();
  }
}

void MettaurIdleState::OnLeave(Mettaur& met) {
}

