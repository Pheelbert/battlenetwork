#pragma once
#include "bnMettaurMoveState.h"
#include "bnMettaur.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnMettaurAttackState.h"
#include "bnMettaurIdleState.h"

MettaurMoveState::MettaurMoveState() : isMoving(false), AIState<Mettaur>() { ; }
MettaurMoveState::~MettaurMoveState() { ; }

void MettaurMoveState::OnEnter(Mettaur& met) {
}

void MettaurMoveState::OnUpdate(float _elapsed, Mettaur& met) {
  if (isMoving) return; // We're already moving (animations take time)

  Tile* temp = met.tile;
  Tile* next = nullptr;

  Entity* target = met.GetTarget();

  if (target && target->GetTile()) {
    if (target->GetTile()->GetY() < met.GetTile()->GetY()) {
      nextDirection = Direction::UP;
    }
    else if (target->GetTile()->GetY() > met.GetTile()->GetY()) {
      nextDirection = Direction::DOWN;
    }
    else {
      // Just attack
      met.StateChange<MettaurAttackState>();
      return;
    }
  }

  if (nextDirection == Direction::UP) {
    if (met.tile->GetY() - 1 > 0) {
      next = met.field->GetAt(met.tile->GetX(), met.tile->GetY() - 1);
      if (met.Teammate(next->GetTeam()) && next->IsWalkable())
        if (!next->ContainsEntityType<Mettaur>()) {
          met.SetTile(next);
        }
        else {
          next = nullptr;
          nextDirection = Direction::DOWN;
        }
      else
        next = nullptr;
    }
    else {
      nextDirection = Direction::DOWN;
    }
  }
  else if (nextDirection == Direction::LEFT) {
    if (met.tile->GetX() - 1 > 0) {
      next = met.field->GetAt(met.tile->GetX() - 1, met.tile->GetY());
      if (met.Teammate(next->GetTeam()) && next->IsWalkable())
        met.SetTile(next);
      else
        next = nullptr;
    }
  }
  else if (nextDirection == Direction::DOWN) {
    if (met.tile->GetY() + 1 <= (int)met.field->GetHeight()) {
      next = met.field->GetAt(met.tile->GetX(), met.tile->GetY() + 1);
      if (met.Teammate(next->GetTeam()) && next->IsWalkable())
        if (!next->ContainsEntityType<Mettaur>()) {
          met.SetTile(next);
        }
        else {
          next = nullptr;
          nextDirection = Direction::UP;
        }
      else
        next = nullptr;
    }
    else {
      nextDirection = Direction::UP;
    }
  }
  else if (nextDirection == Direction::RIGHT) {
    if (met.tile->GetX() + 1 <= (int)met.field->GetWidth()) {
      next = met.field->GetAt(met.tile->GetX() + 1, met.tile->GetY());
      if (met.Teammate(next->GetTeam()) && next->IsWalkable())
        met.SetTile(next);
      else
        next = nullptr;
    }
  }


  if (next) {
    met.tile->AddEntity((Entity*)&met);
    temp->RemoveEntity((Entity*)&met);
    auto onFinish = [&met]() { met.StateChange<MettaurIdleState>(); };
    met.SetAnimation(MobState::MOB_MOVING, onFinish);
    isMoving = true;
  }
  else {
    // Cannot more or attack. Forfeit turn.
    met.StateChange<MettaurIdleState>();
    met.NextMettaurTurn();
  }
}

void MettaurMoveState::OnLeave(Mettaur& met) {

}

