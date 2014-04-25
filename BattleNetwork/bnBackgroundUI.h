#pragma once
#include <SFML\Graphics.hpp>
using sf::Texture;
using sf::Sprite;
using sf::IntRect;
using sf::Drawable;
#include <Thor/Animation.hpp>
using thor::FrameAnimation;
#include <vector>
using std::vector;

class BackgroundUI
{
public:
    BackgroundUI(void);
    ~BackgroundUI(void);

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