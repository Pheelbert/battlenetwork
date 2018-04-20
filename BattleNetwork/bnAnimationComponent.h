#pragma once
#include <string>
using std::string;
using std::to_string;

class Entity;

#define ANIMATION_EXTENSION ".animation"

class AnimationComponent {
public:
  AnimationComponent(Entity* _entity);
  ~AnimationComponent();

  void setup(string _name, string _path);
  void load();
  string valueOf(string _key, string _line);

private:
  Entity * entity;
  string name;
  string path;
};