#pragma once
#include "bnSpell.h"

class ProgBomb : public Spell
{
private:
	Tile* target;
	double posX;
	double posY;
	double velX;
	double velY;
	double arcDuration;
	double arcProgress;
public:
	ProgBomb(Field* _field, Team _team, Tile* _target, double _duration);
	virtual ~ProgBomb(void);

	virtual void Update(float _elapsed);
	virtual bool Move(Direction _direction);
	virtual void Attack(Entity* _entity);
	virtual vector<Drawable*> GetMiscComponents();
	virtual int GetStateFromString(string _string);
	virtual void addAnimation(int _state, FrameAnimation _animation, float _duration);

	void PrepareThrowPath();
};