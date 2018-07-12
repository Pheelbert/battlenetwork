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

  mob->Spawn<Canodumb, CanodumbIdleState>(5, 2, Canodumb::Rank::_1);
  mob->Spawn<Canodumb, CanodumbIdleState>(6, 1, Canodumb::Rank::_2);
  mob->Spawn<Canodumb, CanodumbIdleState>(6, 3, Canodumb::Rank::_3);

  return mob;
}