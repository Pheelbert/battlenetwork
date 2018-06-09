#include "bnAnimate.h"

Animate::Animate() {
  this->onFinish = nullptr;
}

Animate::Animate(Animate& rhs) {
  this->onFinish = rhs.onFinish;
  this->callbacks = rhs.callbacks;
}

Animate::~Animate() {
  this->callbacks.clear();
  this->onFinish = nullptr;
}

void Animate::operator() (float progress, sf::Sprite& target, FrameList& sequence) const
{
  if (sequence.frames.empty()) {
    if (onFinish != nullptr) {
      onFinish();
    }
    return;
  }

  bool applyCallback = (progress > sequence.totalDuration);

  if (applyCallback) {
    if (onFinish != nullptr) {
      onFinish();
    }
  }

  int index = 0;
  for (Frame& frame : sequence.frames) {
    index++;
    progress -= frame.duration;

    // Must be <= and not <, to handle case (progress == frame.duration) correctly
    if (progress <= 0.f || &frame == &sequence.frames.back())
    {
      std::map<int, std::function<void()>>::const_iterator iter = this->callbacks.find(index - 1);

      if (iter != this->callbacks.end()) {
        iter->second();
      }

      target.setTextureRect(frame.subregion);
      if (frame.applyOrigin) {
        target.setOrigin((float)frame.origin.x, (float)frame.origin.y);
      }

      break;
    }
  }
}

Animate & Animate::operator<<(On& rhs)
{
  this->callbacks.insert(std::make_pair(rhs.id, rhs.callback));

  return *this;
}

void Animate::operator<<(std::function<void()> finishNotifier)
{
  this->onFinish = finishNotifier;
}

void Animate::SetFrame(int frameIndex, sf::Sprite & target, FrameList& sequence) const
{
  int index = 0;
  for (Frame& frame : sequence.frames) {
    index++;

    if (index == frameIndex) {
      target.setTextureRect(frame.subregion);
      if (frame.applyOrigin) {
        target.setOrigin((float)frame.origin.x, (float)frame.origin.y);
      }

      return;
    }
  }
}
