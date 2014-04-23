#include "bnField.h"
#include "bnMemory.h"

Field::Field(void)
{
}

Field::Field(int _width, int _height)
    : width(_width),
      height(_height),
      tiles(vector<vector<Tile*>>()),
      entities(vector<Entity*>())
{
	for (int y = 0; y < _height; y++)
	{
		vector<Tile*> row = vector<Tile*>();
		for (int x = 0; x < _width; x++)
		{
			Tile* tile = new Tile(x + 1, y + 1);
			tile->SetField(this);
			row.push_back(tile);
		}
		tiles.push_back(row);
	}
}

Field::~Field(void)
{
	for (size_t i = 0; i < tiles.size(); i++)
	{
		FreeClear(tiles[i]);
		tiles[i].clear();
	}
	tiles.clear();
}

int Field::GetWidth() const
{
	return width;
}

int Field::GetHeight() const
{
	return height;
}

bool Field::GetNextTile(Tile*& out)
{
	static int y = 0;
	while (y < height)
	{
		static int x = 0;
		while (x < width)
		{
			out = tiles[y][x];
			x++;
			return true;
		}
		y++;
		x = 0;
	}
	y = 0;
	return false;
}

void Field::AddEntity(Entity* _entity, int x, int y)
{
	entities.push_back(_entity);
	_entity->SetField(this);
	GetAt(x, y)->AddEntity(_entity);
}

void Field::RemoveEntity(Entity* _entity)
{
	auto it = find(entities.begin(), entities.end(), _entity);
	if (it != entities.end())
	{
		if (_entity->GetTile())
		{
			GetAt(_entity->GetTile()->GetX(), _entity->GetTile()->GetY())->RemoveEntity(_entity);
		}
		delete *it;
		entities.erase(it);
	}
}

Tile* Field::FindEntity(Entity* _entity) const
{
	for (int y = 1; y <= height; y++)
	{
		for (int x = 1; x <= width; x++)
		{
			Tile* tile = GetAt(x, y);
			if (tile->ContainsEntity(_entity))
			{
				return tile;
			}
		}
	}
	return nullptr;
}

bool Field::GetNextEntity(Entity*& out, int _depth) const
{
	static int i = 0;
	for (i; i < (int)entities.size(); i++)
	{
		if (entities.at(i)->GetTile()->GetY() == _depth)
		{
			out = entities.at(i++);
			return true;
		}
	}
	i = 0;
	return false;
}

void Field::SetAt(int _x, int _y, Team _team)
{
	tiles[_y - 1][_x - 1]->SetTeam(_team);
}

Tile* Field::GetAt(int _x, int _y) const
{
	return tiles[_y - 1][_x - 1];
}

void Field::Update(float _elapsed)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tiles[y][x]->Update(_elapsed);
		}
	}
}