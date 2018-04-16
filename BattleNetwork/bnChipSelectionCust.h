#pragma once

#include <SFML/Graphics.hpp>
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnChipLibrary.h"
#include "bnEngine.h"

class ChipSelectionCust {
public:
  struct Bucket {
    Chip* data;
    short state;
  };

private:
  sf::Sprite custSprite;
  sf::Sprite cursorSmall; // TODO: make these two animate
  sf::Sprite cursorBig;
  LayeredDrawable icon;
  LayeredDrawable chipCard;
  sf::Shader greyscale;
  sf::Text label;
  int chipCount;
  int selectCount;
  int chipCap;
  int cursorPos;
  bool areChipsReady;
  Chip** selectedChips;
  Bucket* queue;
  Bucket** selectQueue;
public:
  ChipSelectionCust(int);
  ~ChipSelectionCust();

  // GUI ops
  void CursorRight();
  void CursorLeft();
  void CursorAction();
  void CursorCancel();

  bool IsOutOfView();
  bool IsInView();
  void Move(sf::Vector2f delta);
  void Draw();

  // Chip ops
  void GetNextChips();
  Chip** GetChips();
  void ClearChips();
  const int GetChipCount();
  void ResetState();
  bool AreChipsReady();
};

