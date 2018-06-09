#include <random>
#include <time.h>

#include "bnBuster.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnProgsMan.h"
#include "bnMettaur.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"

#define COOLDOWN 40.0f
#define DAMAGE_COOLDOWN 50.0f

#define BULLET_ANIMATION_SPRITES 3
#define BULLET_ANIMATION_WIDTH 30
#define BULLET_ANIMATION_HEIGHT 27

Buster::Buster(Field* _field, Team _team, bool _charged) {
  cooldown = 0;
  damageCooldown = 0;
  field = _field;
  team = _team;
  direction = Direction::NONE;
  deleted = false;
  hit = false;
  progress = 0.0f;
  hitHeight = 0.0f;
  srand((unsigned int)time(nullptr));
  random = rand() % 20 - 20;
  if (_charged) {
    damage = 10;
    //TODO: make new sprite animation for charged bullet
    texture = TEXTURES.GetTexture(TextureType::SPELL_BULLET_HIT);
  } else {
    damage = 1;
    texture = TEXTURES.GetTexture(TextureType::SPELL_BULLET_HIT);
  }
  setScale(2.f, 2.f);
  for (int x = 0; x < BULLET_ANIMATION_SPRITES; x++) {
    animation.addFrame(0.3f, IntRect(BULLET_ANIMATION_WIDTH*x, 0, BULLET_ANIMATION_WIDTH, BULLET_ANIMATION_HEIGHT));
  }
}

Buster::~Buster(void) {
}

void Buster::Update(float _elapsed) {
  if (hit) {
    if (progress == 0.0f) {
      setTexture(*texture);
      setPosition(tile->getPosition().x + tile->GetWidth() / 2.f + random, tile->getPosition().y + tile->GetHeight() / 2.f - hitHeight);
    }
    progress += 0.2f;
    animation(*this, fmin(progress, 1.0f));
    if (progress >= 1.f) {
      deleted = true;
    }
    return;
  }

  // NOTE: This probably had a cool down because the original dev wanted it to do damage at the end of the buster frame
  // Doing this made direct shots miss. Spawning the bullet should happen at the end frame in the player class. 
  tile->AffectEntities(this);

  cooldown += _elapsed;
  if (cooldown >= COOLDOWN) {
    Move(direction);
    cooldown = 0;
  }
}

bool Buster::Move(Direction _direction) {
  tile->RemoveEntity(this);
  Tile* next = nullptr;
  if (_direction == Direction::UP) {
    if (tile->GetY() - 1 > 0) {
      next = field->GetAt(tile->GetX(), tile->GetY() - 1);
      SetTile(next);
    }
  } else if (_direction == Direction::LEFT) {
    if (tile->GetX() - 1 > 0) {
      next = field->GetAt(tile->GetX() - 1, tile->GetY());
      SetTile(next);
    } else {
      deleted = true;
      return false;
    }
  } else if (_direction == Direction::DOWN) {
    if (tile->GetY() + 1 <= (int)field->GetHeight()) {
      next = field->GetAt(tile->GetX(), tile->GetY() + 1);
      SetTile(next);
    }
  } else if (_direction == Direction::RIGHT) {
    if (tile->GetX() < (int)field->GetWidth()) {
      next = field->GetAt(tile->GetX() + 1, tile->GetY());
      SetTile(next);
    } else {
      deleted = true;
      return false;
    }
  }
  tile->AddEntity(this);
  return true;
}

void Buster::Attack(Entity* _entity) {
  if (deleted) return;

  Mettaur* isMob = dynamic_cast<Mettaur*>(_entity);
  if (isMob) {
    isMob->Hit(damage);
    hitHeight = isMob->GetHitHeight();
    hit = true;
    deleted = true;
  } else {
    ProgsMan* isProgs = dynamic_cast<ProgsMan*>(_entity);
    if (isProgs) {
      isProgs->Hit(damage);
      hitHeight = isProgs->GetHitHeight();
      hit = true;
      deleted = true;
    }
  }

  if (hit) {
    AUDIO.Play(AudioType::HURT, 1);
  }
}

vector<Drawable*> Buster::GetMiscComponents() {
  return vector<Drawable*>();
}

void Buster::AddAnimation(int _state, FrameAnimation _animation, float _duration) {
  //animator.addAnimation(static_cast<Buster>(_state), _animation, sf::seconds(_duration));
  assert(false && "Buster does not have an animator");
}