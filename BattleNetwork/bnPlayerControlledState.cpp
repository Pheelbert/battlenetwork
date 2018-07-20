#include "bnPlayerControlledState.h"
#include "bnInputManager.h"
#include "bnPlayer.h"
#include "bnTile.h"
#include "bnAudioResourceManager.h"

#include <iostream>

#define MOVE_KEY_PRESS_COOLDOWN 200.0f
#define MOVE_LAG_COOLDOWN 40.0f
#define ATTACK_KEY_PRESS_COOLDOWN 300.0f
#define ATTACK_TO_IDLE_COOLDOWN 150.0f
#define HIT_COOLDOWN 300.0f

PlayerControlledState::PlayerControlledState() : InputManager(&InputManager::GetInstance()), AIState<Player>()
{
  //Cooldowns. TODO: Take these out. We base actions on animation speed now.
  moveKeyPressCooldown = MOVE_KEY_PRESS_COOLDOWN;
  attackKeyPressCooldown = ATTACK_KEY_PRESS_COOLDOWN;
  attackToIdleCooldown = 0.0f;

  isChargeHeld = false;
}


PlayerControlledState::~PlayerControlledState()
{
  InputManager = nullptr;
}

void PlayerControlledState::OnEnter(Player& player) {
  player.SetAnimation(PLAYER_IDLE);
}

void PlayerControlledState::OnUpdate(float _elapsed, Player& player) {

  // Action controls take priority over movement
  if (InputManager->has(RELEASED_ACTION1)) {
    if (attackKeyPressCooldown >= ATTACK_KEY_PRESS_COOLDOWN && isChargeHeld == true) {
      player.Attack(player.chargeComponent.GetChargeCounter());
      player.chargeComponent.SetCharging(false);
      isChargeHeld = false;
      attackKeyPressCooldown = 0.0f;
      auto onFinish = [&player, this]() {player.SetAnimation(PLAYER_IDLE); this->attackKeyPressCooldown = ATTACK_KEY_PRESS_COOLDOWN; };
      player.SetAnimation(PLAYER_SHOOTING, onFinish);
    }
    else {
      isChargeHeld = false;
    }
  }

  // Movement increments are restricted based on anim speed
  if (player.state != PLAYER_IDLE)
    return;

  Direction direction = Direction::NONE;
  if (moveKeyPressCooldown >= MOVE_KEY_PRESS_COOLDOWN) {
    if (InputManager->has(PRESSED_UP)) {
      direction = Direction::UP;
    }
    else if (InputManager->has(PRESSED_LEFT)) {
      direction = Direction::LEFT;
    }
    else if (InputManager->has(PRESSED_DOWN)) {
      direction = Direction::DOWN;
    }
    else if (InputManager->has(PRESSED_RIGHT)) {
      direction = Direction::RIGHT;
    }
  }
 

  if (InputManager->has(PRESSED_ACTION1) && isChargeHeld == false) {
    player.Attack(player.chargeComponent.GetChargeCounter());
    isChargeHeld = true;

    attackKeyPressCooldown = 0.0f;
    auto onFinish = [&player, this]() { 

      if (isChargeHeld) {
        player.chargeComponent.SetCharging(true);
      }

      player.SetAnimation(PLAYER_IDLE); 
      this->attackKeyPressCooldown = ATTACK_KEY_PRESS_COOLDOWN; 
    };
    player.SetAnimation(PLAYER_SHOOTING, onFinish);
  }

  if (InputManager->has(RELEASED_UP)) {
    direction = Direction::NONE;
  }
  else if (InputManager->has(RELEASED_LEFT)) {
    direction = Direction::NONE;
  }
  else if (InputManager->has(RELEASED_DOWN)) {
    direction = Direction::NONE;
  }
  else if (InputManager->has(RELEASED_RIGHT)) {
    direction = Direction::NONE;
  }

  if (direction != Direction::NONE && player.state != PLAYER_SHOOTING) {
    bool moved = player.Move(direction);

    if (moved) {
      moveKeyPressCooldown = 0.0f;
      auto onFinish = [&player]() {
 
        //Cooldown until player's movement catches up to actual position (avoid walking through spells)
        if (player.previous) {
          if (player.previous->IsCracked()) {
            AUDIO.Play(AudioType::PANEL_CRACK);
            player.previous->SetState(TileState::BROKEN);
          }
          
          player.AdoptNextTile();
        }
        player.SetAnimation(PLAYER_IDLE);
      }; // end lambda
      player.SetAnimation(PLAYER_MOVING, onFinish);
    }
    else {
      player.SetAnimation(PLAYER_IDLE);
    }
    moveKeyPressCooldown = MOVE_KEY_PRESS_COOLDOWN;
  }
}

void PlayerControlledState::OnLeave(Player& player) {
  /* Mega loses charge when we release control */
  player.chargeComponent.SetCharging(false);
}