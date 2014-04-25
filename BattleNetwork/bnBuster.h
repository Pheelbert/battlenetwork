#pragma once
#include "bnSpell.h"

class Buster : public Spell
{
public:
    Buster(Field* _field, Team _team, bool _charged);
    virtual ~Buster(void);

    virtual void Update(float _elapsed);
    virtual bool Move(Direction _direction);
    virtual void Attack(Entity* _entity);
    virtual vector<Drawable*> GetMiscComponents();
    virtual int GetStateFromString(string _string);
    virtual void addAnimation(int _state, FrameAnimation _animation, float _duration);

private:
    int damage;
};