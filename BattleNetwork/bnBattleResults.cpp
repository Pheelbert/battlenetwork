#include "bnBattleResults.h"
#include "bnTextureResourceManager.h"
#include "bnEngine.h"

std::string BattleResults::FormatString(sf::Time time)
{
  double totalMS = time.asMilliseconds();
  std::cout << "totalMS: " << totalMS << "\n";

  int minutes = totalMS / 1000 / 60;
  std::cout << "minutes: " << minutes << "\n";

  double remainder = (totalMS / 1000.0 / 60.0) - (double)minutes;
  std::cout << "remainder: " << remainder << "\n";

  int seconds = remainder * 100;
  remainder = (remainder * 100.0) - (double)seconds;
  int ms = remainder * 100;

  if (minutes > 99) {
    minutes = 99;
  }

  std::string O = "0";
  std::string builder;

  if (minutes < 10) builder += O;
  builder += std::to_string(minutes) + "/";

  if (seconds < 10) builder += O;
  builder += std::to_string(seconds) + "/";

  if (ms < 10) builder += O;
  builder += std::to_string(ms);

  return builder;
}

BattleResults::BattleResults(sf::Time battleLength) {
  isRevealed = false;

  resultsSprite = sf::Sprite(*TEXTURES.GetTexture(TextureType::BATTLE_RESULTS_FRAME));
  resultsSprite.setScale(2.f, 2.f);
  resultsSprite.setPosition(-resultsSprite.getTextureRect().width*2.f, 20.f);

  // TODO: replace with RewardItem class type to store and handle data 
  sf::IntRect random = TEXTURES.GetCardRectFromID(rand() % (17 * 11));

  rewardCard = sf::Sprite(*TEXTURES.GetTexture(TextureType::CHIP_CARDS));;
  rewardCard.setTextureRect(random);
  rewardCard.setScale(2.f, 2.f);
  rewardCard.setPosition(275.0f, 180.f);

  sf::Font *font = TEXTURES.LoadFontFromFile("resources/fonts/mmbnthick_regular.ttf");
  time.setFont(*font);
  time.setPosition(2.f*192.f, 79.f);
  time.setString(FormatString(battleLength));
  time.setOrigin(time.getLocalBounds().width, 0);
  time.setFillColor(sf::Color(240, 248, 248));

  rank.setFont(*font);
  rank.setPosition(2.f*192.f, 111.f);
  rank.setString("S");
  rank.setOrigin(rank.getLocalBounds().width, 0);
  rank.setFillColor(sf::Color(240, 248, 248));
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

  if (IsInView()) {
    // Draw shadow
    rank.setPosition(2.f*192.f, 112.f);
    rank.setFillColor(sf::Color(80, 72, 88));
    ENGINE.Draw(rank, false);

    // Draw overlay
    rank.setPosition(2.f*191.f, 110.f);
    rank.setFillColor(sf::Color(240, 248, 248));
    ENGINE.Draw(rank, false);

    // Draw shadow
    time.setPosition(2.f*192.f, 80.f);
    time.setFillColor(sf::Color(80, 72, 88));
    ENGINE.Draw(time, false);

    // Draw overlay
    time.setPosition(2.f*191.f, 78.f);
    time.setFillColor(sf::Color(240, 248, 248));
    ENGINE.Draw(time, false);

    if (isRevealed) {
      ENGINE.Draw(rewardCard, false);
    }
  }
}

// Chip ops
bool BattleResults::IsFinished() {
  return isRevealed;
}