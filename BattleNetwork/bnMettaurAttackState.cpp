#pragma once
#include "bnMettaurAttackState.h"
#include "bnWave.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnMettaurIdleState.h"

MettaurAttackState::MettaurAttackState() : AIState<Mettaur>() { ; }
MettaurAttackState::~MettaurAttackState() { ; }

void MettaurAttackState::OnEnter(Mettaur& met) {
  auto onFinish = [this, &met]() {this->Attack(met); };
  met.SetAnimation(MobState::MOB_ATTACKING, onFinish);
}

void MettaurAttackState::OnUpdate(float _elapsed, Mettaur& met) {
  /* Nothing, just wait the animation out*/
}

void MettaurAttackState::OnLeave(Mettaur& met) { 
  met.NextMettaurTurn();
}

void MettaurAttackState::Attack(Mettaur& met) {
  Spell* spell = new Wave(met.field, met.team);
  spell->SetDirection(Direction::LEFT);
  met.field->AddEntity(spell, met.tile->GetX() - 1, met.tile->GetY());
  met.StateChange(new MettaurIdleState());
}
