#pragma once

#include <SFML/Graphics.hpp>
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnEngine.h"

class ChipSelectionCust
{
private:
	sf::Sprite custSprite;

public:
	ChipSelectionCust();
	~ChipSelectionCust();

	bool IsOutOfView();
	bool IsInView();
	void Move(sf::Vector2f delta);
	bool AreChipsReady();
	void Draw();
	// Chips* GetChips();
	// int GetChipCount();
	// Chip* GetNextChip();
};

