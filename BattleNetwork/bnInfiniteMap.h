#pragma once

#include "bnOverworldMap.h"
#include "bnAnimate.h"
#include "bnAnimation.h"

namespace Overworld {
  enum class NPCType :int {
    MR_PROG_UP,
    MR_PROG_DOWN,
    MR_PROG_LEFT,
    MR_PROG_RIGHT,
    MR_PROG_FIRE
  };

  struct NPC {
    sf::Sprite sprite;
    NPCType type;
  };

  class InfiniteMap : public Overworld::Map
  {
  private:
    Overworld::Tile* head;
    int branchDepth;
    
    Animate animator;
    Animation progAnimations;

    std::vector<NPC*> npcs;

  public:
    InfiniteMap(int branchDepth, int numOfCols, int tileWidth, int tileHeight);
    ~InfiniteMap();

    // Overwrite
    //virtual void DrawTiles(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void Update(double elapsed);
  };
}