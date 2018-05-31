#pragma once
#include <string>
#include <assert.h>
#include <functional>

#include <iostream>

#include "bnEntity.h"
#include "bnAnimate.h"

using std::string;
using std::to_string;

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
  int currAnimationID;
  std::function<void()> finishCallback;
  float progress;
  std::map<int, sf::Sprite> textures;
  std::map<int, FrameList> animations;
  Animate animator;
};