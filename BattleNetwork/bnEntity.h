#pragma once
#include <Thor\Animation.hpp>
using thor::FrameAnimation;
#include <string>
using std::string;

#include "bnDirection.h"
#include "bnTeam.h"
#include "bnMemory.h"
#include "bnRenderer.h"
#include "bnTextureType.h"
class Tile;
class Field;

class Entity : public LayeredDrawable
{
public:
    Entity(void);
    virtual ~Entity(void);

    virtual void Update(float _elapsed);
    virtual bool Move(Direction _direction);
    virtual vector<Drawable*> GetMiscComponents();
    virtual int GetStateFromString(string _string);
    virtual void addAnimation(int _state, FrameAnimation _animation, float _duration);
    virtual int GetHealth();
    virtual TextureType GetTextureType();

    bool Teammate(Team _team);

    void SetTile(Tile* _tile);
    Tile* GetTile() const;

    void SetField(Field* _field);
    Field* GetField() const;

    Team GetTeam() const;
    void SetTeam(Team _team);

    bool IsDeleted() const;

protected:
    Tile* tile;
    Tile* previous;
    Field* field;
    Team team;
    bool deleted;
};