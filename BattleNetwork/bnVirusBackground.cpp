#include "bnVirusBackground.h"
#include "bnLogger.h"
#include "bnTextureResourceManager.h"
#include "bnEngine.h"

#define X_OFFSET 0
#define Y_OFFSET 0

#define COMPONENT_FRAME_COUNT 8
#define COMPONENT_WIDTH 128
#define COMPONENT_HEIGHT 128

VirusBackground::VirusBackground(void)
  : progress(0.0f), Background() {

  cmTexture = TEXTURES.LoadTextureFromFile("resources/backgrounds/virus/fg.png");
  component.setTexture(*cmTexture);
  for (int x = 0; x < COMPONENT_FRAME_COUNT; x++) {
    float relative = 0.0f;
    (x == COMPONENT_FRAME_COUNT - 1) ? relative = 5.f : relative = 1.f;

    animation.addFrame(relative, IntRect(COMPONENT_WIDTH*x, 0, COMPONENT_WIDTH, COMPONENT_HEIGHT));
  }
}

VirusBackground::~VirusBackground(void) {
}

void VirusBackground::Draw() {
  progress += 0.01f;
  if (progress >= 1.f) progress = 0.0f;
  static float mx = 0.0f, my = 0.0f;
  bool ySwitch = false, xSwitch = false;;
  for (int y = -10; y <= 9; y++) {
    for (int x = -10; x <= 14; x++) {
      if (xSwitch && ySwitch) {
        component.setPosition(x*128.f + x * X_OFFSET + mx, y*128.f + y * Y_OFFSET);
        component.setScale(2.f, 2.f);
        animation(component, progress);
        ENGINE.Draw(component);
      }
      xSwitch = !xSwitch;
    }
    ySwitch = !ySwitch;
  }

  my += 0.5f;
  mx += 0.5f;

  if (mx >= 470.0f) {
    mx = 0;
  }
  if (my >= 283.f) {
    my = 0;
  }
}