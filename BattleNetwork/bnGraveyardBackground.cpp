#include "bnGraveyardBackground.h"
#include "bnLogger.h"
#include "bnTextureResourceManager.h"
#include "bnEngine.h"

#define X_OFFSET 0
#define Y_OFFSET 0

#define COMPONENT_FRAME_COUNT 8
#define COMPONENT_WIDTH 128
#define COMPONENT_HEIGHT 32

GraveyardBackground::GraveyardBackground(void)
  : x(0.0f), y(0.0f), progress(0.0f), Background(*TEXTURES.LoadTextureFromFile("resources/backgrounds/grave/fg.png"), 240, 180) {
  FillScreen(sf::Vector2u(COMPONENT_WIDTH, COMPONENT_HEIGHT));
}

GraveyardBackground::~GraveyardBackground(void) {
}

void GraveyardBackground::Update(float _elapsed) {
  progress += 0.02f/_elapsed;
  if (progress >= 1.f) progress = 0.0f;

  y += 0.005f/_elapsed;
  x += 0.005f/_elapsed;

  if (x > 1) x = 0;
  if (y > 1) y = 0;

  int frame = progress * COMPONENT_FRAME_COUNT;

  Wrap(sf::Vector2f(x, y));
  TextureOffset(sf::Vector2f(frame*COMPONENT_WIDTH, 0));
}