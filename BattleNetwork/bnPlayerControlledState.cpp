#include "bnPlayerControlledState.h"
#include "bnControllableComponent.h"
#include "bnPlayer.h"
#include "bnTile.h"
#include "bnAudioResourceManager.h"

#include <iostream>

#define MOVE_KEY_PRESS_COOLDOWN 200.0f
#define MOVE_LAG_COOLDOWN 40.0f
#define ATTACK_KEY_PRESS_COOLDOWN 300.0f
#define ATTACK_TO_IDLE_COOLDOWN 150.0f
#define HIT_COOLDOWN 300.0f

PlayerControlledState::PlayerControlledState() : controllableComponent(&ControllableComponent::GetInstance()), AIState<Player>()
{
  //Cooldowns
  moveKeyPressCooldown = 0.0f;
  attackKeyPressCooldown = 0.0f;
  attackToIdleCooldown = 0.0f;
}


PlayerControlledState::~PlayerControlledState()
{
  controllableComponent = nullptr;
}

void PlayerControlledState::OnEnter(Player& player) {
  player.SetAnimation(PlayerState::PLAYER_IDLE);
}

void PlayerControlledState::OnUpdate(float _elapsed, Player& player) {

  // Action controls take priority over movement
  if (controllableComponent->has(RELEASED_ACTION1)) {
    player.Attack(player.chargeComponent.GetChargeCounter());
    player.chargeComponent.SetCharging(false);
    attackToIdleCooldown = 0.0f;

    auto onFinish = [&player]() {player.SetAnimation(PlayerState::PLAYER_IDLE); };
    player.SetAnimation(PlayerState::PLAYER_SHOOTING, onFinish);
  }
  else if (controllableComponent->has(RELEASED_ACTION2)) {
    player.GetChipsUI()->UseNextChip();
  }

  // Movement increments are restricted based on anim speed
  if (player.state == PlayerState::PLAYER_MOVING)
    return;

  moveKeyPressCooldown += _elapsed;
  attackKeyPressCooldown += _elapsed;
  attackToIdleCooldown += _elapsed;

  Direction direction = Direction::NONE;
  if (moveKeyPressCooldown >= MOVE_KEY_PRESS_COOLDOWN) {
    if (controllableComponent->has(PRESSED_UP)) {
      direction = Direction::UP;
    }
    else if (controllableComponent->has(PRESSED_LEFT)) {
      direction = Direction::LEFT;
    }
    else if (controllableComponent->has(PRESSED_DOWN)) {
      direction = Direction::DOWN;
    }
    else if (controllableComponent->has(PRESSED_RIGHT)) {
      direction = Direction::RIGHT;
    }
  }
 

  if (attackKeyPressCooldown >= ATTACK_KEY_PRESS_COOLDOWN) {
    if (controllableComponent->has(PRESSED_ACTION1)) {
      attackKeyPressCooldown = 0.0f;
      player.chargeComponent.SetCharging(true);
    }
  }

  if (controllableComponent->empty()) {
    if (player.state != PlayerState::PLAYER_SHOOTING) {
      player.SetAnimation(PlayerState::PLAYER_IDLE);
    }
  }

  if (controllableComponent->has(RELEASED_UP)) {
    direction = Direction::NONE;
  }
  else if (controllableComponent->has(RELEASED_LEFT)) {
    direction = Direction::NONE;
  }
  else if (controllableComponent->has(RELEASED_DOWN)) {
    direction = Direction::NONE;
  }
  else if (controllableComponent->has(RELEASED_RIGHT)) {
    direction = Direction::NONE;
  }

  if (direction != Direction::NONE && player.state != PlayerState::PLAYER_SHOOTING) {
    bool moved = player.Move(direction);
    if (moved) {
      auto onFinish = [&player]() {
 
        //Cooldown until player's movement catches up to actual position (avoid walking through spells)
        if (player.previous) {
          if (player.previous->IsCracked()) {
            AudioResourceManager::GetInstance().Play(AudioType::PANEL_CRACK);
            player.previous->SetState(TileState::BROKEN);
          }
          
          player.SetTile(player.next);
          player.tile->AddEntity((Entity*)&player);
          player.previous->RemoveEntity((Entity*)&player);
          player.previous = nullptr;
          player.next = nullptr;
        }
        player.SetAnimation(PlayerState::PLAYER_IDLE);
      }; // end lambda
      player.SetAnimation(PlayerState::PLAYER_MOVING, onFinish);
    }
    else {
      player.SetAnimation(PlayerState::PLAYER_IDLE);
    }
    moveKeyPressCooldown = 0.0f;
  }
}

void PlayerControlledState::OnLeave(Player& player) {
  /* Mega loses charge when we release control */
  player.chargeComponent.SetCharging(false);
}