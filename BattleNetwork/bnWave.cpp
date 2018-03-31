#include "bnWave.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnMettaur.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"

#define COOLDOWN 250.0f
#define DAMAGE_COOLDOWN 250.0f

#define WAVE_ANIMATION_SPRITES 5
#define WAVE_ANIMATION_WIDTH 41
#define WAVE_ANIMATION_HEIGHT 46

Wave::Wave(Field* _field, Team _team)
{
    SetLayer(0);
    cooldown = 0;
    damageCooldown = 0;
    field = _field;
    team = _team;
    direction = Direction::NONE;
    deleted = false;
    hit = false;
    texture = TextureResourceManager::GetInstance().GetTexture(TextureType::SPELL_WAVE);
    for (int x = 0; x < WAVE_ANIMATION_SPRITES; x++)
    {
        animation.addFrame(0.3f, IntRect(WAVE_ANIMATION_WIDTH*x, 0, WAVE_ANIMATION_WIDTH, WAVE_ANIMATION_HEIGHT));
    }
    progress = 0.0f;
    hitHeight = 0.0f;
    random = 0;

	AudioResourceManager::GetInstance().Play(AudioType::WAVE);
}

Wave::~Wave(void)
{
}

void Wave::Update(float _elapsed)
{
    if (!tile->IsWalkable())
    {
        deleted = true;
        return;
    }

    setTexture(*texture);
    setScale(2.f, 2.f);
    setPosition(tile->getPosition().x + 5.f, tile->getPosition().y - 50.0f);
    progress += 0.05f;
    if (progress < 1.f)
    {
        animation(*this, progress);
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
		AudioResourceManager::GetInstance().Play(AudioType::WAVE);
        cooldown = 0;
        progress = 0.0f;
    }
}

bool Wave::Move(Direction _direction)
{
    tile->RemoveEntity(this);
    Tile* next = nullptr;
    if (_direction == Direction::LEFT)
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

void Wave::Attack(Entity* _entity)
{
    Player* isPlayer = dynamic_cast<Player*>(_entity);
    if (isPlayer)
    {
        isPlayer->Hit(10);
        deleted = true;
        return;
    }
    /*Mettaur* isMob = dynamic_cast<Mettaur*>(_entity);
    if (isMob)
    {
        isMob->Hit(10);
        deleted = true;
        return;
    }*/
}

vector<Drawable*> Wave::GetMiscComponents()
{
    return vector<Drawable*>();
}

int Wave::GetStateFromString(string _string)
{
    assert(false && "Wave does not have states");
    return 0;
}

void Wave::addAnimation(int _state, FrameAnimation _animation, float _duration)
{
    //animator.addAnimation(static_cast<Buster>(_state), _animation, sf::seconds(_duration));
    assert(false && "Wave does not have an animator");
}