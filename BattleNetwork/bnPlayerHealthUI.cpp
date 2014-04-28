#include <string>
using std::to_string;

#include "bnPlayer.h"
#include "bnPlayerHealthUI.h"
#include "bnResourceManager.h"

PlayerHealthUI::PlayerHealthUI(Entity* _entity)
    : player(nullptr),
      font(nullptr),
      texture(nullptr)
{
    PlayerHealthUI(dynamic_cast<Player*>(_entity));
}

PlayerHealthUI::PlayerHealthUI(Player* _player)
    : player(_player)
{
    font = ResourceManager::GetInstance().LoadFontFromFile("resources/fonts/mgm_nbr_pheelbert.ttf");
    texture = ResourceManager::GetInstance().LoadTextureFromFile("resources/ui/img_health.png");
    sprite.setTexture(*texture);
    sprite.setPosition(3.f, 0.0f);
    sprite.setScale(2.f, 2.f);
    components.push_back(&sprite);
    components.push_back(&text);
}

PlayerHealthUI::~PlayerHealthUI(void)
{
}

bool PlayerHealthUI::GetNextComponent(Drawable*& out)
{
    static int i = 0;
    while( i < (int)components.size())
    {
        out = components.at(i);
        i++;
        return true;
    }
    i = 0;
    return false;
}

void PlayerHealthUI::Update()
{
    if (player)
    {
        text = Text(to_string(player->GetHealth()), *font);
        text.setOrigin(text.getLocalBounds().width, 0);
        text.setPosition(80.0f, -4.f);
    }
}