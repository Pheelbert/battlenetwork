#include "bnChipSelectionCust.h"



ChipSelectionCust::ChipSelectionCust()
{
	custSprite = sf::Sprite(*TextureResourceManager::GetInstance().GetTexture(TextureType::CHIP_SELECT_MENU));
	custSprite.setScale(2.f, 2.f);
	custSprite.setPosition(-custSprite.getTextureRect().width*2.f, 0);
}


ChipSelectionCust::~ChipSelectionCust()
{
}

bool ChipSelectionCust::IsOutOfView()
{
	return (custSprite.getPosition().x <= -custSprite.getTextureRect().width*2.f);
}

bool ChipSelectionCust::IsInView()
{
	return (custSprite.getPosition().x >= 0);
}

void ChipSelectionCust::Move(sf::Vector2f delta)
{
	custSprite.setPosition(custSprite.getPosition() + delta);
}

bool ChipSelectionCust::AreChipsReady()
{
	return false;
}

void ChipSelectionCust::Draw()
{
	Engine::GetInstance().Draw(custSprite);
}
