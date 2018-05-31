#pragma once

#include <SFML/Graphics.hpp>
#include "bnEntity.h"

struct Frame {
  float duration;
  sf::IntRect subregion;
  bool applyOrigin;
  sf::Vector2i origin;
};

class FrameList {
  std::vector<Frame> frames;
  float totalDuration;

public:
  friend class Animate;

  FrameList() { totalDuration = 0; }
  FrameList(const FrameList& rhs) { frames = rhs.frames; totalDuration = rhs.totalDuration; }

  void Add(float dur, sf::IntRect sub) {
    frames.push_back({ dur, sub, false, sf::Vector2i(0,0) });
    totalDuration += dur;
  }

  void Add(float dur, sf::IntRect sub, sf::Vector2i origin) {
    frames.push_back({ dur, sub, true, origin });
    totalDuration += dur;
  }

  const float GetTotalDuration() { return totalDuration; }

  const bool IsEmpty() const { return frames.empty(); }
};

class Animate {
public:
  void operator() (float progress, Entity& target, FrameList& sequence, std::function<void()> callback = nullptr) const;
};