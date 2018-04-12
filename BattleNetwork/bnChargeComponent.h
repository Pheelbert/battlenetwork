#pragma once
#include <SFML/Graphics.hpp>
using sf::CircleShape;
using sf::Sprite;
using sf::Drawable;
using sf::Texture;
#include <Thor/Animations.hpp>
using thor::FrameAnimation;
using thor::Animator;
using sf::IntRect;
class Entity;

#define CHARGE_TEXTURE "resources/spells/spell_buster_charge.png"
#define CHARGE_COUNTER_MIN 400.0f
#define BLUE_CHARGE_FRAME_COUNT 8
#define CHARGE_COUNTER_MAX 2400.0f
#define PURPLE_CHARGE_FRAME_COUNT 16
#define CHARGE_WIDTH 65
#define CHARGE_HEIGHT 65

/*!
 * @brief For player only right now
*/
class ChargeComponent
{
public:
    ChargeComponent(Entity* _entity);
    ~ChargeComponent();

    void load();
    void update(float _elapsed);
    void SetCharging(bool _charging);
    float GetChargeCounter() const;
    Sprite& GetSprite();

private:
    Entity* entity;
    bool charging;
	bool isCharged;
	bool isPartiallyCharged;
    float chargeCounter;
    float animationProgress;
    Texture chargeTexture;
    Sprite chargeSprite;
    FrameAnimation blueChargeAnimation;
    FrameAnimation purpleChargeAnimation;
};