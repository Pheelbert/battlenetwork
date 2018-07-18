#pragma once

class Entity;

/* 
Components need to be genric enough to be used by all in-battle entities
*/

class Component {
public:
  Component() = default;
  ~Component() = default;
  Component(Component&& rhs) = delete;
  Component(const Component& rhs) = delete;

  virtual void Update(float _elapsed, Entity& context) = 0;
};