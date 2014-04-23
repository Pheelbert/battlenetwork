#pragma once
#include "bnEntity.h"
#include <Thor\Animation.hpp>
using thor::FrameAnimation;
using sf::Texture;

class Spell : public Entity
{
public:
	Spell(void);
	Spell(Field* _field, Team _team);
	virtual ~Spell(void);

	virtual void Update(float _elapsed)            = 0;
	virtual bool Move(Direction _direction)        = 0;
	virtual void Attack(Entity* _entity)           = 0;
	virtual vector<Drawable*> GetMiscComponents()  = 0;
    virtual int getStateFromString(string _string) = 0;
    virtual void addAnimation(int _state, FrameAnimation _animation, float _duration) = 0;

	void SetDirection(Direction _direction);
	Direction GetDirection() const;

protected:
	bool hit;
	int random;
	float cooldown;
	float damageCooldown;
	float progress;
	float hitHeight;
	Texture* texture;
	Direction direction;
	FrameAnimation animation;
};
