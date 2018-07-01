#pragma once

#include "bnOverworldMap.h"

namespace Overworld {
  class InfiniteMap : public Overworld::Map
  {
  private:
    Overworld::Tile* head;
    int branchDepth;

  public:
    InfiniteMap(int branchDepth, int numOfCols, int tileWidth, int tileHeight);
    ~InfiniteMap();

    // Overwrite
    virtual void DrawTiles(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void Update();
  };
}