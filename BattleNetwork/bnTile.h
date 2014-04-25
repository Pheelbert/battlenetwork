#pragma once
#include <SFML/Graphics.hpp>
using sf::RectangleShape;
using sf::Sprite;
#include <vector>
using std::vector;
#include <algorithm>
using std::find;

#include "bnTeam.h"
class Entity;
class Spell;
class Field;
#include "bnMemory.h"
#include "bnTextureType.h"
#include "bnTileState.h"

class Tile : public Sprite
{
public:
    Tile(void);
    Tile(int _x, int _y);
    ~Tile(void);

    void SetField(Field* _field);

    int GetX() const;
    int GetY() const;

    Team GetTeam() const;
    void SetTeam(Team _team);

    float GetWidth() const;
    float GetHeight() const;

    void SetState(TileState _state);
    void RefreshTexture();

    bool IsWalkable() const;
    bool IsCracked() const;

    void AddEntity(Entity* _entity);
    void RemoveEntity(Entity* _entity);
    bool ContainsEntity(Entity* _entity) const;
    void AffectEntities(Spell* caller);
    bool GetNextEntity(Entity*& out) const;

    void Update(float _elapsed);

private:
    int x;
    int y;
    Team team;
    TileState state;
    TextureType ttype;
    float elapsed;
    vector<Entity*> entities;
    float width;
    float height;
    Field* field;
};