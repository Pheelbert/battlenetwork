#include "bnBattleResults.h"
#include "bnTextureResourceManager.h"
#include "bnEngine.h"

BattleResults::BattleResults(sf::Time battleLength) {
  resultsSprite = sf::Sprite(*TEXTURES.GetTexture(TextureType::BATTLE_RESULTS_FRAME));
  resultsSprite.setScale(2.f, 2.f);
  resultsSprite.setPosition(-resultsSprite.getTextureRect().width*2.f, 20.f);
}

BattleResults::~BattleResults() {

}

// GUI ops
bool BattleResults::CursorAction() {
  bool prevStatus = isRevealed;

  isRevealed = true;

  return prevStatus;
}

bool BattleResults::IsOutOfView() {
  float bounds = -resultsSprite.getTextureRect().width*2.f;

  if (resultsSprite.getPosition().x <= bounds) {
    resultsSprite.setPosition(bounds, resultsSprite.getPosition().y);
  }

  return (resultsSprite.getPosition().x == bounds);
}

bool BattleResults::IsInView() {
  float bounds = 50.f;

  if (resultsSprite.getPosition().x >= bounds) {
    resultsSprite.setPosition(bounds, resultsSprite.getPosition().y);
  }

  return (resultsSprite.getPosition().x == bounds);
}

void BattleResults::Move(sf::Vector2f delta) {
  resultsSprite.setPosition(resultsSprite.getPosition() + delta);
}

void BattleResults::Draw() {
  ENGINE.Draw(resultsSprite, false);
}

// Chip ops
bool BattleResults::IsFinished() {
  return isRevealed;
}