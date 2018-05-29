#include "bnRandomMettaurMob.h"
#include "bnProgsMan.h"
#include "bnBattleItem.h"
#include "bnStringEncoder.h"
#include "bnChip.h"

RandomMettaurMob::RandomMettaurMob(Field* field) : MobFactory(field)
{
}


RandomMettaurMob::~RandomMettaurMob()
{
}

Mob* RandomMettaurMob::Build() {
  Mob* mob = new Mob(field);
  mob->ToggleBossFlag();
  mob->RegisterRankedReward(1, BattleItem(Chip(72, 0, '*', 0, "Reflct", "Defends and reflects")));
  mob->RegisterRankedReward(5, BattleItem(Chip(83, 0, 'K', 0, "CrckPanel", "Cracks a panel")));
  mob->RegisterRankedReward(11, BattleItem(Chip(135, 0, 'P', 300, "ProgsMan", "Throws ProgBomb")));
  mob->RegisterRankedReward(11, BattleItem(Chip(136, 0, 'P', 300, EX("ProgsMan"), "Throws ProgBomb")));

  for (int i = 0; i < field->GetWidth(); i++) {
    for (int j = 0; j < field->GetHeight(); j++) {
      Tile* tile = field->GetAt(i + 1, j + 1);
      if (tile->IsWalkable() && tile->GetTeam() == Team::RED) {
        if (rand() % 50 > 40) {
          if (rand() % 10 > 8) {
            mob->Spawn<ProgsMan, ProgsManIdleState>(i + 1, j + 1);
          }
          else {
            mob->Spawn<Mettaur, MettaurIdleState>(i + 1, j + 1);
          }
        }
      }
    }
  }
  return mob;
}