#pragma once

class Mob;

class BattleScene {
public:
  // Returns 0 for LOSE, 1 for WIN, 2 for TIE, -1 for EXIT_FAILURE
  static int Run( Mob* mob);
};