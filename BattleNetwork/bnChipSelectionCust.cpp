#include "bnChipSelectionCust.h"

ChipSelectionCust::ChipSelectionCust()
{
	custSprite = sf::Sprite(*TextureResourceManager::GetInstance().GetTexture(TextureType::CHIP_SELECT_MENU));
	custSprite.setScale(2.f, 2.f);
	custSprite.setPosition(-custSprite.getTextureRect().width*2.f, 0);

	ChipLibrary::GetInstance().LoadLibrary();
}


ChipSelectionCust::~ChipSelectionCust()
{
}

bool ChipSelectionCust::IsOutOfView()
{
	float bounds = -custSprite.getTextureRect().width*2.f;

	if (custSprite.getPosition().x <= bounds) {
		custSprite.setPosition(bounds, custSprite.getPosition().y);
	}

	return (custSprite.getPosition().x == bounds);
}

bool ChipSelectionCust::IsInView()
{
	float bounds = 0;

	if (custSprite.getPosition().x >= bounds) {
		custSprite.setPosition(bounds, custSprite.getPosition().y);
	}

	return (custSprite.getPosition().x == bounds);
}

void ChipSelectionCust::Move(sf::Vector2f delta)
{
	custSprite.setPosition(custSprite.getPosition() + delta);
}

void ChipSelectionCust::GetNextChips()
{
	ClearChips();
	int chipCap = 3; // TODO: expose this in API

	selectedChips = (Chip**)malloc(sizeof(Chip*)*chipCap);

	for (int i = 0; i < chipCap; i++) {
		selectedChips[i] = ChipLibrary::GetInstance().Next();

		if (!selectedChips[i]) {
			// nullptr is end of list
			return;
		}
		chipCount++;
	}
}

void ChipSelectionCust::Draw()
{
	Engine::GetInstance().Draw(custSprite);
}

Chip** ChipSelectionCust::GetChips()
{
	return selectedChips;
}

void ChipSelectionCust::ClearChips()
{
	for (int i = 0; i < chipCount; i++) {
		delete selectedChips[i];
	}

	delete[] selectedChips;
	chipCount = 0;
}

const int ChipSelectionCust::GetChipCount()
{
	return chipCount;
}
