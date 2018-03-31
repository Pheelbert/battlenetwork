#pragma once
#include "bnSpell.h"

class ProgBomb : public Spell
{
public:
	ProgBomb(Field* _field, Team _team);
	virtual ~ProgBomb(void);

	virtual void Update(float _elapsed);
	virtual bool Move(Direction _direction);
	virtual void Attack(Entity* _entity);
	virtual vector<Drawable*> GetMiscComponents();
	virtual int GetStateFromString(string _string);
	virtual void addAnimation(int _state, FrameAnimation _animation, float _duration);
};