#include <random>
#include <time.h>

#include "bnBuster.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnMob.h"
#include "bnResourceManager.h"

#define COOLDOWN 40.0f
#define DAMAGE_COOLDOWN 50.0f

#define BULLET_ANIMATION_SPRITES 3
#define BULLET_ANIMATION_WIDTH 30
#define BULLET_ANIMATION_HEIGHT 27

Buster::Buster(Field* _field, Team _team, bool _charged)
{
    cooldown = 0;
    damageCooldown = 0;
    field = _field;
    team = _team;
    direction = Direction::NONE;
    deleted = false;
    hit = false;
    progress = 0.0f;
    hitHeight = 0.0f;
    srand((unsigned int)time(nullptr));
    random = rand() % 20 - 20;
    if (_charged)
    {
        damage = 10;
        //TODO: make new sprite animation for charged bullet
        texture = ResourceManager::GetInstance().GetTexture(TextureType::SPELL_BULLET_HIT);
    }
    else
    {
        damage = 1;
        texture = ResourceManager::GetInstance().GetTexture(TextureType::SPELL_BULLET_HIT);
    }
    setScale(2.f, 2.f);
    for (int x = 0; x < BULLET_ANIMATION_SPRITES; x++)
    {
        animation.addFrame(0.3f, IntRect(BULLET_ANIMATION_WIDTH*x, 0, BULLET_ANIMATION_WIDTH, BULLET_ANIMATION_HEIGHT));
    }
}

Buster::~Buster(void)
{
}

void Buster::Update(float _elapsed)
{
    if (hit)
    {
        if (progress == 0.0f)
        {
            setTexture(*texture);
            setPosition(tile->getPosition().x + tile->GetWidth()/2.f + random, tile->getPosition().y + tile->GetHeight()/2.f - hitHeight);
        }
        progress += 0.2f;
        animation(*this, progress);
        if (progress >= 1.f)
        {
            deleted = true;
        }
        return;
    }

    damageCooldown += _elapsed;
    if (damageCooldown >= DAMAGE_COOLDOWN)
    {
        tile->AffectEntities(this);
        damageCooldown = 0;
    }

    cooldown += _elapsed;
    if (cooldown >= COOLDOWN)
    {
        Move(direction);
        cooldown = 0;
    }
}

bool Buster::Move(Direction _direction)
{
    tile->RemoveEntity(this);
    Tile* next = nullptr;
    if (_direction == Direction::UP)
    {
        if (tile->GetY() - 1 > 0)
        {
            next = field->GetAt(tile->GetX(), tile->GetY() - 1);
            SetTile(next);
        }
    }
    else if (_direction == Direction::LEFT)
    {
        if (tile->GetX() - 1 > 0)
        {
            next = field->GetAt(tile->GetX() - 1, tile->GetY());
            SetTile(next);
        }
        else
        {
            deleted = true;
            return false;
        }
    }
    else if (_direction == Direction::DOWN)
    {
        if (tile->GetY() + 1 <= (int)field->GetHeight())
        {
            next = field->GetAt(tile->GetX(), tile->GetY() + 1);
            SetTile(next);
        }
    }
    else if (_direction == Direction::RIGHT)
    {
        if (tile->GetX() + 1 <= (int)field->GetWidth())
        {
            next = field->GetAt(tile->GetX() + 1, tile->GetY());
            SetTile(next);
        }
        else
        {
            deleted = true;
            return false;
        }
    }
    tile->AddEntity(this);
    return true;
}

void Buster::Attack(Entity* _entity)
{
    Player* isPlayer = dynamic_cast<Player*>(_entity);
    if (isPlayer)
    {
        isPlayer->Hit(damage);
        hitHeight = isPlayer->getLocalBounds().height * isPlayer->getScale().y - 20.0f + random/2;
        hit = true;
        return;
    }
    Mob* isMob = dynamic_cast<Mob*>(_entity);
    if (isMob)
    {
        isMob->Hit(damage);
        hitHeight = isMob->GetHitHeight();
        hit = true;
        return;
    }
}

vector<Drawable*> Buster::GetMiscComponents()
{
    return vector<Drawable*>();
}

int Buster::getStateFromString(string _string)
{
    assert(false && "Buster does not have states");
    return 0;
}

void Buster::addAnimation(int _state, FrameAnimation _animation, float _duration)
{
    //animator.addAnimation(static_cast<Buster>(_state), _animation, sf::seconds(_duration));
    assert(false && "Buster does not have an animator");
}