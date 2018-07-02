#include "bnInfiniteMap.h"

namespace Overworld {

  InfiniteMap::InfiniteMap(int branchDepth, int numOfCols, int tileWidth, int tileHeight) 
    : Overworld::Map(numOfCols, 0, tileWidth, tileHeight)
  {
    this->branchDepth = branchDepth;

    ToggleLighting(false);

    DeleteTiles();

    // One long road
    int row = 0;
    for (int i = 0; i < numOfCols; i++) {
       map.push_back(new Tile(sf::Vector2f((float)i*tileWidth, (float)row*tileHeight)));
    }

    head = map.back();
  }

  InfiniteMap::~InfiniteMap()
  {
  }


  void InfiniteMap::DrawTiles(sf::RenderTarget& target, sf::RenderStates states) const {
    //std::cout << "map size: " << map.size() << "\n";
    for (int i = 0; i < map.size(); i++) {
      sf::Sprite tileSprite(map[i]->GetTexture());


      tileSprite.setScale(2.0f, 2.0f);
      sf::Vector2f pos = map[i]->GetPos();

      if (cam) {
        sf::View view = cam->GetView();
        sf::Vector2f offset = IsoToOrthogonal(view.getCenter() - (view.getSize() / 2.0f));

        pos -= offset;

        pos = OrthoToIsometric(pos*2.0f);

        tileSprite.setPosition(pos);

        if (!cam->IsInView(tileSprite)) {
          if (pos.x < 0) {
            if (&(*head) != &(*map[i])) {
              map[i]->Cleanup();
              continue;
            }
          }
        }
      }

      pos = map[i]->GetPos();

      if (cam) {
        sf::View view = cam->GetView();
        sf::Vector2f offset = IsoToOrthogonal(view.getCenter() - (view.getSize() / 2.0f));

        pos -= offset;
      }

      pos = OrthoToIsometric(pos*2.0f);

      if (cam) {
        tileSprite.setPosition(pos);

        if (cam->IsInView(tileSprite)) {
          target.draw(tileSprite, states);
        }
      }
    }
  }
  
  void InfiniteMap::Update()
  {
    Map::Update();

    std::sort(map.begin(), map.end(), InfiniteMap::TileComparitor(this));

    if (std::max((int)(map.size()-branchDepth), 0) < cols) {

      Overworld::Tile* tile = new Tile(sf::Vector2f(head->GetPos().x + this->GetTileSize().x, head->GetPos().y));
      map.push_back(tile);

      head = tile;

      std::sort(map.begin(), map.end(), InfiniteMap::TileComparitor(this));

      int randBranch = rand() % 100;

      int depth = 0;
      if (randBranch < 10) {

        Overworld::Tile* offroad = head;

        while (depth < branchDepth) {
          int randDirection = rand() % 3;

          if (randDirection == 0) {
            offroad = new Tile(sf::Vector2f(offroad->GetPos().x, offroad->GetPos().y + this->GetTileSize().y));
            map.push_back(offroad);
            depth++;
          }
          else if (randDirection == 1) {
            offroad = new Tile(sf::Vector2f(offroad->GetPos().x, offroad->GetPos().y - this->GetTileSize().y));
            map.push_back(offroad);
            depth++;
          }
          else if (depth > 1) {
            offroad = new Tile(sf::Vector2f(offroad->GetPos().x + this->GetTileSize().x, offroad->GetPos().y));
            map.push_back(offroad);
            depth++;
          }

          std::sort(map.begin(), map.end(), InfiniteMap::TileComparitor(this));
        }
      }
    }
  }
}