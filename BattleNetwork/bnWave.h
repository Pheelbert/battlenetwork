#pragma once
#include "bnSpell.h"

class Wave : public Spell
{
public:
	Wave(Field* _field, Team _team);
	virtual ~Wave(void);

	virtual void Update(float _elapsed);
	virtual bool Move(Direction _direction);
	virtual void Attack(Entity* _entity);
	virtual vector<Drawable*> GetMiscComponents();
    virtual int getStateFromString(string _string);
    virtual void addAnimation(int _state, FrameAnimation _animation, float _duration);
};