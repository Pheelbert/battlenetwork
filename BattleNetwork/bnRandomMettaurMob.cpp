#include "bnRandomMettaurMob.h"

RandomMettaurMob::RandomMettaurMob(Field* field) : MobFactory(field)
{
}


RandomMettaurMob::~RandomMettaurMob()
{
}

Mob* RandomMettaurMob::Build() {
  Mob* mob = new Mob(field);

  for (int i = 0; i < field->GetWidth(); i++) {
    for (int j = 0; j < field->GetHeight(); j++) {
      Tile* tile = field->GetAt(i + 1, j + 1);
      if (tile->IsWalkable() && tile->GetTeam() == Team::RED) {
        if(rand() % 50 > 25)
          mob->Spawn<Mettaur, MettaurIdleState>(i + 1, j + 1);
      }
    }
  }
  return mob;
}