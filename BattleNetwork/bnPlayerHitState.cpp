#include "bnPlayerHitState.h"
#include "bnPlayerControlledState.h"
#include "bnPlayer.h"
#include "bnAudioResourceManager.h"

PlayerHitState::PlayerHitState(float _cooldown) : cooldown(_cooldown), AIState<Player>()
{
}


PlayerHitState::~PlayerHitState()
{
}

void PlayerHitState::OnEnter(Player& player) {
  player.SetAnimation(PlayerState::PLAYER_HIT);
  AudioResourceManager::GetInstance().Play(AudioType::HURT);
}

void PlayerHitState::OnUpdate(float _elapsed, Player& player) {
  cooldown -= _elapsed;

  // Cant do anything if hit/stunned
  if (cooldown < 0) {
    player.StateChange(new PlayerControlledState());
  }
}

void PlayerHitState::OnLeave(Player& player) {
  // Disable previous ailments... e.g. confusion, barrier, soul-cross if damage was bad enough
}