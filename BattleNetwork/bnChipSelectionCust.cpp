#include "bnChipSelectionCust.h"
#include "bnTextureResourceManager.h"

ChipSelectionCust::ChipSelectionCust(int cap)
{
	chipCap = cap;
	queue = new Bucket[chipCap];
	selectQueue = new Bucket*[chipCap];

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
	cursorBig.setPosition(sf::Vector2f(2.f*90.f, 2.f*114.f));

	sf::Font* font = TextureResourceManager::GetInstance().LoadFontFromFile("resources/fonts/mmbnthick_regular.ttf");
	label.setFont(*font);

	ChipLibrary::GetInstance().LoadLibrary();
}


ChipSelectionCust::~ChipSelectionCust()
{
	ClearChips();

	for (int i = 0; i < chipCount; i++) {
		selectQueue[i] = nullptr;
		delete queue[i].data;
	}

	if (chipCount > 0) {
		delete[] queue;
		delete[] selectQueue;
	}

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
			// Queue this chip if not selected
			if (queue[cursorPos].state) {
				selectQueue[selectCount++] = &queue[cursorPos];
				queue[cursorPos].state = 0;
			}
		}
	}
}

void ChipSelectionCust::CursorCancel()
{
	// Unqueue all chips buckets
	if (selectCount <= 0) {
		selectCount = 0;
		return;
	}

	selectQueue[selectCount-1]->state = 1;
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

	for (int i = chipCount; i < chipCap; i++) {

		queue[i].data = ChipLibrary::GetInstance().Next();
		queue[i].state = 1;

		if (!queue[i].data) {
			// nullptr is end of list
			return;
		}
		chipCount++;
	}

	areChipsReady = false;
}

void ChipSelectionCust::Draw()
{
	Engine::GetInstance().Draw(custSprite, false);

	if (IsInView()) {
		cursorSmall.setPosition(2.f*(7.0f + (cursorPos*16.0f)), 2.f*103.f); // TODO: Make this relative to cust instead of screen

		for (int i = 0; i < chipCount; i++) {
			icon.setPosition(2.f*(9.0f + (i*16.0f)), 2.f*106.f);

			if(queue[i].state)
				Engine::GetInstance().Draw(icon,false);
		}

		for (int i = 0; i < selectCount; i++) {
			icon.setPosition(2*97.f, 2.f*(25.0f + (i*16.0f)));

			Engine::GetInstance().Draw(icon, false);
		}


		if (cursorPos < 5) {
			// Draw the selected chip info
			label.setColor(sf::Color::White);

			if (cursorPos < chipCount) {
				label.setPosition(2.f*9.f, 29.f);
				label.setString(queue[cursorPos].data->GetShortName());
				Engine::GetInstance().Draw(label, false);

				label.setPosition(2.f*45.f, 110.f);
				label.setString(queue[cursorPos].data->GetDamage());
				Engine::GetInstance().Draw(label, false);

				label.setPosition(2.f*9.f, 110.f);
				label.setString(queue[cursorPos].data->GetCode());
				label.setColor(sf::Color(225, 140, 0));
				Engine::GetInstance().Draw(label, false);
			}

			// Draw the small cursor
			Engine::GetInstance().Draw(cursorSmall, false); \
		}
		else
			Engine::GetInstance().Draw(cursorBig, false);
	}
}

Chip** ChipSelectionCust::GetChips()
{
	selectedChips = (Chip**)malloc(sizeof(Chip*)*chipCap);

	for (int i = 0; i < selectCount; i++) {
		selectedChips[i] = (*(selectQueue[i])).data;
	}

	return selectedChips;
}

void ChipSelectionCust::ClearChips()
{
	for (int i = 0; i < selectCount; i++) {
		if (selectedChips[i]) {
			delete selectedChips[i];
		}

		selectQueue[i] = nullptr;
		selectedChips[i] = nullptr;
	}

	if(selectCount > 0)
		delete[] selectedChips;

	// Restructure queue
	for (int i = 0; i < chipCount; i++) {
		int next = i;
		while (!queue[next].data && next < chipCount) {
			queue[i].data = queue[next + 1].data;
			queue[next + 1].data = nullptr;
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
