#pragma once
#include <SFML/Graphics.hpp>
using sf::CircleShape;
using sf::Sprite;
using sf::Drawable;
#include <Thor\Animation.hpp>
using thor::FrameAnimation;
#include <vector>
using std::vector;
#include <string>
using std::string;

#include "bnDirection.h"
#include "bnTeam.h"
#include "bnMemory.h"
#include "bnRenderer.h"
class Tile;
class Field;

class Entity : public LayeredDrawable
{
public:
    Entity(void)
    : tile(nullptr),
      previous(nullptr),
      field(nullptr),
      team(Team::UNKNOWN),
      deleted(false)
    {
    }

    virtual ~Entity(void)
    {
    }

    virtual void Update(float _elapsed)            = 0;
    virtual bool Move(Direction _direction)        = 0;
    virtual vector<Drawable*> GetMiscComponents()  = 0;
    virtual int getStateFromString(string _string) = 0;
    virtual void addAnimation(int _state, FrameAnimation _animation, float _duration) = 0;

    bool Teammate(Team _team)    { return team == _team; }

    void SetTile(Tile* _tile)    { tile = _tile; }
    Tile* GetTile() const        { return tile; }

    void SetField(Field* _field) { field = _field; }
    Field* GetField() const      { return field; }

    Team GetTeam() const         { return team; }
    void SetTeam(Team _team)     { team = _team; }

    bool IsDeleted() const       { return deleted; }

protected:
    Tile* tile;
    Tile* previous;
    Field* field;
    Team team;
    bool deleted;
};