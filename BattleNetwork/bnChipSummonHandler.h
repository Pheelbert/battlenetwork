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

  Player* GetPlayer() {
    return player;
  }

  void SummonEntity(Entity* _new) {
    summonedItems.push_back(_new);
  }

  void RemoveEntity(Entity* _entity) {
    for (auto items = summonedItems.begin(); items != summonedItems.end(); items++) {
      if (*items == _entity) {
        if ((*items)->GetTile()) {
          (*items)->GetTile()->RemoveEntity(*items);
        }

        delete *items;
        summonedItems.erase(items);
        return;
      }
    }
  }

  void Update(double _elapsed) {
    if (summon.empty())
      return;

    player->Update(0);
    player->GetAnimationComponent().Update(_elapsed);

    timeInSecs += _elapsed;

    for (auto items : summonedItems) {
      items->Update((float)_elapsed);
    }
  }

  void OnEnter() { 
    if (summon == "Roll") {
      player->SetAlpha(0);

      Entity* roll = new RollHeal(this, 10);
      SummonEntity(roll);
    }
  }

  void OnLeave() { 
    player->SetAlpha(255);  

    for (auto items : summonedItems) {
      delete items;
    }

    summonedItems.clear();

    summon = std::string();
  }

  void OnChipUse(Chip& chip) {
    player->SetCharging(false);

    std::string name = chip.GetShortName();

    if (name == "Roll") {
      summon = "Roll";
      timeInSecs = 0;
      duration = sf::seconds(4);
    }
  }
};
