#include <string>
using std::to_string;

#include "bnMobHealthUI.h"
#include "bnCharacter.h"
#include "bnTextureResourceManager.h"

#include "bnLogger.h"

MobHealthUI::MobHealthUI(void)
  : mob(nullptr),
  font(nullptr) {
  healthCounter = 0;
}

MobHealthUI::MobHealthUI(Character* _mob)
  : mob(_mob) {
  font = TEXTURES.LoadFontFromFile("resources/fonts/mgm_nbr_pheelbert.ttf");
  setFont(*font);
  setOutlineColor(sf::Color(48,56,80));
  setOutlineThickness(2.f);
  setScale(0.8f, 0.8f);
  healthCounter = mob->GetHealth();
  loaded = false;
}

MobHealthUI::~MobHealthUI(void) {
  delete font;
}

void MobHealthUI::Update() {
  if (mob) {
    if (!loaded) {
      healthCounter = mob->GetHealth();
      loaded = true;
    }

    setOrigin(getLocalBounds().width, 0);

    if (mob->GetHealth() <= 0) {
      setString("");
      return;
    }

    // Only delay damage display if 80 or more HP in the red
    if (healthCounter > mob->GetHealth() &&  healthCounter - mob->GetHealth() < 80) {
      healthCounter = mob->GetHealth();
    }

    if (healthCounter > mob->GetHealth()) {
      healthCounter--;
      setFillColor(sf::Color(255, 165, 0));

    }
    else if (healthCounter < mob->GetHealth()) {
      healthCounter++;
      setFillColor(sf::Color(0, 255, 80));
    }
    else {
      setFillColor(sf::Color::White);
    }

    setString(to_string(healthCounter));

    int* res = mob->GetAnimOffset();

    if (res != nullptr) {
      setPosition(mob->getPosition().x + res[0], mob->getPosition().y + res[1]);
      delete[] res;
    } else {
      setPosition(mob->getPosition().x, mob->getPosition().y);
    }
  }
}