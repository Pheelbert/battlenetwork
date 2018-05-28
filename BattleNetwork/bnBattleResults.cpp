#include "bnBattleResults.h"
#include "bnTextureResourceManager.h"
#include "bnEngine.h"
#include "bnMob.h"
#include "bnBattleItem.h"

BattleResults::BattleResults(sf::Time battleLength, int moveCount, int hitCount, int counterCount, bool doubleDelete, bool tripleDelete, Mob *mob) {
  /*
  Calculate score and rank
  Calculations are based off http ://megaman.wikia.com/wiki/Virus_Busting

  Delete Time
    0.00~5.00 = 7
    5.01~12.00 = 6
    12.01~36.00 = 5
    36.01 and up = 4

    Boss Delete Time
    0.00~30.00 = 10
    30.01~40.00 = 8
    40.01~50.00 = 6
    50.01 and up = 4

    Number of Hits(received by MegaMan)
    0 = +1
    1 = 0
    2 = -1
    3 = -2
    4 or more = -3

    Movement Number of Steps
    0~2 = +1
    3 or more = +0

    Multiple Deletion
    Double Delete = +2
    Triple Delete = +4

    Counter Hits
    0 = +0
    1 = +1
    2 = +2
    3 = +3
    */
  score = 0;

  if(!mob->IsBoss()) {
    if (battleLength.asSeconds() > 36.1) score += 4;
    else if (battleLength.asSeconds() > 12.01) score += 5;
    else if (battleLength.asSeconds() > 5.01) score += 6;
    else score += 7;
  }
  else {
    if (battleLength.asSeconds() > 50.01) score += 4;
    else if (battleLength.asSeconds() > 40.01) score += 6;
    else if (battleLength.asSeconds() > 30.01) score += 8;
    else score += 10;
  }

  switch (hitCount) {
    case 0: score += 1; break;
    case 1: score += 0; break;
    case 2: score -= 1; break;
    case 3: score -= 2; break;
    default: score -= 3; break;
  }

  if (moveCount >= 0 && moveCount <= 2) {
    score += 1;
  }

  if (doubleDelete) score += 2;
  if (tripleDelete) score += 4;

  std::cout << "score before normalization: " << score << "\n";

  score += std::max(counterCount, 3);

  // No score of zero or below. Min score of 1
  score = std::max(1, score);

  // Get reward based on score
  item = mob->GetRankedReward(score);
 
  isRevealed = false;

  resultsSprite = sf::Sprite(*TEXTURES.GetTexture(TextureType::BATTLE_RESULTS_FRAME));
  resultsSprite.setScale(2.f, 2.f);
  resultsSprite.setPosition(-resultsSprite.getTextureRect().width*2.f, 20.f);

  if (item) {
    sf::IntRect rect = TEXTURES.GetCardRectFromID(item->GetID());

    rewardCard = sf::Sprite(*TEXTURES.GetTexture(TextureType::CHIP_CARDS));
    rewardCard.setTextureRect(rect);
  }
  else {
    rewardCard = sf::Sprite(*TEXTURES.GetTexture(TextureType::CHIP_NODATA));
  }

  rewardCard.setScale(2.f, 2.f);
  rewardCard.setPosition(274.0f, 180.f);

  sf::Font *font = TEXTURES.LoadFontFromFile("resources/fonts/mmbnthick_regular.ttf");
  time.setFont(*font);
  time.setPosition(2.f*192.f, 79.f);
  time.setString(FormatString(battleLength));
  time.setOrigin(time.getLocalBounds().width, 0);
  time.setFillColor(sf::Color(240, 248, 248));

  rank.setFont(*font);
  rank.setPosition(2.f*192.f, 111.f);

  reward.setFont(*font);
  reward.setPosition(2.f*40.f, 209.f);
  reward.setString(item->GetName());
  reward.setFillColor(sf::Color(240, 248, 248));

  if (score > 10) {
    rank.setString("S");
  }
  else {
    rank.setString(std::to_string(score));
  }

  rank.setOrigin(rank.getLocalBounds().width, 0);
  rank.setFillColor(sf::Color(240, 248, 248));
}

BattleResults::~BattleResults() {

}

std::string BattleResults::FormatString(sf::Time time)
{
  double totalMS = time.asMilliseconds();
  std::cout << "totalMS: " << totalMS << "\n";

  int minutes = (int)(totalMS / 1000) / 60;
  std::cout << "minutes: " << minutes << "\n";

  double remainder = ((int)(totalMS / 1000.0) / 60.0) - (double)minutes;
  std::cout << "remainder: " << remainder << "\n";

  int seconds = (int)(remainder * 100) % 60;
  remainder = (remainder * 100.0) - (double)seconds;
  int ms = (int)(remainder * 100) % 100;

  if (minutes > 99) {
    minutes = 99;
  }

  std::string O = "0";
  std::string builder;

  if (minutes < 10) builder += O;
  builder += std::to_string(minutes) + ":";

  if (seconds < 10) builder += O;
  builder += std::to_string(seconds) + ":";

  if (ms < 10) builder += O;
  builder += std::to_string(ms);

  return builder;
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
      ENGINE.Draw(reward, false);
    }
  }
}

// Chip ops
bool BattleResults::IsFinished() {
  return isRevealed;
}

BattleItem* BattleResults::GetReward()
{
  return item;
}
