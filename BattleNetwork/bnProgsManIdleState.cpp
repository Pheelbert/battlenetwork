#include "bnProgsManIdleState.h"
#include "bnProgsManMoveState.h"
#include "bnProgsMan.h"


ProgsManIdleState::ProgsManIdleState() : cooldown(500.f), AIState<ProgsMan>()
{
}


ProgsManIdleState::~ProgsManIdleState()
{
}

void ProgsManIdleState::OnEnter(ProgsMan& progs) {
  progs.SetAnimation(MobState::MOB_IDLE);
}

void ProgsManIdleState::OnUpdate(float _elapsed, ProgsMan& progs) {
  cooldown -= _elapsed;

  if (cooldown < 0) {
    progs.StateChange<ProgsManMoveState>();
  }
}

void ProgsManIdleState::OnLeave(ProgsMan& progs) {}