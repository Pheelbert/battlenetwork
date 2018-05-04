#include <string>
using std::to_string;

#include "bnPlayer.h"
#include "bnField.h"
#include "bnCannon.h"
#include "bnTile.h"
#include "bnSelectedChipsUI.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnChip.h"
#include "bnChipType.h"
#include "bnEngine.h"

SelectedChipsUI::SelectedChipsUI(Entity* _entity)
  : player(nullptr),
  font(nullptr) {
  SelectedChipsUI(dynamic_cast<Player*>(_entity));
}

SelectedChipsUI::SelectedChipsUI(Player* _player)
  : player(_player) {
  chipCount = curr = 0;
  icon = sf::Sprite(*TextureResourceManager::GetInstance().GetTexture(CHIP_ICONS));
  icon.setScale(sf::Vector2f(2.f, 2.f));

  font = TextureResourceManager::GetInstance().LoadFontFromFile("resources/fonts/mmbnthick_regular.ttf");
  components.push_back(&text);
  components.push_back(&dmg);
}

SelectedChipsUI::~SelectedChipsUI(void) {
}

bool SelectedChipsUI::GetNextComponent(Drawable*& out) {
  static int i = 0;
  while (i < (int)components.size()) {
    out = components.at(i);
    i++;
    return true;
  }
  i = 0;
  return false;
}

void SelectedChipsUI::Update() {
  if (player) {
    // TODO: Move chip use logic out of chip UI. Utilize component.s
    if (invisTimer.getElapsedTime() > sf::seconds(10)) {
      player->SetPassthrough(false);
      player->setColor(sf::Color(255, 255, 255, 255));
    }

    // TODO: Move draw out of update. Utilize components.
    int chipOrder = 0;
    for (int i = curr; i < chipCount; i++) {
      icon.setPosition(player->getPosition() + sf::Vector2f(30.0f - (i - curr) * 3.0f, - (i - curr) * 3.0f));
      sf::IntRect iconSubFrame = TextureResourceManager::GetInstance().GetIconRectFromID(selectedChips[curr]->GetIconID());
      icon.setTextureRect(iconSubFrame);
      Engine::GetInstance().Draw(icon);
    }

    if (chipCount > 0 && curr < chipCount && selectedChips[curr]) {
      text = Text(sf::String(selectedChips[curr]->GetShortName()), *font);
      text.setOrigin(0, 0);
      text.setScale(0.8f, 0.8f);
      text.setPosition(3.0f, 290.0f);
      text.setOutlineThickness(2.f);
      text.setOutlineColor(sf::Color(48, 56, 80));

      if (selectedChips[curr]->GetDamage() > 0) {
        dmg = Text(to_string(selectedChips[curr]->GetDamage()), *font);
        dmg.setOrigin(0, 0);
        dmg.setScale(0.8f, 0.8f);
        dmg.setPosition((text.getLocalBounds().width*text.getScale().x) + 13.f, 290.f);
        dmg.setFillColor(sf::Color(225, 140, 0));
        dmg.setOutlineThickness(2.f);
        dmg.setOutlineColor(sf::Color(48, 56, 80));
      }
    } else {
      text.setString("");
      dmg.setString("");
    }
  }
}

void SelectedChipsUI::LoadChips(Chip ** incoming, int size) {
  selectedChips = incoming;
  chipCount = size;
  curr = 0;
}

void SelectedChipsUI::UseNextChip() {
  if (curr >= chipCount) {
    return;
  }

  if (selectedChips[curr]->GetID() == ChipType::HP10) {
    player->SetHealth(player->GetHealth() + 10);
    AudioResourceManager::GetInstance().Play(AudioType::RECOVER);
  } else if (selectedChips[curr]->GetID() == ChipType::CRCKPNL) {
    Tile* top = player->GetField()->GetAt(player->GetTile()->GetX() + 1, 1);
    Tile* mid = player->GetField()->GetAt(player->GetTile()->GetX() + 1, 2);
    Tile* low = player->GetField()->GetAt(player->GetTile()->GetX() + 1, 3);

    if (top) { top->SetState(TileState::CRACKED); }
    if (mid) { mid->SetState(TileState::CRACKED); }
    if (low) { low->SetState(TileState::CRACKED); }

    AudioResourceManager::GetInstance().Play(AudioType::PANEL_CRACK);
  }
  else if (selectedChips[curr]->GetID() == ChipType::INVSBLE) {
    // Todo make this a time-based component
    AudioResourceManager::GetInstance().Play(AudioType::INVISIBLE);
    player->SetPassthrough(true);
    player->setColor(sf::Color(255, 255, 255, (sf::Uint8)(255 / 2.f)));
    invisTimer.restart();
  }
  else if (selectedChips[curr]->GetShortName() == "XtrmeCnnon") {
    AudioResourceManager::GetInstance().Play(AudioType::CANNON);
    Cannon* xtreme1 = new Cannon(player->GetField(), player->GetTeam(), 600);
    Cannon* xtreme2 = new Cannon(player->GetField(), player->GetTeam(), 600);
    Cannon* xtreme3 = new Cannon(player->GetField(), player->GetTeam(), 600);

    player->SetAnimation(PlayerState::PLAYER_SHOOTING);
    xtreme1->SetDirection(Direction::RIGHT);
    xtreme2->SetDirection(Direction::RIGHT);
    xtreme3->SetDirection(Direction::RIGHT);

    player->GetField()->OwnEntity(xtreme1, 4, 1);
    player->GetField()->OwnEntity(xtreme2, 4, 2);
    player->GetField()->OwnEntity(xtreme3, 4, 3);
  }

  curr++;
}
