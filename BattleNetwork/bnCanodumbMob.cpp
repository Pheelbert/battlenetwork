#include "bnCanodumbMob.h"

CanodumbMob::CanodumbMob(Field* field) : MobFactory(field)
{
}


CanodumbMob::~CanodumbMob()
{
}

Mob* CanodumbMob::Build() {
  Mob* mob = new Mob(field);

  Tile* tile = field->GetAt(4, 2);
  if (!tile->IsWalkable()) { tile->SetState(TileState::NORMAL); }

  mob->Spawn<Canodumb, CanodumbIdleState>(4, 2);

  return mob;
}