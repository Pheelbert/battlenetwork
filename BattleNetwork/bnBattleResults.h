#pragma once

#include <SFML/Graphics.hpp>

class BattleResults {
private:
  sf::Sprite resultsSprite;
  sf::Text label;
  sf::Text Rank;

  bool isHidden;
  bool isRevealed;

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

