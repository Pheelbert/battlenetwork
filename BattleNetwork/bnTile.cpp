#include "bnTile.h"
#include "bnEntity.h"
#include "bnSpell.h"
#include "bnPlayer.h"
#include "bnResourceManager.h"
#include "bnField.h"

#define START_X 0.0f
#define START_Y 144.f
#define COOLDOWN 4.f
#define Y_OFFSET 10.0f

Tile::Tile(int _x, int _y)
{
    x = _x;
    y = _y;
    if (x <= 3)
    {
        team = Team::BLUE;
    }
    else
    {
        team = Team::RED;
    }
    state = TileState::NORMAL;
    RefreshTexture();
    elapsed = 0;
    entities = vector<Entity*>();
    setScale(2.f, 2.f);
    width = getTextureRect().width * getScale().x;
    height = getTextureRect().height * getScale().y;
    setPosition(((x - 1) * width) + START_X, ((y - 1) * (height - Y_OFFSET)) + START_Y);
}

Tile::~Tile(void)
{
    if (entities.size() > 0)
    {
        FreeClear(entities);
    }
}

void Tile::SetField(Field* _field)
{
    field = _field;
}

int Tile::GetX() const
{
    return x;
}

int Tile::GetY() const
{
    return y;
}

Team Tile::GetTeam() const
{
    return team;
}

void Tile::SetTeam(Team _team)
{
    team = _team;
}

float Tile::GetWidth() const
{
    return width;
}

float Tile::GetHeight() const
{
    return height;
}

void Tile::SetState(TileState _state)
{
    state = _state;
}

void Tile::RefreshTexture()
{
    sf::Vector2f test = getPosition();
    if (state == TileState::NORMAL)
    {
        if (team == Team::BLUE)
        {
            ttype = TextureType::TILE_BLUE_NORMAL;
        }
        else
        {
            ttype = TextureType::TILE_RED_NORMAL;
        }
    }
    else if (state == TileState::CRACKED)
    {
        if (team == Team::BLUE)
        {
            ttype = TextureType::TILE_BLUE_CRACKED;
        }
        else
        {
            ttype = TextureType::TILE_RED_CRACKED;
        }
    }
    else if (state == TileState::BROKEN)
    {
        if (team == Team::BLUE)
        {
            ttype = TextureType::TILE_BLUE_BROKEN;
        }
        else
        {
            ttype = TextureType::TILE_RED_BROKEN;
        }
    }
    else if (state == TileState::EMPTY)
    {
        if (team == Team::BLUE)
        {
            ttype = TextureType::TILE_BLUE_EMPTY;
        }
        else
        {
            ttype = TextureType::TILE_RED_EMPTY;
        }
    }
    else
    {
        assert(false && "Tile in invalid state");
    }
    setTexture(*ResourceManager::GetInstance().GetTexture(ttype));
}

bool Tile::IsWalkable() const
{
    return state != TileState::BROKEN && state != TileState::EMPTY;
}

bool Tile::IsCracked() const
{
    return state == TileState::CRACKED;
}

void Tile::AddEntity(Entity* _entity)
{
    if (!ContainsEntity(_entity))
    {
        _entity->SetTile(this);
        entities.push_back(_entity);
    }
}

void Tile::RemoveEntity(Entity* _entity)
{
    auto it = find(entities.begin(), entities.end(), _entity);
    if (it != entities.end())
    {
        entities.erase(it);
    }
}

bool Tile::ContainsEntity(Entity* _entity) const
{
    return find(entities.begin(), entities.end(), _entity) != entities.end();
}

void Tile::AffectEntities(Spell* caller)
{
    for (auto it = entities.begin(); it < entities.end(); ++it)
    {
        if (*it != caller)
            caller->Attack(*it);
    }
}

bool Tile::GetNextEntity(Entity*& out) const
{
    static int x = 0;
    while( x < (int)entities.size())
    {
        out = entities.at(x);
        x++;
        return true;
    }
    x = 0;
    return false;
}

void Tile::Update(float _elapsed)
{
    vector<Entity*> copy = entities;
    for (auto it = copy.begin(); it < copy.end(); ++it)
    {
        Entity* entity = *it;
        entity->Update(_elapsed);
        if (entity->IsDeleted())
        {
            this->RemoveEntity(entity);
            field->RemoveEntity(entity);
        }
    }
    this->RefreshTexture();
}