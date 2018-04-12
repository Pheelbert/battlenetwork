#pragma once

#include <SFML/Graphics.hpp>
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnChipLibrary.h"
#include "bnEngine.h"

class ChipSelectionCust
{
private:
	sf::Sprite custSprite;
	int chipCount;
	Chip** selectedChips;
public:
	ChipSelectionCust();
	~ChipSelectionCust();

	bool IsOutOfView();
	bool IsInView();
	void Move(sf::Vector2f delta);
	void GetNextChips();
	void Draw();
	Chip** GetChips();
	void ClearChips();
	const int GetChipCount();
};

