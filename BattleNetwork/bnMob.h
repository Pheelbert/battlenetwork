#pragma once
#include <Thor/Animation.hpp>
using thor::FrameAnimation;
using thor::Animator;
using sf::IntRect;

#include "bnEntity.h"
#include "bnMobState.h"
#include "bnTextureType.h"
#include "bnMobHealthUI.h"

class Mob : public Entity
{
public:
    Mob(void)
    {
        SetLayer(0);
    }

    virtual ~Mob(void)
    {
        delete healthUI;
    }

    virtual void Update(float _elapsed)            = 0;
    virtual bool Move(Direction _direction)        = 0;
    virtual vector<Drawable*> GetMiscComponents()  = 0;
    virtual void RefreshTexture()                  = 0;
    virtual int getStateFromString(string _string) = 0;
    virtual void addAnimation(int _state, FrameAnimation _animation, float _duration) = 0;

    TextureType GetTextureType() const
    {
        return ttype;
    }

    MobState GetMobState() const
    {
        return state;
    }

    int GetHealth() const
    {
        return health;
    }

    void SetHealth(int _health)
    {
        health = _health;
    }

    int Hit(int _damage)
    {
        (health - _damage < 0) ? health = 0 : health -= _damage;
        return health;
    }

    float GetHitHeight() const
    {
        return hitHeight;
    }

protected:
    int health;
    float hitHeight;
    Direction direction;
    MobState state;
    TextureType ttype;
    MobHealthUI* healthUI;
    Animator<Sprite, MobState> animator;
};