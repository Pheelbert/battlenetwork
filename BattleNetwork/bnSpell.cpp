#include <random>
#include <time.h>

#include "bnSpell.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnTextureResourceManager.h"

#define COOLDOWN 40.0f
#define DAMAGE_COOLDOWN 50.0f

#define BULLET_ANIMATION_SPRITES 3
#define BULLET_ANIMATION_WIDTH 30
#define BULLET_ANIMATION_HEIGHT 27

Spell::Spell(void) {
  SetFloatShoe(true);
  SetLayer(1);
  hit = false;
  srand((unsigned int)time(nullptr));
  random = rand() % 20 - 20;
  cooldown = 0.0f;
  damageCooldown = 0.0f;
  progress = 0.0f;
  hitHeight = 0.0f;
  direction = Direction::NONE;
  texture = nullptr;
  bool markTile = false;
}

Spell::~Spell(void) {
}

const bool Spell::IsTileHighlightEnabled() const {
  return markTile;
}

void Spell::SetDirection(Direction _direction) {
  direction = _direction;
}

Direction Spell::GetDirection() const {
  return direction;
}

void Spell::EnableTileHighlight(bool enable)
{
  markTile = true;
}
