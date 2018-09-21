#pragma once

#include "bnField.h"
#include "bnTile.h"
#include "bnDirection.h"

class TileBehavior {
private:
  sf::Vector2f displacement;
  double slideSpeed;

  Battle::Tile* next;
  Battle::Tile* tile;
  Battle::Tile* previous;
  Entity* owner;
  Field* field;
  int moveCount;

  bool isFloatShoe;
  bool isLavaResistant;
  bool isPoisonResistant;
  bool isIceResistant;
  bool isGrassResistant;

public:
  TileBehavior(Entity* _owner) {
    owner = _owner;
    field = owner->GetField();
    tile = owner->GetTile();
    moveCount = 0;
  }

  void Update(double elapsed) {

  }

  void BeginSlide() {

  }


  bool Move(Direction _direction) {
    bool moved = false;
    Battle::Tile* temp = tile;
    if (_direction == Direction::UP) {
      if (tile->GetY() - 1 > 0) {
        next = field->GetAt(tile->GetX(), tile->GetY() - 1);
        if (owner->Teammate(next->GetTeam()) && next->IsWalkable()) {
          ;
        }
        else {
          next = nullptr;
        }
      }
    }
    else if (_direction == Direction::LEFT) {
      if (tile->GetX() - 1 > 0) {
        next = field->GetAt(tile->GetX() - 1, tile->GetY());
        if (owner->Teammate(next->GetTeam()) && next->IsWalkable()) {
          ;
        }
        else {
          next = nullptr;
        }
      }
    }
    else if (_direction == Direction::DOWN) {
      if (tile->GetY() + 1 <= (int)field->GetHeight()) {
        next = field->GetAt(tile->GetX(), tile->GetY() + 1);
        if (owner->Teammate(next->GetTeam()) && next->IsWalkable()) {
          ;
        }
        else {
          next = nullptr;
        }
      }
    }
    else if (_direction == Direction::RIGHT) {
      if (tile->GetX() + 1 <= static_cast<int>(field->GetWidth())) {
        next = field->GetAt(tile->GetX() + 1, tile->GetY());
        if (owner->Teammate(next->GetTeam()) && next->IsWalkable()) {
          ;
        }
        else {
          next = nullptr;
        }
      }
    }

    if (next) {
      previous = temp;
      moved = true;
    }
    return moved;
  }

  void AdoptNextTile() {
    owner->SetTile(next);
    tile->AddEntity(owner);
    previous->RemoveEntity(owner);
    previous = nullptr;
    next = nullptr;
    moveCount++;
  }

  const Battle::Tile* GetTile() const { return tile; }
  const int GetMoveCount() const { return moveCount; }
};