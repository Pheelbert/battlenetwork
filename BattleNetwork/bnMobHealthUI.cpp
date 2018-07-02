#include <string>
using std::to_string;

#include "bnMobHealthUI.h"
#include "bnCharacter.h"
#include "bnTextureResourceManager.h"

#include "bnLogger.h"

MobHealthUI::MobHealthUI(void)
  : mob(nullptr),
  font(nullptr) {
}

MobHealthUI::MobHealthUI(Character* _mob)
  : mob(_mob) {
  font = TEXTURES.LoadFontFromFile("resources/fonts/mgm_nbr_pheelbert.ttf");
  setFont(*font);
  setOutlineColor(sf::Color(48,56,80));
  setOutlineThickness(2.f);
  setScale(0.8f, 0.8f);
}

MobHealthUI::~MobHealthUI(void) {
  delete font;
}

void MobHealthUI::Update() {
  if (mob) {
    setOrigin(getLocalBounds().width, 0);
    setString(to_string(mob->GetHealth()));

    int* res = mob->GetAnimOffset();

    if (res != nullptr) {
      setPosition(mob->getPosition().x + res[0], mob->getPosition().y + res[1]);
      delete[] res;
    } else {
      setPosition(mob->getPosition().x, mob->getPosition().y);
    }
  }
}