#include "bnLongExplosion.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnField.h"

using sf::IntRect;

#define EXPLODE_ANIMATION_SPRITES 16
#define EXPLODE_ANIMATION_WIDTH 59
#define EXPLODE_ANIMATION_HEIGHT 59

LongExplosion::LongExplosion(Field* _field, Team _team)
{
  SetLayer(0);
  field = _field;
  team = _team;

  explosionProgress = 0.0f;
  explosionProgress2 = 0.0f;

  explosion.setTexture(*TEXTURES.GetTexture(TextureType::MOB_EXPLOSION));
  explosion.setScale(2.0f, 2.0f);
  explosion2.setTexture(*TEXTURES.GetTexture(TextureType::MOB_EXPLOSION));
  explosion2.setScale(2.0f, 2.0f);
  int i = 0;
  int y = 0;
  for (int x = 0; x < EXPLODE_ANIMATION_SPRITES; x++) {
    if (x == 8) {
      y += EXPLODE_ANIMATION_HEIGHT;
      i = 0;
    }
    explode.Add(0.1f, IntRect(EXPLODE_ANIMATION_WIDTH*i, y, EXPLODE_ANIMATION_WIDTH, EXPLODE_ANIMATION_HEIGHT));
    i++;
  }
  setScale(2.f, 2.f);

  x1 = 0.f, x2 = 0.f, y1 = 0.f, y2 = 0.f;
}

void LongExplosion::Update(float _elapsed) {
  if (explosionProgress <= 0.0f) {
    AUDIO.Play(AudioType::EXPLODE);
    x1 = tile->getPosition().x - 10.0f;
    y1 = tile->getPosition().y - 35.f;
  }

  if (explosionProgress2 <= 0.0f) {
    x2 = tile->getPosition().x + 10.0f;
    y2 = tile->getPosition().y - 50.0f;
  }

  explosionProgress += 2*_elapsed;
  if (explosionProgress >= 0.3f) {
    explosionProgress2 += 2*_elapsed;
    explosion2.setScale(2.f, 2.f);
    explosion2.setPosition(x2, y2);
    animator(fmin(explosionProgress2, 1.0f), explosion2, explode);
  }

  if (explosionProgress <= 1.f) {
    explosion.setScale(2.f, 2.f);
    explosion.setPosition(x1, y1);
    animator(fmin(explosionProgress, 1.0f), explosion, explode);
  }

  if (explosionProgress2 >= 0.8f) {
    deleted = true;
  }

  Entity::Update(_elapsed);
}

vector<Drawable*> LongExplosion::GetMiscComponents() {
  vector<Drawable*> drawables = vector<Drawable*>();

  if (explosionProgress > 0.0f) {
    drawables.push_back(&explosion);
  }
  if (explosionProgress2 > 0.0f) {
    drawables.push_back(&explosion2);
  }

  return drawables;
}

LongExplosion::~LongExplosion()
{
}