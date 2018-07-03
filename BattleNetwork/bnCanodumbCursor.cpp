#include "bnCanodumbCursor.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnField.h"

using sf::IntRect;

#define MAX_COOLDOWN_1 500.0f
#define MAX_COOLDOWN_2 250.0f
#define MAX_COOLDOWN_3 175.0f

#define RESOURCE_PATH "resources/mobs/canodumb/canodumb.animation"

CanodumbCursor::CanodumbCursor(Field* _field, Team _team, Canodumb* _parent) : animationComponent(this) {
  parent = _parent;
  target = parent->GetTarget();

  SetLayer(0);
  field = _field;
  team = _team;

  direction = Direction::LEFT;

  setTexture(*TEXTURES.GetTexture(TextureType::MOB_CANODUMB_ATLAS));
  setScale(2.f, 2.f);

  //Components setup and load
  animationComponent.Setup(RESOURCE_PATH);
  animationComponent.Load();
  animationComponent.SetAnimation(MOB_CANODUMB_CURSOR);
  animationComponent.Update(0);

  switch (parent->GetRank()) {
  case Canodumb::Rank::_1:
    maxcooldown = MAX_COOLDOWN_1;
    break;
  case Canodumb::Rank::_2:
    maxcooldown = MAX_COOLDOWN_2;
    break;
  case Canodumb::Rank::_3:
    maxcooldown = MAX_COOLDOWN_3;
    break;
  }

  movecooldown = maxcooldown;
}

void CanodumbCursor::Update(float _elapsed) {
  animationComponent.Update(_elapsed);

  setPosition(tile->getPosition().x + tile->GetWidth() / 2.0f, tile->getPosition().y + tile->GetHeight() / 2.0f - 15.0f);

  movecooldown -= _elapsed;

  if (movecooldown <= 0) {
    if (this->GetTile() == target->GetTile()) {
      deleted = true;

      parent->StateChange<CanodumbAttackState>();
    }
    else {
      movecooldown = maxcooldown;

      Field* f = this->GetField();
      Tile* t = f->GetAt(this->GetTile()->GetX() - 1, this->GetTile()->GetY());

      if (t != nullptr) {
        this->GetTile()->RemoveEntity(this);
        t->AddEntity(this);
      }
      else {
        deleted = true;
      }
    }
  }

  Entity::Update(_elapsed);
}

vector<Drawable*> CanodumbCursor::GetMiscComponents() {
  vector<Drawable*> drawables = vector<Drawable*>();
  return drawables;
}

CanodumbCursor::~CanodumbCursor()
{
}

int CanodumbCursor::GetStateFromString(string _string) {
  assert(false && "LongExplosion does not have states");
  return 0;
}

void CanodumbCursor::AddAnimation(int _state, FrameAnimation _animation, float _duration) {
  assert(false && "LongExplosion does not have an animator");
}