#pragma once
#include <string>
#include <assert.h>
#include <functional>

#include <iostream>

#include "bnEntity.h"

using std::string;
using std::to_string;

#define ANIMATION_EXTENSION ".animation"

struct Frame {
  float duration;
  sf::IntRect subregion;
  bool applyOrigin;
  sf::Vector2f origin;
};

class FrameList {
  std::vector<Frame> frames; 
  float totalDuration;

public:
  friend class Animate;

  FrameList() { totalDuration = 0; }
  FrameList(const FrameList& rhs) { frames = rhs.frames; totalDuration = rhs.totalDuration; }

  void Add(float dur, sf::IntRect sub) {
    frames.push_back({ dur, sub, false, sf::Vector2f(0,0)});
    totalDuration += dur;
  }

  void Add(float dur, sf::IntRect sub, sf::Vector2f origin) {
    frames.push_back({ dur, sub, true, origin });
    totalDuration += dur;
  }

  const float GetTotalDuration() { return totalDuration;  }

  const bool IsEmpty() const { return frames.empty(); }
};

class Animate {
public:
  void operator() (float progress, Entity& target, FrameList& sequence, std::function<void()> callback = nullptr) const
  {
    assert(!sequence.IsEmpty());

    bool applyCallback = (progress > sequence.totalDuration);

    if (applyCallback) {
      if (callback) {
        callback();
      }
    }

    for(Frame& frame : sequence.frames) {
      progress -= frame.duration;

      // Must be <= and not <, to handle case (progress == frame.duration) correctly
      if (progress <= 0.f || &frame == &sequence.frames.back())
      {
        target.setTextureRect(frame.subregion);
        if (frame.applyOrigin)
          target.setOrigin(frame.origin);

        break;
      }
    }
  }
};

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