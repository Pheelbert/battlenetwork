#include "bnCannodumbMob.h"



CannodumbMob::CannodumbMob(Field* field) : MobFactory(field)
{
}


CannodumbMob::~CannodumbMob()
{
}

Mob* CannodumbMob::Build() {
  Mob* mob = new Mob(field);

  Tile* tile = field->GetAt(4, 2);
  if (!tile->IsWalkable()) { tile->SetState(TileState::NORMAL); }

  mob->Spawn<Mettaur, MettaurIdleState>(4, 2);

  return mob;
}