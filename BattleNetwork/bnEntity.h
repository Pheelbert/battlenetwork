#pragma once
#include <Thor/Animations.hpp>
using thor::FrameAnimation;
#include <string>
using std::string;

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
  Entity(void);
  virtual ~Entity(void);

  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction);
  virtual vector<Drawable*> GetMiscComponents();
  virtual int GetStateFromString(string _string);
  virtual void AddAnimation(int _state, FrameAnimation _animation, float _duration);
  virtual void SetAnimation(int _state);
  virtual int GetHealth();
  virtual TextureType GetTextureType();
  virtual int* GetAnimOffset();

  bool Teammate(Team _team);

  void SetTile(Tile* _tile);
  Tile* GetTile() const;

  void SetField(Field* _field);
  Field* GetField() const;

  Team GetTeam() const;
  void SetTeam(Team _team);

  void SetHealth(int _health);

  void SetPassthrough(bool state);
  bool IsPassthrough();
  bool IsDeleted() const;
  void TryDelete();

  // For mob UI
  const std::string GetName() const;

protected:
  bool ownedByField;
  Tile* next;
  Tile* tile;
  Tile* previous;
  Field* field;
  Team team;
  int health;
  bool passthrough;
  bool deleted;
  std::string name;
};