#include <random>
#include <time.h>

#include "bnRollHeal.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"

#define RESOURCE_PATH "resources/spells/spell_roll.animation"

RollHeal::RollHeal(Field* _field, Battle::Tile* _tile, Team _team, int _heal) : Spell()
{
  SetPassthrough(true);
  EnableTileHighlight(false); // Do not highlight where we move

  field = _field;
  team = _team;

  direction = Direction::NONE;
  deleted = false;
  hit = false;
  progress = 0.0f;
  hitHeight = 0.0f;
  srand((unsigned int)time(nullptr));
  random = rand() % 20 - 20;

  heal = _heal;

  setScale(2.0f, 2.0f);

  this->field->AddEntity(this, _tile->GetX(), _tile->GetY());

  setTexture(*TEXTURES.LoadTextureFromFile("resources/spells/spell_roll.png"), true);
  animationComponent.Setup(RESOURCE_PATH);
  animationComponent.Load();
  animationComponent.SetAnimation("ROLL_IDLE", [this] { 
    AUDIO.Play(AudioType::APPEAR);

    this->animationComponent.SetAnimation("ROLL_MOVE", [this] {

      bool found = false;

      Battle::Tile* next = nullptr;
      Battle::Tile* attack = nullptr;
      while(field->GetNextTile(next)) {
        if (!found) {
          if (next->ContainsEntityType<Character>() && next->GetTeam() != this->GetTeam()) {
            this->GetTile()->RemoveEntity(this);

            Battle::Tile* prev = field->GetAt(next->GetX() - 1, next->GetY());
            this->SetTile(prev);
            prev->AddEntity(this);

            attack = next;

            found = true;
          }
        }
      }

      if (found) {
        this->animationComponent.SetAnimation("ROLL_ATTACKING", [this] {
          this->animationComponent.SetAnimation("ROLL_MOVE", [this] {
            this->GetTile()->RemoveEntity(this);
          });
        });

        if (attack) {
          this->animationComponent.AddCallback(4, [this, attack]() { attack->AffectEntities(this); });
          this->animationComponent.AddCallback(12, [this, attack]() { attack->AffectEntities(this); });
          this->animationComponent.AddCallback(20, [this, attack]() { attack->AffectEntities(this); });
        }
      }
      else {
        this->animationComponent.SetAnimation("ROLL_MOVE", [this] {
          this->GetTile()->RemoveEntity(this);
        });
      }
    });
  });

  this->Update(0);
}

RollHeal::~RollHeal(void) {
}

void RollHeal::Update(float _elapsed) {
  animationComponent.Update(_elapsed);

  if (tile != nullptr) {
    setPosition(tile->getPosition().x + (tile->GetWidth() / 2.0f), tile->getPosition().y + (tile->GetHeight() / 2.0f));
  }

  Entity::Update(_elapsed);
}

bool RollHeal::Move(Direction _direction) {
  return true;
}

void RollHeal::Attack(Entity* _entity) {
  if (hit || deleted) {
    return;
  }

  if (_entity && _entity->GetTeam() != this->GetTeam()) {
    if (!_entity->IsPassthrough()) {
      _entity->Hit(10);
      _entity->Update(1);
      hitHeight = _entity->GetHitHeight();
      AUDIO.Play(AudioType::HURT);
    }
  }
}

vector<Drawable*> RollHeal::GetMiscComponents() {
  return vector<Drawable*>();
}
