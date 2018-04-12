#include <string>
using std::to_string;

#include "bnPlayer.h"
#include "bnSelectedChipsUI.h"
#include "bnTextureResourceManager.h"
#include "bnChip.h"
#include "bnEngine.h"

SelectedChipsUI::SelectedChipsUI(Entity* _entity)
	: player(nullptr),
	font(nullptr)
{
	SelectedChipsUI(dynamic_cast<Player*>(_entity));
}

SelectedChipsUI::SelectedChipsUI(Player* _player)
	: player(_player)
{
	chipCount = curr = 0;
	icon = sf::Sprite(*TextureResourceManager::GetInstance().GetTexture(CHIP_ICON));
	icon.setScale(sf::Vector2f(2.f, 2.f));

	font = TextureResourceManager::GetInstance().LoadFontFromFile("resources/fonts/mmbnthick_regular.ttf");
	components.push_back(&text);
	components.push_back(&dmg);
}

SelectedChipsUI::~SelectedChipsUI(void)
{
}

bool SelectedChipsUI::GetNextComponent(Drawable*& out)
{
	static int i = 0;
	while (i < (int)components.size())
	{
		out = components.at(i);
		i++;
		return true;
	}
	i = 0;
	return false;
}

void SelectedChipsUI::Update()
{
	if (player)
	{
		// TODO: Move draw out of update. Utilize components.
		for(int i = curr; i < chipCount; i++) {
			icon.setPosition(player->getPosition() + sf::Vector2f(30.0f -(i-curr)*3, -(i-curr)*3));
			Engine::GetInstance().Draw(icon);
		}

		if (chipCount > 0 && curr < chipCount) {
			text = Text(sf::String(selectedChips[curr]->GetShortName()), *font);
			text.setOrigin(0, 0);
			text.setPosition(3.0f, 290.0f);
			dmg = Text(to_string(selectedChips[curr]->GetDamage()), *font);
			dmg.setOrigin(0, 0);
			dmg.setPosition(text.getLocalBounds().width + 13.f,290.f);
			dmg.setColor(sf::Color(225, 140, 0));
		}
		else {
			text.setString("");
			dmg.setString("");
		}
	}
}

void SelectedChipsUI::LoadChips(Chip ** incoming, int size)
{
	selectedChips = incoming;
	chipCount = size;
	curr = 0;
}

void SelectedChipsUI::UseNextChip()
{
	// TODO: Spells
	if (curr < chipCount) { curr++; }
}
