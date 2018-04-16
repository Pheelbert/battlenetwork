#pragma once
#include <string>
using std::string;
using std::to_string;

class Entity;

#define ANIMATION_EXTENSION ".animation"

class ResourceComponent {
public:
  ResourceComponent(Entity* _entity);
  ~ResourceComponent();

  void setup(string _name, string _path);
  void load();
  string valueOf(string _key, string _line);

private:
  Entity * entity;
  string name;
  string path;
};