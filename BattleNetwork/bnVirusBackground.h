#pragma once
#include <SFML\Graphics.hpp>
using sf::Texture;
using sf::Sprite;
using sf::IntRect;
using sf::Drawable;
#include <Thor/Animations.hpp>
using thor::FrameAnimation;
#include <vector>
using std::vector;

#include "bnBackground.h"

class VirusBackground : public Background
{
public:
  VirusBackground();
  ~VirusBackground();

  void Draw();

private:
  Sprite component;
  Sprite background;
  Texture* bgTexture;
  Texture* cmTexture;
  FrameAnimation animation;

  //Animation
  float progress;
};

