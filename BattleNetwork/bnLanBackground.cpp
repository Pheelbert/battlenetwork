#include "bnLanBackground.h"
#include "bnLogger.h"
#include "bnTextureResourceManager.h"
#include "bnEngine.h"

#define X_OFFSET 15
#define Y_OFFSET 15

#define COMPONENT_FRAME_COUNT 4
#define COMPONENT_WIDTH 64
#define COMPONENT_HEIGHT 64

LanBackground::LanBackground(void)
  : x(0.0f), y(0.0f), progress(0.0f), Background(*TEXTURES.LoadTextureFromFile("resources/backgrounds/lan/bg.png"), 240, 180) {
  FillScreen(sf::Vector2u(COMPONENT_WIDTH, COMPONENT_HEIGHT));
}

LanBackground::~LanBackground(void) {
}

void LanBackground::Update(float _elapsed) {
  progress += 1 * _elapsed;
  if (progress >= 1.f) progress = 0.0f;

  y -= 0.3 * _elapsed;
  x -= 0.3 * _elapsed;

  if (x < 0) {
    x = 1;
  }

  if (y < 0) {
    y = 1;
  }

  int frame = progress * COMPONENT_FRAME_COUNT;

  Wrap(sf::Vector2f(x, y));
  TextureOffset(sf::Vector2f(frame*COMPONENT_WIDTH, 0));
}