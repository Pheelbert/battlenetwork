#pragma once
#include <Thor/Animations.hpp>
using thor::FrameAnimation;
using thor::Animator;
using sf::IntRect;

#include "bnEntity.h"
#include "bnPlayerState.h"
#include "bnTextureType.h"
#include "bnPlayerHealthUI.h"
#include "bnChargeComponent.h"
#include "bnResourceComponent.h"
#include "bnControllableComponent.h"

class Player : public Entity
{
public:
    Player(void);
    virtual ~Player(void);

    virtual void Update(float _elapsed);
    virtual bool Move(Direction _direction);
    void Attack(float _charge);
    virtual vector<Drawable*> GetMiscComponents();
    void RefreshTexture();

    int GetHealth() const;
    void SetHealth(int _health);
    int Hit(int _damage);

    PlayerHealthUI* GetHealthUI() const;

    virtual int GetStateFromString(string _string);
    virtual void addAnimation(int _state, FrameAnimation _animation, float _duration);

private:
    int health;

    //Cooldowns
    float moveKeyPressCooldown;
    float moveLagCooldown;
    float attackKeyPressCooldown;
    float attackToIdleCooldown;
	float hitCoolDown;

    TextureType textureType;
    PlayerState state;
    PlayerHealthUI* healthUI;
    Animator<Sprite, PlayerState> animator;

    //-Animation-
    float animationProgress;
    
    ControllableComponent controllableComponent;
    ChargeComponent chargeComponent;
    ResourceComponent resourceComponent;
};