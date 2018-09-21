#pragma once

#include "bnAudioResourceManager.h"
#include "bnChipUseListener.h"
#include "bnRollHeal.h"
#include "bnPlayer.h"
#include "bnField.h"
#include "bnTile.h"
#include "bnTimer.h"

class ChipSummonHandler : public ChipUseListener {
private:
  Player * player;
  double timeInSecs;
  sf::Time duration;
  std::string summon;

  std::vector<Entity*> summonedItems; // We must handle our own summoned entites

public:
  ChipSummonHandler(Player* _player) : ChipUseListener() { player = _player; duration = sf::seconds(3); timeInSecs = 0; summon = std::string();  }
  ChipSummonHandler(Player& _player) : ChipUseListener() { player = &_player; duration = sf::seconds(3); timeInSecs = 0; summon = std::string(); }

  const bool IsSummonOver() {
    return timeInSecs >= duration.asSeconds() || summon.empty();
  }

  const bool IsSummonsActive() {
    return !IsSummonOver();
  }

  const std::string GetSummonLabel() { return summon; }

  void Update(double _elapsed) {
    if (summon.empty())
      return;

    timeInSecs += _elapsed;

    for (auto items : summonedItems) {
      items->Update((float)_elapsed);
    }
  }

  void OnEnter() { 
    if (summon == "Roll") {
      player->SetAlpha(0);

      Entity* roll = new RollHeal(player->GetField(), player->GetTile(), player->GetTeam(), 30);
      summonedItems.push_back(roll);
    }
  }

  void OnLeave() { 
    player->SetAlpha(255);  

    if (summon == "Roll") {
      player->SetHealth(player->GetHealth() + 30);
      AUDIO.Play(AudioType::RECOVER);
    }

    for (auto items : summonedItems) {
      delete items;
    }

    summonedItems.clear();

    summon = std::string();
  }

  void OnChipUse(Chip& chip) {
    player->SetCharging(false);

    std::string name = chip.GetShortName();

    if (name == "Roll v1") {
      summon = "Roll";
      timeInSecs = 0;
      duration = sf::seconds(2);
    }
  }
};
