#include "bnChipSelectionCust.h"

ChipSelectionCust::ChipSelectionCust(int cap)
{
	chipCap = cap;
	queue = (Chip**)malloc(sizeof(Chip*)*chipCap);

	chipCount = selectCount = cursorPos = 0;

	custSprite = sf::Sprite(*TextureResourceManager::GetInstance().GetTexture(TextureType::CHIP_SELECT_MENU));
	custSprite.setScale(2.f, 2.f);
	custSprite.setPosition(-custSprite.getTextureRect().width*2.f, 0);

	icon = sf::Sprite(*TextureResourceManager::GetInstance().GetTexture(CHIP_ICON));
	icon.setScale(sf::Vector2f(2.f, 2.f));

	cursorSmall = sf::Sprite(*TextureResourceManager::GetInstance().GetTexture(TextureType::CHIP_CURSOR_SMALL));
	cursorSmall.setScale(sf::Vector2f(2.f, 2.f));

	cursorBig = sf::Sprite(*TextureResourceManager::GetInstance().GetTexture(TextureType::CHIP_CURSOR_BIG));
	cursorBig.setScale(sf::Vector2f(2.f, 2.f));
	cursorBig.setPosition(sf::Vector2f(2.f*85.f, 2.f*104.f));

	ChipLibrary::GetInstance().LoadLibrary();
}


ChipSelectionCust::~ChipSelectionCust()
{
	ClearChips();

	for (int i = 0; i < chipCount; i++) {
		delete queue[i];
	}

	if(chipCount > 0)
		delete[] queue;

	chipCount = 0;
}

void ChipSelectionCust::CursorRight()
{
	if (++cursorPos > 5) {
		cursorPos = 5;
	}
}

void ChipSelectionCust::CursorLeft()
{
	if (--cursorPos < 0) {
		cursorPos = 0;
	}
}

void ChipSelectionCust::CursorAction()
{
	if (cursorPos == 5) {
		// End chip select
		areChipsReady = true;
		chipCount -= selectCount;
	}
	else {
		// Does chip exist 
		if (cursorPos < chipCount) {
			// Queue this chip
			selectedChips[selectCount++] = queue[cursorPos];
		}
	}
}

void ChipSelectionCust::CursorCancel()
{
	// Unqueue all chips 
	selectCount--;
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

	selectedChips = (Chip**)malloc(sizeof(Chip*)*chipCap);

	std::cout << "chipCount: " << chipCount << "\n";

	for (int i = chipCount; i < chipCap; i++) {
		queue[i] = ChipLibrary::GetInstance().Next();

		if (!queue[i]) {
			// nullptr is end of list
			return;
		}
		chipCount++;
	}
}

void ChipSelectionCust::Draw()
{
	Engine::GetInstance().Draw(custSprite, false);

	if (IsInView()) {
		cursorSmall.setPosition(2.f*(8.0f + (cursorPos*16.0f)), 2.f*104.f); // TODO: Make this relative to cust instead of screen

		if (cursorPos < 5)
			Engine::GetInstance().Draw(cursorSmall, false);
		else
			Engine::GetInstance().Draw(cursorBig,false);

		for (int i = 0; i < chipCount; i++) {
			icon.setPosition(2.f*(9.0f + (i*16.0f)), 2.f*106.f);
			Engine::GetInstance().Draw(icon,false);
		}
	}
}

Chip** ChipSelectionCust::GetChips()
{
	return selectedChips;
}

void ChipSelectionCust::ClearChips()
{
	for (int i = 0; i < selectCount; i++) {
		delete selectedChips[i];
		selectedChips[i] = nullptr;
	}

	if(selectCount > 0)
		delete[] selectedChips;

	// Restructure queue
	for (int i = 0; i < chipCount; i++) {
		int next = i;
		while (!queue[next] && next < chipCount) {
			queue[i] = queue[next + 1];
			queue[next + 1] = nullptr;
			next++;
		}
	}

	chipCount = chipCount - selectCount;
	selectCount = 0;
}

const int ChipSelectionCust::GetChipCount()
{
	return selectCount;
}

void ChipSelectionCust::ResetState()
{
	areChipsReady = false;
}

bool ChipSelectionCust::AreChipsReady()
{
	return areChipsReady;
}
