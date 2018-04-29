#include "bnTile.h"
#include "bnEntity.h"
#include "bnSpell.h"
#include "bnPlayer.h"
#include "bnTextureResourceManager.h"
#include "bnField.h"

#define START_X 0.0f
#define START_Y 144.f
#define COOLDOWN 30.f
#define FLICKER 4.0f
#define Y_OFFSET 10.0f

Tile::Tile(int _x, int _y) {
  x = _x;
  y = _y;
  if (x <= 3) {
    team = Team::BLUE;
  } else {
    team = Team::RED;
  }
  cooldown = 0.0f;
  cooldownLength = COOLDOWN;
  state = TileState::NORMAL;
  RefreshTexture();
  elapsed = 0;
  entities = vector<Entity*>();
  setScale(2.f, 2.f);
  width = getTextureRect().width * getScale().x;
  height = getTextureRect().height * getScale().y;
  setPosition(((x - 1) * width) + START_X, ((y - 1) * (height - Y_OFFSET)) + START_Y);
}

Tile::~Tile(void) {
  if (entities.size() > 0) {
    FreeClear(entities);
  }
}

void Tile::SetField(Field* _field) {
  field = _field;
}

int Tile::GetX() const {
  return x;
}

int Tile::GetY() const {
  return y;
}

Team Tile::GetTeam() const {
  return team;
}

void Tile::SetTeam(Team _team) {
  team = _team;
}

float Tile::GetWidth() const {
  return width;
}

float Tile::GetHeight() const {
  return height;
}

void Tile::SetState(TileState _state) {
  if (_state == TileState::CRACKED && (state == TileState::EMPTY || state == TileState::BROKEN)) {
    return;
  }

  if (_state == TileState::CRACKED) {
    cooldown = cooldownLength;
  }

  state = _state;
}

void Tile::RefreshTexture() {
  sf::Vector2f test = getPosition();
  if (state == TileState::NORMAL) {
    if (team == Team::BLUE) {
      textureType = TextureType::TILE_BLUE_NORMAL;
    } else {
      textureType = TextureType::TILE_RED_NORMAL;
    }
  } else if (state == TileState::CRACKED) {
    if (team == Team::BLUE) {
      textureType = TextureType::TILE_BLUE_CRACKED;
    } else {
      textureType = TextureType::TILE_RED_CRACKED;
    }
  } else if (state == TileState::BROKEN) {
    if (team == Team::BLUE) {
      textureType = ((int)(cooldown * 5) % 2 == 0 && cooldown <= FLICKER) ? TextureType::TILE_BLUE_NORMAL : TextureType::TILE_BLUE_BROKEN;
    } else {
      textureType = ((int)(cooldown * 5) % 2 == 0 && cooldown <= FLICKER) ? TextureType::TILE_RED_NORMAL : TextureType::TILE_RED_BROKEN;
    }
  } else if (state == TileState::EMPTY) {
    if (team == Team::BLUE) {
      textureType = TextureType::TILE_BLUE_EMPTY;
    } else {
      textureType = TextureType::TILE_RED_EMPTY;
    }
  } else {
    assert(false && "Tile in invalid state");
  }
  setTexture(*TextureResourceManager::GetInstance().GetTexture(textureType));
}

bool Tile::IsWalkable() const {
  return (state != TileState::BROKEN && state != TileState::EMPTY);
}

bool Tile::IsCracked() const {
  return state == TileState::CRACKED;
}

void Tile::AddEntity(Entity* _entity) {
  if (!ContainsEntity(_entity)) {
    _entity->SetTile(this);
    entities.push_back(_entity);
  }
}

void Tile::RemoveEntity(Entity* _entity) {
  auto it = find(entities.begin(), entities.end(), _entity);
  if (it != entities.end()) {
    entities.erase(it);
  }
}

bool Tile::ContainsEntity(Entity* _entity) const {
  return find(entities.begin(), entities.end(), _entity) != entities.end();
}

void Tile::AffectEntities(Spell* caller) {
  for (auto it = entities.begin(); it < entities.end(); ++it) {
    if (*it != caller && !(*it)->IsPassthrough()) {
      caller->Attack(*it);
    }
  }
}

bool Tile::GetNextEntity(Entity*& out) const {
  static int x = 0;
  while (x < (int)this->entities.size()) {
    out = this->entities.at(x);
    x++;
    return true;
  }
  x = 0;
  return false;
}

void Tile::Update(float _elapsed) {
  vector<Entity*> copy = this->entities;
  for (auto entity = copy.begin(); entity < copy.end(); ++entity) {
    (*entity)->Update(_elapsed);
  }

  this->RefreshTexture();

  if (state == TileState::BROKEN) {
    cooldown -= 0.1f;
  }

  if (cooldown <= 0.0f && state == TileState::BROKEN) {
    state = TileState::NORMAL;
  }
}