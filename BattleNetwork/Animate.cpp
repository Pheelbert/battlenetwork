#include "bnAnimate.h"

void Animate::operator() (float progress, Entity& target, FrameList& sequence, std::function<void()> callback) const
{
  assert(!sequence.IsEmpty());

  bool applyCallback = (progress > sequence.totalDuration);

  if (applyCallback) {
    if (callback) {
      callback();
    }
  }

  for (Frame& frame : sequence.frames) {
    progress -= frame.duration;

    // Must be <= and not <, to handle case (progress == frame.duration) correctly
    if (progress <= 0.f || &frame == &sequence.frames.back())
    {
      target.setTextureRect(frame.subregion);
      if (frame.applyOrigin) {
        target.setOrigin((float)frame.origin.x, (float)frame.origin.y);
      }

      break;
    }
  }
}