#pragma once
#include <string>
#include <assert.h>
#include <functional>

#include <iostream>

#include "bnAnimate.h"

using std::string;
using std::to_string;

#define ANIMATION_EXTENSION ".animation"

class Animation {
public:
  Animation();
  Animation(string _name, string _path);
  ~Animation();

  void Load();
  void Update(float _elapsed, sf::Sprite* target);
  void SetFrame(int frame, sf::Sprite* target);
  void SetAnimation(string state);

  Animation& operator<<(Animate::On& rhs);
  Animation& operator<<(Animate::Mode& rhs);
  void operator<<(std::function<void()> onFinish);

private:
  string ValueOf(string _key, string _line);
protected:
  Animate animator;

  string name;
  string path;
  string currAnimation;
  float progress;
  std::map<int, sf::Sprite> textures;
  std::map<string, FrameList> animations;
};