#pragma once

#include <SFML/Graphics.hpp>

class BattleResults {
private:
  sf::Sprite resultsSprite;
  sf::Text time;
  sf::Text rank;
  sf::Text reward;
  sf::Sprite rewardCard;

  bool isHidden;
  bool isRevealed;

  std::string FormatString(sf::Time time);

public:
  BattleResults(sf::Time battleLength);
  ~BattleResults();

  // GUI ops
  bool CursorAction();

  bool IsOutOfView();
  bool IsInView();
  void Move(sf::Vector2f delta);
  void Draw();

  // Chip ops
  bool IsFinished();
};

