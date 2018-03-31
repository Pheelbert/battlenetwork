#include "bnProgBomb.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"

ProgBomb::ProgBomb(Field* _field, Team _team)
{
	SetLayer(0);
	cooldown = 0;
	damageCooldown = 0;
	field = _field;
	team = _team;
	direction = Direction::NONE;
	deleted = false;
	hit = false;
	texture = TextureResourceManager::GetInstance().GetTexture(TextureType::SPELL_PROG_BOMB);

	progress = 0.0f;
	hitHeight = 0.0f;
	random = 0;

	animation.addFrame(0.3f, IntRect(0, 0, 19, 41));


	AudioResourceManager::GetInstance().Play(AudioType::TOSS_ITEM);
}

ProgBomb::~ProgBomb(void)
{
}

/* Have a target tile in mind
	calculate the time to the tile
	follow arc path to the tile from start height over time
	at time x (where x is the end of the arc), attack tile
	explode
*/
void ProgBomb::Update(float _elapsed)
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

	// When at the end of the arc
	// TODO: tile->AffectEntities(this);
}

bool ProgBomb::Move(Direction _direction)
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

void ProgBomb::Attack(Entity* _entity)
{
	Player* isPlayer = dynamic_cast<Player*>(_entity);
	if (isPlayer)
	{
		isPlayer->Hit(20);
		deleted = true;
		return;
	}
}

vector<Drawable*> ProgBomb::GetMiscComponents()
{
	return vector<Drawable*>();
}

int ProgBomb::GetStateFromString(string _string)
{
	assert(false && "Wave does not have states");
	return 0;
}

void ProgBomb::addAnimation(int _state, FrameAnimation _animation, float _duration)
{
	assert(false && "Wave does not have an animator");
}