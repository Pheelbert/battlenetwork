#include "bnPlayer.h"
#include "bnExplodeState.h"
#include "bnField.h"
#include "bnBuster.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnEngine.h"
#include "bnLogger.h"

#define RESOURCE_NAME "megaman"
#define RESOURCE_PATH "resources/navis/megaman/megaman.animation"

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
  chargeComponent(this),
  animationComponent(this),
  AI<Player>(this) 
{
  SetLayer(0);
  team = Team::BLUE;

  moveCount = hitCount = 0;

  //Animation
  animationProgress = 0.0f;
  setScale(2.0f, 2.0f);

  healthUI = new PlayerHealthUI(this);
  chipsUI = new SelectedChipsUI(this);

  //Components setup and load
  chargeComponent.load();

  animationComponent.setup(RESOURCE_NAME, RESOURCE_PATH);
  animationComponent.load();

  textureType = TextureType::NAVI_MEGAMAN_ATLAS;
  setTexture(*TEXTURES.GetTexture(textureType));

  previous = nullptr;

  moveCount = 0;
}

Player::~Player(void) {
  delete chipsUI;
  delete healthUI;
}

void Player::Update(float _elapsed) {
  //Update UI of player's health (top left corner)
  healthUI->Update();

  // Explode if health depleted
  if (GetHealth() <= 0) {
    if (chipsUI) {
      delete chipsUI;
      chipsUI = nullptr;
    }
    this->StateChange<ExplodeState<Player>>();
    this->StateUpdate(_elapsed);
    return;
  }

  this->StateUpdate(_elapsed);

  RefreshTexture();

  //Components updates
  chargeComponent.update(_elapsed);
  animationComponent.update(_elapsed);

  Entity::Update(_elapsed);
}

bool Player::Move(Direction _direction) {
  bool moved = false;
  Tile* temp = tile;
  if (_direction == Direction::UP) {
    if (tile->GetY() - 1 > 0) {
      next = field->GetAt(tile->GetX(), tile->GetY() - 1);
      if (Teammate(next->GetTeam()) && next->IsWalkable()) {
        ;
      } else {
        next = nullptr;
      }
    }
  } else if (_direction == Direction::LEFT) {
    if (tile->GetX() - 1 > 0) {
      next = field->GetAt(tile->GetX() - 1, tile->GetY());
      if (Teammate(next->GetTeam()) && next->IsWalkable()) {
        ;
      } else {
        next = nullptr;
      }
    }
  } else if (_direction == Direction::DOWN) {
    if (tile->GetY() + 1 <= (int)field->GetHeight()) {
      next = field->GetAt(tile->GetX(), tile->GetY() + 1);
      if (Teammate(next->GetTeam()) && next->IsWalkable()) {
        ;
      } else {
        next = nullptr;
      }
    }
  } else if (_direction == Direction::RIGHT) {
    if (tile->GetX() + 1 <= static_cast<int>(field->GetWidth())) {
      next = field->GetAt(tile->GetX() + 1, tile->GetY());
      if (Teammate(next->GetTeam()) && next->IsWalkable()) {
        ;
      } else {
        next = nullptr;
      }
    }
  }

  if (next) {
    previous = temp;
    moved = true;
  }
  return moved;
}

void Player::AdoptNextTile() {
  SetTile(next);
  tile->AddEntity(this);
  previous->RemoveEntity(this);
  previous = nullptr;
  next = nullptr;
  moveCount++;
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

  if (chipsUI) {
    while (chipsUI->GetNextComponent(component)) {
      drawables.push_back(component);
    }
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
  if (this->IsPassthrough()) return false;

  bool result = false;

  if (health - _damage < 0) {
    health = 0;
    result = true;
  } else {
    health -= _damage;
    hitCount++;
    this->StateChange<PlayerHitState, float>({ 600.0f });
  }

  return result;
}

int Player::GetMoveCount() const
{
  return moveCount;
}

int Player::GetHitCount() const
{
  return hitCount;
}

void Player::RefreshTexture() {
  /*switch (state) {
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
  case PlayerState::PLAYER_SLASHING:
    textureType = TextureType::NAVI_MEGAMAN_ATLAS;
    break;
  default:
    assert(false && "Invalid player state.");
  }*/

  if (tile != nullptr) {
    setPosition(tile->getPosition().x + (tile->GetWidth()/2.0f), tile->getPosition().y + (tile->GetHeight()/2.0f));
  }
}

PlayerHealthUI* Player::GetHealthUI() const {
  return healthUI;
}

SelectedChipsUI* Player::GetChipsUI() const {
  return chipsUI;
}

int Player::GetStateFromString(string _string) {
  int size = 7;
  string PLAYER_STATE_STRINGS[] = { "PLAYER_IDLE", "PLAYER_MOVING", "PLAYER_HIT", "PLAYER_SHOOTING", "PLAYER_SLASHING", "PLAYER_HEAL", "PLAYER_CANNON" };
  for (int i = 0; i < size; i++) {
    if (_string == PLAYER_STATE_STRINGS[i]) {
      return static_cast<PlayerState>(i);
    }
  }
  Logger::Logf("Failed to find corresponding enum: %s\n", _string);
  return -1;
}

void Player::SetAnimation(int _state, std::function<void()> onFinish) {
  this->state = static_cast<PlayerState>(_state);
  animationComponent.setAnimation(_state, onFinish);
}
