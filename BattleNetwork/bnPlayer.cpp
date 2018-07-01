#include "bnPlayer.h"
#include "bnExplodeState.h"
#include "bnField.h"
#include "bnBuster.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnEngine.h"
#include "bnLogger.h"

#define RESOURCE_NAME "megaman"
#define RESOURCE_PATH "resources/navis/starman/starman.animation"

#define MOVE_ANIMATION_SPRITES 4
#define MOVE_ANIMATION_WIDTH 38
#define MOVE_ANIMATION_HEIGHT 58

#define SHOOT_ANIMATION_SPRITES 5
#define SHOOT_ANIMATION_WIDTH 75
#define SHOOT_ANIMATION_HEIGHT 58

Player::Player(void)
  : health(200),
  state(PLAYER_IDLE),
  chargeComponent(this),
  animationComponent(this),
  AI<Player>(this) 
{
  name = "Megaman";
  SetLayer(0);
  team = Team::RED;

  moveCount = hitCount = 0;

  //Animation
  animationProgress = 0.0f;
  setScale(2.0f, 2.0f);

  healthUI = new PlayerHealthUI(this);
  chipsUI = new SelectedChipsUI(this);

  //Components setup and load
  chargeComponent.load();

  animationComponent.Setup(RESOURCE_PATH);
  animationComponent.Load();

  textureType = TextureType::NAVI_STARMAN_ATLAS;
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
  animationComponent.Update(_elapsed);

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

const bool Player::Hit(int _damage) {
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

void Player::SetAnimation(string _state, std::function<void()> onFinish) {
  state = _state;

  if (state == PLAYER_IDLE) {
    int playback = Animate::Mode::Loop;
    animationComponent.SetAnimation(_state, Animate::Mode(playback), onFinish);
  }
  else {
    animationComponent.SetAnimation(_state, onFinish);
  }
}
