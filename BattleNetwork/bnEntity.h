#pragma once
#include <string>
using std::string;

#include "bnAnimation.h"
#include "bnDirection.h"
#include "bnTeam.h"
#include "bnMemory.h"
#include "bnEngine.h"
#include "bnTextureType.h"
class Tile;
class Field;

class Entity : public LayeredDrawable {
  friend class Field;

public:
  Entity();
  virtual ~Entity();

  virtual const bool Hit(int damage);
  virtual const float GetHitHeight() const;
  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction);
  virtual vector<Drawable*> GetMiscComponents();
  virtual void SetAnimation(string _state);
  virtual TextureType GetTextureType();

  bool Teammate(Team _team);

  void SetTile(Tile* _tile);
  Tile* GetTile() const;

  void SetField(Field* _field);
  Field* GetField() const;

  Team GetTeam() const;
  void SetTeam(Team _team);

  void SetPassthrough(bool state);
  bool IsPassthrough();

  void Delete();
  bool IsDeleted() const;

protected:
  bool ownedByField;
  Tile* next;
  Tile* tile;
  Tile* previous;
  Field* field;
  Team team;
  bool passthrough;
  bool deleted;
};