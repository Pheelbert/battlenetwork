#include "bnPlayer.h"
#include "bnField.h"
#include "bnBuster.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnEngine.h"
#include "bnLogger.h"

#define RESOURCE_NAME "megaman"
#define RESOURCE_PATH "resources/navis/megaman/megaman.animation"

#define MOVE_KEY_PRESS_COOLDOWN 200.0f
#define MOVE_LAG_COOLDOWN 40.0f
#define ATTACK_KEY_PRESS_COOLDOWN 300.0f
#define ATTACK_TO_IDLE_COOLDOWN 150.0f
#define HIT_COOLDOWN 300.0f

#define MOVE_ANIMATION_SPRITES 4
#define MOVE_ANIMATION_WIDTH 38
#define MOVE_ANIMATION_HEIGHT 58

#define SHOOT_ANIMATION_SPRITES 5
#define SHOOT_ANIMATION_WIDTH 75
#define SHOOT_ANIMATION_HEIGHT 58

Player::Player(void)
  : health(99),
  state(PlayerState::PLAYER_IDLE),
  textureType(TextureType::NAVI_MEGAMAN_MOVE),
  controllableComponent(&ControllableComponent::GetInstance()),
  chargeComponent(ChargeComponent(this)),
  animationComponent(AnimationComponent(this)) {
  SetLayer(0);
  team = Team::BLUE;

  //Cooldowns
  moveKeyPressCooldown = 0.0f;
  moveLagCooldown = 0.0f;
  attackKeyPressCooldown = 0.0f;
  attackToIdleCooldown = 0.0f;
  hitCoolDown = 0.0f;

  //Animation
  animationProgress = 0.0f;
  setScale(2.0f, 2.0f);

  healthUI = new PlayerHealthUI(this);
  chipsUI = new SelectedChipsUI(this);

  //Components setup and load
  chargeComponent.load();
  animationComponent.setup(RESOURCE_NAME, RESOURCE_PATH);
  animationComponent.load();

  this->SetAnimation(PlayerState::PLAYER_IDLE);
}

Player::~Player(void) {
  controllableComponent = nullptr;
  delete chipsUI;
  delete healthUI;
}

void Player::Update(float _elapsed) {

  moveKeyPressCooldown += _elapsed;
  moveLagCooldown += _elapsed;
  attackKeyPressCooldown += _elapsed;
  attackToIdleCooldown += _elapsed;
  hitCoolDown += _elapsed;

  //Update UI of player's health (top left corner)
  healthUI->Update();

  // Update chip UI
  // chipsUI->Update(); TODO: Forced z-ordering for some UI?

  // Cant do anything if hit/stunned 
  if (hitCoolDown < HIT_COOLDOWN) {
    RefreshTexture();
    return;
  } else {
    // we're just now leaving the hit state
    if (state == PlayerState::PLAYER_HIT) {
      state = PlayerState::PLAYER_IDLE;
    }
  }

  //Cooldown until player's movement catches up to actual position (avoid walking through spells)
  if (moveLagCooldown >= MOVE_LAG_COOLDOWN) {
    if (previous) {
      if (previous->IsCracked()) {
        AudioResourceManager::GetInstance().Play(AudioType::PANEL_CRACK);
        previous->SetState(TileState::BROKEN);
      }

      previous->RemoveEntity(this);
      previous = nullptr;
    }
  }

  Direction direction = Direction::NONE;
  if (moveKeyPressCooldown >= MOVE_KEY_PRESS_COOLDOWN) {
    if (controllableComponent->has(PRESSED_UP)) {
      direction = Direction::UP;
    } else if (controllableComponent->has(PRESSED_LEFT)) {
      direction = Direction::LEFT;
    } else if (controllableComponent->has(PRESSED_DOWN)) {
      direction = Direction::DOWN;
    } else if (controllableComponent->has(PRESSED_RIGHT)) {
      direction = Direction::RIGHT;
    }
  } else {
    state = PlayerState::PLAYER_IDLE;
  }

  if (attackKeyPressCooldown >= ATTACK_KEY_PRESS_COOLDOWN) {
    if (controllableComponent->has(PRESSED_ACTION1)) {
      attackKeyPressCooldown = 0.0f;
      chargeComponent.SetCharging(true);
    }
  }

  if (controllableComponent->empty()) {
    if (state != PlayerState::PLAYER_SHOOTING) {
      state = PlayerState::PLAYER_IDLE;
    }
  }

  if (controllableComponent->has(RELEASED_UP)) {
    direction = Direction::NONE;
  } else if (controllableComponent->has(RELEASED_LEFT)) {
    direction = Direction::NONE;
  } else if (controllableComponent->has(RELEASED_DOWN)) {
    direction = Direction::NONE;
  } else if (controllableComponent->has(RELEASED_RIGHT)) {
    direction = Direction::NONE;
  } else if (controllableComponent->has(RELEASED_ACTION1)) {
    Attack(chargeComponent.GetChargeCounter());
    chargeComponent.SetCharging(false);
    attackToIdleCooldown = 0.0f;
    state = PlayerState::PLAYER_SHOOTING;
  } else if (controllableComponent->has(RELEASED_ACTION2)) {
    chipsUI->UseNextChip();
  }

  if (direction != Direction::NONE && state != PlayerState::PLAYER_SHOOTING) {
    bool moved = Move(direction);
    if (moved) {
      state = PlayerState::PLAYER_MOVING;
    } else {
      state = PlayerState::PLAYER_IDLE;
    }
    moveKeyPressCooldown = 0.0f;
    moveLagCooldown = 0.0f;
  }

  if (attackToIdleCooldown >= ATTACK_TO_IDLE_COOLDOWN) {
    if (state == PlayerState::PLAYER_SHOOTING) {
      state = PlayerState::PLAYER_IDLE;
    }
  }

  RefreshTexture();

  //Components updates
  chargeComponent.update(_elapsed);
  animationComponent.update(_elapsed);
}

bool Player::Move(Direction _direction) {
  bool moved = false;
  Tile* temp = tile;
  Tile* next = nullptr;
  if (_direction == Direction::UP) {
    if (tile->GetY() - 1 > 0) {
      next = field->GetAt(tile->GetX(), tile->GetY() - 1);
      if (Teammate(next->GetTeam()) && next->IsWalkable()) {
        SetTile(next);
      } else {
        next = nullptr;
      }
    }
  } else if (_direction == Direction::LEFT) {
    if (tile->GetX() - 1 > 0) {
      next = field->GetAt(tile->GetX() - 1, tile->GetY());
      if (Teammate(next->GetTeam()) && next->IsWalkable()) {
        SetTile(next);
      } else {
        next = nullptr;
      }
    }
  } else if (_direction == Direction::DOWN) {
    if (tile->GetY() + 1 <= (int)field->GetHeight()) {
      next = field->GetAt(tile->GetX(), tile->GetY() + 1);
      if (Teammate(next->GetTeam()) && next->IsWalkable()) {
        SetTile(next);
      } else {
        next = nullptr;
      }
    }
  } else if (_direction == Direction::RIGHT) {
    if (tile->GetX() + 1 <= static_cast<int>(field->GetWidth())) {
      next = field->GetAt(tile->GetX() + 1, tile->GetY());
      if (Teammate(next->GetTeam()) && next->IsWalkable()) {
        SetTile(next);
      } else {
        next = nullptr;
      }
    }
  }

  if (next) {
    previous = temp;
    moved = true;
  }

  tile->AddEntity(this);
  return moved;
}

void Player::Attack(float _charge) {
  if (tile->GetX() <= static_cast<int>(field->GetWidth())) {
    Spell* spell = new Buster(field, team, chargeComponent.IsFullyCharged());
    spell->SetDirection(Direction::RIGHT);
    field->AddEntity(spell, tile->GetX(), tile->GetY());
  }
}

vector<Drawable*> Player::GetMiscComponents() {
  vector<Drawable*> drawables = vector<Drawable*>();
  Drawable* component;
  while (healthUI->GetNextComponent(component)) {
    drawables.push_back(component);
  }
  drawables.push_back(&chargeComponent.GetSprite());

  while (chipsUI->GetNextComponent(component)) {
    drawables.push_back(component);
  }
  return drawables;
}

void Player::SetHealth(int _health) {
  health = _health;
}

int Player::GetHealth() const {
  return health;
}

int Player::Hit(int _damage) {
  hitCoolDown = 0.0f; // start the timer 
  state = PlayerState::PLAYER_HIT;
  AudioResourceManager::GetInstance().Play(AudioType::HURT);

  bool result = false;

  if (health - _damage < 0) {
    health = 0; deleted = true;
    result = true;
  } else {
    health -= _damage;
  }

  return result;
}

void Player::RefreshTexture() {
  switch (state) {
  case PlayerState::PLAYER_IDLE:
    textureType = TextureType::NAVI_MEGAMAN_MOVE;
    break;
  case PlayerState::PLAYER_MOVING:
    textureType = TextureType::NAVI_MEGAMAN_MOVE;
    break;
  case PlayerState::PLAYER_SHOOTING:
    textureType = TextureType::NAVI_MEGAMAN_SHOOT;
    break;
  case PlayerState::PLAYER_HIT:
    textureType = TextureType::NAVI_MEGAMAN_HIT;
    break;
  default:
    assert(false && "Invalid player state.");
  }

  setTexture(*TextureResourceManager::GetInstance().GetTexture(textureType));

  if (tile != nullptr) {
    setPosition(tile->getPosition().x + 2.f, tile->getPosition().y - 76.f);
  }

  animationComponent.setAnimation(state);

}

PlayerHealthUI* Player::GetHealthUI() const {
  return healthUI;
}

SelectedChipsUI* Player::GetChipsUI() const {
  return chipsUI;
}

int Player::GetStateFromString(string _string) {
  int size = 4;
  string PLAYER_STATE_STRINGS[] = { "PLAYER_IDLE", "PLAYER_MOVING", "PLAYER_HIT", "PLAYER_SHOOTING" };
  for (int i = 0; i < size; i++) {
    if (_string == PLAYER_STATE_STRINGS[i]) {
      return static_cast<PlayerState>(i);
    }
  }
  Logger::Log(string("Failed to find corresponding enum: " + _string));
  assert(false);
  return -1;
}

void Player::SetAnimation(int _state)
{
  this->state = static_cast<PlayerState>(_state);
  animationComponent.setAnimation(_state);
}
