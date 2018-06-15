#include "bnProgsManBossFight.h"
#include "bnProgsMan.h"
#include "bnBattleItem.h"
#include "bnStringEncoder.h"
#include "bnChip.h"

ProgsManBossFight::ProgsManBossFight(Field* field) : MobFactory(field)
{
}


ProgsManBossFight::~ProgsManBossFight()
{
}

Mob* ProgsManBossFight::Build() {
  Mob* mob = new Mob(field);
  mob->ToggleBossFlag();
  mob->RegisterRankedReward(1, BattleItem(Chip(135, 0, 'P', 300, "ProgsMan", "Throws ProgBomb")));
  mob->RegisterRankedReward(11, BattleItem(Chip(136, 0, 'P', 300, EX("ProgsMan"), "Throws ProgBomb")));

  int x = (field->GetWidth() / 2) + 1;
  int y = (field->GetHeight() / 2) + 1;
  mob->Spawn<ProgsMan, ProgsManIdleState>(x, y);

  Tile* tile = field->GetAt(x, y);
  if (!tile->IsWalkable()) { tile->SetState(TileState::NORMAL); }

  return mob;
}