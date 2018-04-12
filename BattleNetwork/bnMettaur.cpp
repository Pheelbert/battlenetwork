#include "bnMettaur.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnWave.h"
#include "bnTextureResourceManager.h"
#include "bnEngine.h"

#define RESOURCE_NAME "mettaur"
#define RESOURCE_PATH "resources/mobs/mettaur/mettaur.animation"

#define COOLDOWN 1000.0f
#define ATTACK_COOLDOWN 2222.f
#define WAIT_COOLDOWN 500.0f
#define ATTACK_DELAY 450.0f

#define MOVING_ANIMATION_SPRITES 2
#define MOVING_ANIMATION_WIDTH 32
#define MOVING_ANIMATION_HEIGHT 41

#define IDLE_ANIMATION_WIDTH 54
#define IDLE_ANIMATION_HEIGHT 56

#define HIT_ANIMATION_SPRITES 3
#define HIT_ANIMATION_WIDTH 34
#define HIT_ANIMATION_HEIGHT 31

#define ATTACK_ANIMATION_SPRITES 9
#define ATTACK_ANIMATION_WIDTH 53
#define ATTACK_ANIMATION_HEIGHT 56

#define EXPLODE_ANIMATION_SPRITES 16
#define EXPLODE_ANIMATION_WIDTH 59
#define EXPLODE_ANIMATION_HEIGHT 59

vector<int> Mettaur::metIDs = vector<int>();
int Mettaur::currMetIndex = 0;

Mettaur::Mettaur(void)
    : resourceComponent(ResourceComponent(this))
{
    Entity::team = Team::RED;
    health = 20;
    hitHeight = 0;
    direction = Direction::DOWN;
    state = MobState::MOB_IDLE;
    textureType = TextureType::MOB_METTAUR_IDLE;
    healthUI = new MobHealthUI(this);

    cooldown = 0;
    attackCooldown = 0;
    waitCooldown = 0;
    explosionProgress = 0.0f;
    explosionProgress2 = 0.0f;
    attackDelay = 0.0f;

    blinker = 0.0f;
    x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;

    setTexture(*TextureResourceManager::GetInstance().GetTexture(textureType));
    setScale(2.f, 2.f);

    explosion.setTexture(*TextureResourceManager::GetInstance().GetTexture(TextureType::MOB_EXPLOSION));
    explosion.setScale(0.0f, 0.0f);
    explosion2.setTexture(*TextureResourceManager::GetInstance().GetTexture(TextureType::MOB_EXPLOSION));
    explosion2.setScale(0.0f, 0.0f);
    int i = 0;
    int y = 0;
    for (int x = 0; x < EXPLODE_ANIMATION_SPRITES; x++)
    {
        if (x == 8)
        {
            y += EXPLODE_ANIMATION_HEIGHT;
            i = 0;
        }
        explode.addFrame(0.3f, IntRect(EXPLODE_ANIMATION_WIDTH*i, y, EXPLODE_ANIMATION_WIDTH, EXPLODE_ANIMATION_HEIGHT));
        i++;
    }

	this->SetHealth(health);

    //Components setup and load
    resourceComponent.setup(RESOURCE_NAME, RESOURCE_PATH);
    resourceComponent.load();

	if (!whiteout.loadFromFile("resources/shaders/white.frag.txt", sf::Shader::Fragment)) {
		// TODO: log error...
	}
	else {
		whiteout.setParameter("texture", sf::Shader::CurrentTexture);
	}
	
	Mettaur::metIDs.push_back((int)Mettaur::metIDs.size() + 1);
	metID = (int)Mettaur::metIDs.size();
}

Mettaur::~Mettaur(void)
{
	if (Mettaur::metIDs.size() > 0) 
	{
		vector<int>::iterator it = find(Mettaur::metIDs.begin(), Mettaur::metIDs.end(), metID);

		if (it != Mettaur::metIDs.end())
		{
			Mettaur::metIDs.erase(it);
			if (Mettaur::currMetIndex >= Mettaur::metIDs.size())
			{
				Mettaur::currMetIndex = 0;
			}
		}
	}
}

int* Mettaur::getAnimOffset() {
	Mettaur* mob = this;

	int* res = new int[2];
	res[0] = res[1] = 0;

	if (mob->GetTextureType() == TextureType::MOB_METTAUR_IDLE)
	{
		res[0] = 35.f;
		res[1] = 35.f;
	}
	else if (mob->GetTextureType() == TextureType::MOB_METTAUR_ATTACK)
	{
		res[0] = 65.f;
		res[1] = 95.f;
	}
	else if (mob->GetTextureType() == TextureType::MOB_MOVE)
	{
		res[0] = 45.f;
		res[1] = 55.f;
	}

	return res;
}

void Mettaur::Update(float _elapsed)
{
    //Explode animation then set deleted to true once it finishes
    if (health <= 0)
    {
		if ((int)_elapsed*1000 % 4 == 0) {
			SetShader(&whiteout);
		}
		else {
			SetShader(nullptr);
		}

        blinker = 0.0f;
        blinker += 0.01f;
        if (blinker >= 0.5f)
        {
            setColor(sf::Color(255, 255, 255, 128));
        }
        else setColor(sf::Color(255, 255, 255, 255));
        if (blinker >= 1.f) blinker = 0.0f;

        if (explosionProgress == 0.0f)
        {
            x1 = tile->getPosition().x - 10.0f;
            y1 = tile->getPosition().y - 35.f;
            healthUI->setScale(0.0f, 0.0f);
        }
        if (explosionProgress2 == 0.0f)
        {
            x2 = tile->getPosition().x + 10.0f;
            y2 = tile->getPosition().y - 50.0f;
        }
        explosionProgress += 0.020f;
        if (explosionProgress >= 0.3f)
        {
            explosionProgress2 += 0.020f;
            if (explosionProgress >= 0.9f)
            {
                setScale(0.0f, 0.0f);
            }
            if (explosionProgress2 >= 1.f)
            {
                deleted = true;
                return;
            }
            explosion2.setScale(2.f, 2.f);
            explosion2.setPosition(x2, y2);
            explode(explosion2, explosionProgress2);
        }

        if (explosionProgress <= 1.f)
        {
            explosion.setScale(2.f, 2.f);
            explosion.setPosition(x1, y1);
            explode(explosion, explosionProgress);
        }
        return;
    }

    //Cooldown until mob's movement catches up to actual position (avoid walking through spells)
    if (previous)
    {
        if (previous->IsCracked()) previous->SetState(TileState::BROKEN);
        previous->RemoveEntity(this);
        previous = nullptr;
    }

    //Cooldown between moving and attacking
    waitCooldown += _elapsed;
    if (waitCooldown >= WAIT_COOLDOWN)
    {
        cooldown += _elapsed;
        if (cooldown >= COOLDOWN)
        {
            if (state != MobState::MOB_ATTACKING)
            {
                if (Move(direction))
                    state = MobState::MOB_MOVING;
                cooldown = 0.0f;
            }
        }

		// Wait your turn
		if (Mettaur::metIDs.at(Mettaur::currMetIndex) == metID) {
			attackCooldown += _elapsed;
		}

		// NOTE: Mets only attack on their turn
        if (attackCooldown >= ATTACK_COOLDOWN  && (Mettaur::metIDs.at(Mettaur::currMetIndex) == metID))
        {
			Tile* forward = field->GetAt(tile->GetX() - 1, tile->GetY());

            if (state == MobState::MOB_IDLE)
            {
				// NOTE: Mets do not attack on tiles are broken or empty
				if (forward->IsWalkable()) {
					attackCooldown = 0;
					waitCooldown = 0;
					state = MobState::MOB_ATTACKING;
				}
				else {
					state = MobState::MOB_MOVING;
					attackCooldown = 0.0f;
				}
            }
        }
        else
        {
            if (cooldown != 0.0f)
                state = MobState::MOB_IDLE;
        }
    }

    //Delay for animation to look cooler (only do wave at the end of animation)
    if (state == MobState::MOB_ATTACKING)
    {
        attackDelay += _elapsed;
        if (attackDelay >= ATTACK_DELAY)
        {
            Attack();
            attackDelay = 0.0f;

			Mettaur::currMetIndex++; // move to the next met

			if (Mettaur::currMetIndex >= Mettaur::metIDs.size()) {
				Mettaur::currMetIndex = 0;
			}
        }
    }

    RefreshTexture();
    healthUI->Update();
	SetShader(nullptr);
}

bool Mettaur::Move(Direction _direction)
{
    bool moved = false;
    Tile* temp = tile;
    Tile* next = nullptr;
    if (_direction == Direction::UP)
    {
        if (tile->GetY() - 1 > 0)
        {
            next = field->GetAt(tile->GetX(), tile->GetY() - 1);
            if (Teammate(next->GetTeam()) && next->IsWalkable())
				if (!next->ContainsEntityType<Mettaur>())
				{
					SetTile(next);
				}
				else {
					next = nullptr;
					direction = Direction::DOWN;
				}
            else
                next = nullptr;
        }
        else
        {
            direction = Direction::DOWN;
        }
    }
    else if (_direction == Direction::LEFT)
    {
        if (tile->GetX() - 1 > 0)
        {
            next = field->GetAt(tile->GetX() - 1, tile->GetY());
            if (Teammate(next->GetTeam()) && next->IsWalkable())
                SetTile(next);
            else
                next = nullptr;
        }
    }
    else if (_direction == Direction::DOWN)
    {
        if (tile->GetY() + 1 <= (int)field->GetHeight())
        {
            next = field->GetAt(tile->GetX(), tile->GetY() + 1);
            if (Teammate(next->GetTeam()) && next->IsWalkable())
				if (!next->ContainsEntityType<Mettaur>())
				{
					SetTile(next);
				}
				else 
				{
					next = nullptr;
					direction = Direction::UP;
				}
            else
                next = nullptr;
        }
        else
        {
            direction = Direction::UP;
        }
    }
    else if (_direction == Direction::RIGHT)
    {
        if (tile->GetX() + 1 <= (int)field->GetWidth())
        {
            next = field->GetAt(tile->GetX() + 1, tile->GetY());
            if (Teammate(next->GetTeam()) && next->IsWalkable())
                SetTile(next);
            else
                next = nullptr;
        }
    }

    if (next)
    {
        previous = temp;
        moved = true;
    }

    tile->AddEntity(this);
    return moved;
}

void Mettaur::Attack()
{
    if (tile->GetX() + 1 <= (int)field->GetWidth() + 1)
    {
        Spell* spell = new Wave(field, team);
        spell->SetDirection(Direction::LEFT);
        field->AddEntity(spell, tile->GetX() - 1, tile->GetY());
    }
}

void Mettaur::RefreshTexture()
{
    animator.update(clock.restart());
    if (!animator.isPlayingAnimation())
    {
        if (state == MobState::MOB_IDLE)
        {
            textureType = TextureType::MOB_METTAUR_IDLE;
        }
        else if (state == MobState::MOB_MOVING)
        {
            textureType = TextureType::MOB_MOVE;
        }
        else if (state == MobState::MOB_ATTACKING)
        {
            textureType = TextureType::MOB_METTAUR_ATTACK;
        }
        setTexture(*TextureResourceManager::GetInstance().GetTexture(textureType));
        
        if (textureType == TextureType::MOB_METTAUR_IDLE)
        {
            setPosition(tile->getPosition().x + tile->GetWidth()/2.0f - 25.0f, tile->getPosition().y + tile->GetHeight()/2.0f - 45.0f);
            hitHeight = getLocalBounds().height;
        }
        else if (textureType == TextureType::MOB_MOVE)
        {
            setPosition(tile->getPosition().x + tile->GetWidth()/2.0f - 35.0f, tile->getPosition().y + tile->GetHeight()/2.0f - 60.0f);
        }
        else if (textureType == TextureType::MOB_METTAUR_ATTACK)
        {
            setPosition(tile->getPosition().x + tile->GetWidth()/2.0f - 55.0f, tile->getPosition().y + tile->GetHeight()/2.0f - 105.0f);
            hitHeight = getLocalBounds().height;
        }
        animator.playAnimation(state);
    }
    animator.animate(*this);
}

vector<Drawable*> Mettaur::GetMiscComponents()
{
    vector<Drawable*> drawables = vector<Drawable*>();
    drawables.push_back(healthUI);

    if (explosionProgress > 0.0f)
    {
        drawables.push_back(&explosion);
    }
    if (explosionProgress2 > 0.0f)
    {
        drawables.push_back(&explosion2);
    }

    return drawables;
}

int Mettaur::GetStateFromString(string _string)
{
    int size = 4;
    string MOB_STATE_STRINGS[] = { "MOB_MOVING", "MOB_IDLE", "MOB_HIT", "MOB_ATTACKING" };
    for (int i = 0; i < size; i++)
    {
        if (_string == MOB_STATE_STRINGS[i])
        {
            return static_cast<MobState>(i);
        }
    }
    Logger::Log(string("Failed to find corresponding enum: " + _string));
    assert(false);
    return -1;
}

void Mettaur::addAnimation(int _state, FrameAnimation _animation, float _duration)
{
    animator.addAnimation(static_cast<MobState>(_state), _animation, sf::seconds(_duration));
}

TextureType Mettaur::GetTextureType() const
{
    return textureType;
}

MobState Mettaur::GetMobState() const
{
    return state;
}

int Mettaur::GetHealth() const
{
    return health;
}

void Mettaur::SetHealth(int _health)
{
    health = _health;
}

int Mettaur::Hit(int _damage)
{
	SetShader(&whiteout);
    (health - _damage < 0) ? health = 0 : health -= _damage;
    return health;
}

float Mettaur::GetHitHeight() const
{
    return hitHeight;
}