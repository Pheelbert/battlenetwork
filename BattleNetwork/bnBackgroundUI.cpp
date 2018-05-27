#include "bnBackgroundUI.h"
#include "bnLogger.h"
#include "bnTextureResourceManager.h"
#include "bnEngine.h"

#define X_OFFSET 15
#define Y_OFFSET 15

#define COMPONENT_FRAME_COUNT 4
#define COMPONENT_WIDTH 32
#define COMPONENT_HEIGHT 32

BackgroundUI::BackgroundUI(void)
  : progress(0.0f) {
  bgTexture = TEXTURES.LoadTextureFromFile("resources/backgrounds/green/bg_green.png");
  background.setTexture(*bgTexture);
  background.setScale(2.f, 2.f);
  cmTexture = TEXTURES.LoadTextureFromFile("resources/backgrounds/green/fg_green.png");
  component.setTexture(*cmTexture);
  for (int x = 0; x < COMPONENT_FRAME_COUNT; x++) {
    float relative = 0.0f;
    (x == COMPONENT_FRAME_COUNT - 1) ? relative = 2.f : relative = 0.3f;
    animation.addFrame(relative, IntRect(COMPONENT_WIDTH*x, 0, COMPONENT_WIDTH, COMPONENT_HEIGHT));
  }
}

BackgroundUI::~BackgroundUI(void) {
}

void BackgroundUI::Draw() {
  progress += 0.01f;
  if (progress >= 1.f) progress = 0.0f;
  static float mx = 0.0f, my = 0.0f;
  bool ySwitch = false, xSwitch = false;;
  ENGINE.Draw(background);
  for (int y = -10; y <= 10; y++) {
    for (int x = -10; x <= 15; x++) {
      if (xSwitch && ySwitch) {
        component.setPosition(x*32.f + x * X_OFFSET + mx, y*32.f + y * Y_OFFSET + my);
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