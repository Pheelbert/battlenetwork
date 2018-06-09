#pragma once
#include "bnAnimationComponent.h"
#include "bnAnimation.h"

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
  Entity();
  virtual ~Entity();

  virtual void Update(float _elapsed);
  virtual bool Move(Direction _direction);
  virtual vector<Drawable*> GetMiscComponents();
  virtual void AddAnimation(string _state, FrameList _frameList, float _duration);
  virtual void SetAnimation(string _state);
  virtual void SetCounterFrame(int frame);
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
  void ToggleCounter(bool on=true);
  void Stun(double maxCooldown);
  bool IsCountered();
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
  bool counterable;
  std::string name;
  double stunCooldown;
};