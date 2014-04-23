#pragma once
#include <Thor/Animation.hpp>
using thor::FrameAnimation;
using thor::Animator;
using sf::IntRect;

#include "bnMob.h"
#include "bnMobState.h"
#include "bnTextureType.h"
#include "bnMobHealthUI.h"
#include "bnResourceComponent.h"

class Mettaur : public Mob
{
public:
	Mettaur(void);
	virtual ~Mettaur(void);

	virtual void Update(float _elapsed);
	virtual bool Move(Direction _direction);
	virtual void RefreshTexture();
	virtual vector<Drawable*> GetMiscComponents();
    virtual int getStateFromString(string _string);
    virtual void addAnimation(int _state, FrameAnimation _animation, float _duration);

    void Attack();

private:
    //Cooldowns
	float cooldown;
	float attackCooldown;
	float waitCooldown;

    //Animation
	float explosionProgress;
	float explosionProgress2;
	float attackDelay;
	Sprite explosion;
	Sprite explosion2;
	FrameAnimation explode;

    ResourceComponent resourceComponent;
};