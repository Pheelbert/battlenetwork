#include "bnProgBomb.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"

#define GRAVITY 9.81

ProgBomb::ProgBomb(Field* _field, Team _team, Tile* _target, double _duration)
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

	arcDuration = _duration;
	arcProgress = 0;
	target = _target;

	setOrigin(sf::Vector2f(19, 24) / 2.f);
	AudioResourceManager::GetInstance().Play(AudioType::TOSS_ITEM);
}

ProgBomb::~ProgBomb(void)
{
}

void ProgBomb::PrepareThrowPath() {
	velX = velY = 0;
	posX = tile->getPosition().x;
	posY = tile->getPosition().y;

	// Calculate projectile variables on init 
	velX = (target->getPosition().x - posX) / arcDuration;
	velY = -1.0 * (abs(target->getPosition().y + 0.5*GRAVITY*arcDuration*arcDuration - posY)) / arcDuration;
	arcProgress = 0;
}

void ProgBomb::Update(float _elapsed)
{
	arcProgress += _elapsed/1000.0; // convert from ms to s

	if (!tile->IsWalkable())
	{
		deleted = true;
		return;
	}

	setTexture(*texture);
	setScale(2.f, 2.f);
	setPosition(tile->getPosition().x + 5.f, tile->getPosition().y - 50.0f);
	setRotation(-(arcProgress/arcDuration)*45.0f);
	progress += 0.05f;
	if (progress < 1.f)
	{
		animation(*this, progress);
	}

	/* Have a target tile in mind
	calculate the time to the tile
	follow arc path to the tile from start height over time
	at time x (where x is the end of the arc), attack tile
	explode
	*/
	sf::Vector2f pos(getPosition());

	pos.x = velX * arcProgress + pos.x;
	pos.y = 0.5 * GRAVITY * arcProgress * arcProgress + velY * arcProgress + pos.y;

	setPosition(pos);

	// When at the end of the arc
	if (arcProgress >= arcDuration) {
		// update tile to target tile 
		target->AffectEntities(this);
		deleted = true;
	}
}

bool ProgBomb::Move(Direction _direction)
{
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