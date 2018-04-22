#pragma once
#include <string>
#include <Thor/Animations.hpp>
using std::string;
using std::to_string;
using thor::Animator;
using thor::AnimationMap;
class Entity;

#define ANIMATION_EXTENSION ".animation"

class AnimationComponent {
public:
  AnimationComponent(Entity* _entity);
  ~AnimationComponent();

  void setup(string _name, string _path);
  void load();
  void update(float _elapsed);
  void setAnimation(int state, std::function<void()> onFinish = nullptr);
  string valueOf(string _key, string _line);

private:
  Entity * entity;
  string name;
  string path;
  AnimationMap<sf::Sprite, int> animations;
  Animator<sf::Sprite, int>* animator;
};