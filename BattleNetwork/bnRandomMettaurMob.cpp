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
  mob->RegisterRankedReward(1, BattleItem(Chip(72, 0, '*', 0, "Reflct", "Defends and reflects")));
  mob->RegisterRankedReward(5, BattleItem(Chip(83, 0, 'K', 0, "CrckPanel", "Cracks a panel")));

  for (int i = 0; i < field->GetWidth(); i++) {
    for (int j = 0; j < field->GetHeight(); j++) {
      Tile* tile = field->GetAt(i + 1, j + 1);

      tile->SetState((TileState)(rand() % (int)TileState::EMPTY)); // Make it random excluding an empty tile

      if (tile->IsWalkable() && tile->GetTeam() == Team::RED) {
        if (rand() % 50 > 30) {
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

  if (mob->GetMobCount() == 0) {
    int x = (field->GetWidth() / 2) + 1;
    int y = (field->GetHeight() / 2) + 1;
    mob->Spawn<Mettaur, MettaurIdleState>(x, y);

    Tile* tile = field->GetAt(x, y);
    if (!tile->IsWalkable()) { tile->SetState(TileState::NORMAL); }
  }

  return mob;
}