#include <string>
using std::to_string;

#include "bnMobHealthUI.h"
#include "bnMettaur.h"
#include "bnResourceManager.h"

#include "bnLogger.h"

MobHealthUI::MobHealthUI(void)
    : mob(nullptr),
      font(nullptr)
{
}

MobHealthUI::MobHealthUI(Mettaur* _mob)
    : mob(_mob)
{
    font = ResourceManager::GetInstance().LoadFontFromFile("resources/fonts/mgm_nbr_pheelbert.ttf");
    setFont(*font);
    setScale(0.8f, 0.8f);
}

MobHealthUI::~MobHealthUI(void)
{
    delete font;
}

void MobHealthUI::Update()
{
    if (mob)
    {
        setOrigin(getLocalBounds().width, 0);
        setString(to_string(mob->GetHealth()));
        if (mob->GetTextureType() == TextureType::MOB_METTAUR_IDLE)
        {
            setPosition(mob->getPosition().x + 35.f, mob->getPosition().y + 35.f);
        }
        else if (mob->GetTextureType() == TextureType::MOB_METTAUR_ATTACK)
        {
            setPosition(mob->getPosition().x + 65.f, mob->getPosition().y + 95.f);
        }
        else if (mob->GetTextureType() == TextureType::MOB_MOVE)
        {
            setPosition(mob->getPosition().x + 45.f, mob->getPosition().y + 55.f);
        }
    }
}