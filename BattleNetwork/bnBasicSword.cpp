#include <random>
#include <time.h>

#include "bnBasicSword.h"
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

BasicSword::BasicSword(Field* _field, Team _team, int _damage) {
  field = _field;
  team = _team;
  direction = Direction::NONE;
  deleted = false;
  hit = false;
  srand((unsigned int)time(nullptr));
  cooldown = 0;
  damage = _damage;

  EnableTileHighlight(true);
}

BasicSword::~BasicSword(void) {
}

void BasicSword::Update(float _elapsed) {
  if (cooldown >= COOLDOWN || hit) {
    deleted = true;
    Entity::Update(_elapsed);
    return;
  }

  tile->AffectEntities(this);

  cooldown += _elapsed;
}

bool BasicSword::Move(Direction _direction) {
  return false;
}

void BasicSword::Attack(Entity* _entity) {
  Mettaur* isMob = dynamic_cast<Mettaur*>(_entity);
  if (isMob) {
    isMob->Hit(damage);
    hitHeight = isMob->GetHitHeight();
    hit = true;
  }
  else {
    ProgsMan* isProgs = dynamic_cast<ProgsMan*>(_entity);
    if (isProgs) {
      isProgs->Hit(damage);
      hitHeight = isProgs->GetHitHeight();
      hit = true;
    }
  }
}

vector<Drawable*> BasicSword::GetMiscComponents() {
  return vector<Drawable*>();
}

void BasicSword::AddAnimation(int _state, FrameAnimation _animation, float _duration) {
  //animator.addAnimation(static_cast<Buster>(_state), _animation, sf::seconds(_duration));
  assert(false && "Buster does not have an animator");
}