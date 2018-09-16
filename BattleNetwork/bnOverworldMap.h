#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "bnTextureResourceManager.h"
#include "bnCamera.h"
#include "bnOverworldLight.h"

namespace Overworld {
  class Tile {
    sf::Texture* texture;
    sf::Vector2f pos;

    bool cleanup;

    void LoadTexture() { 
      int randTex = rand() % 100;

      if (randTex > 80) {
        texture = TEXTURES.GetTexture(TextureType::MAIN_MENU_OW2);
      }
      else {
        texture = TEXTURES.GetTexture(TextureType::MAIN_MENU_OW);
      }
    }

  public:
    Tile() { pos = sf::Vector2f(0, 0); LoadTexture(); cleanup = false;  }
    Tile(const Tile& rhs) { texture = rhs.texture; pos = rhs.pos;  cleanup = false; }
    Tile(sf::Texture* _texture, sf::Vector2f pos = sf::Vector2f()) : pos(pos) { texture = _texture; cleanup = false; }
    Tile(sf::Vector2f pos) : pos(pos) { LoadTexture(); cleanup = false;}
    ~Tile() { ; }
    const sf::Vector2f GetPos() const { return pos; }
    const sf::Texture& GetTexture() { return *texture; }

    void Cleanup() {
      cleanup = true;
    }

    bool ShouldRemove() {
      return cleanup;
    }

  };

  class Map : public sf::Drawable
  {
  protected:
    std::vector<Tile*> map;
    std::vector<Overworld::Light*> lights;
    std::vector<sf::Sprite*> sprites;

    bool enableLighting;

    int cols, rows;
    int tileWidth, tileHeight;
    Camera* cam;

    const sf::Vector2f OrthoToIsometric(sf::Vector2f ortho) const;
    const sf::Vector2f IsoToOrthogonal(sf::Vector2f iso) const;

    void DeleteTiles();
    virtual void DrawTiles(sf::RenderTarget& target, sf::RenderStates states) const ;
    virtual void DrawSprites(sf::RenderTarget& target, sf::RenderStates states) const;

    struct TileComparitor
    {
    private:
      Overworld::Map* map;

    public:
      TileComparitor(Map* _map) {
        map = _map;
      }

      inline bool operator() (const Overworld::Tile* a, const Overworld::Tile* b)
      {
        sf::Vector2f isoA = map->OrthoToIsometric(a->GetPos());
        sf::Vector2f isoB = map->OrthoToIsometric(b->GetPos());

        return (isoA.y < isoB.y);
      }
    };

  public:
    Map(int numOfCols, int numOfRows, int tileWidth, int tileHeight);

    void ToggleLighting(bool state = true);

    const sf::Vector2f ScreenToWorld(sf::Vector2f screen) const;

    ~Map();

    void SetCamera(Camera* _camera);

    void AddLight(Overworld::Light* _light);
    void AddSprite(sf::Sprite* _sprite);
    void RemoveSprite(sf::Sprite * _sprite);

    virtual void Update(double elapsed);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    const sf::Vector2i GetTileSize() const;
  };
}