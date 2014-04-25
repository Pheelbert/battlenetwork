#include <SFML\Window.hpp>
using sf::Event;
using sf::Keyboard;
#include "bnPlayer.h"
#include "bnField.h"
#include "bnBuster.h"
#include "bnResourceManager.h"
#include "bnRenderer.h"
#include "bnLogger.h"

#define RESOURCE_NAME "megaman"
#define RESOURCE_PATH "resources/navis/megaman/megaman.animation"

#define MOVE_KEY_PRESS_COOLDOWN 200.0f
#define MOVE_LAG_COOLDOWN 80.0f
#define ATTACK_KEY_PRESS_COOLDOWN 300.0f
#define ATTACK_TO_IDLE_COOLDOWN 150.0f

#define CHARGE_COUNTER_MAX 1400.0f

#define MOVE_ANIMATION_SPRITES 4
#define MOVE_ANIMATION_WIDTH 38
#define MOVE_ANIMATION_HEIGHT 58

#define SHOOT_ANIMATION_SPRITES 5
#define SHOOT_ANIMATION_WIDTH 75
#define SHOOT_ANIMATION_HEIGHT 58

Player::Player(void)
    : health(99),
      state(PlayerState::PLAYER_IDLE),
      ttype(TextureType::NAVI_MEGAMAN_MOVE),
      chargeComponent(ChargeComponent(this)),
      resourceComponent(ResourceComponent(this))
{
    SetLayer(0);
    team = Team::BLUE;

    //Cooldowns
    moveKeyPressCooldown = 0.0f;
    moveLagCooldown = 0.0f;
    attackKeyPressCooldown = 0.0f;
    attackToIdleCooldown = 0.0f;

    //Animation
    animationProgress = 0.0f;
    setScale(2.0f, 2.0f);

    healthUI = new PlayerHealthUI(this);

    //Components setup and load
    chargeComponent.load();
    resourceComponent.setup(RESOURCE_NAME, RESOURCE_PATH);
    resourceComponent.load();
}

Player::~Player(void)
{
    delete healthUI; 
}

void Player::Update(float _elapsed)
{
    moveKeyPressCooldown   += _elapsed;
    moveLagCooldown        += _elapsed;
    attackKeyPressCooldown += _elapsed;
    attackToIdleCooldown   += _elapsed;

    //Components updates
    chargeComponent.update(_elapsed);

    //Update UI of player's health (top left corner)
    healthUI->Update();

    //Cooldown until player's movement catches up to actual position (avoid walking through spells)
    if (moveLagCooldown >= MOVE_LAG_COOLDOWN)
    {
        if (previous)
        {
            if (previous->IsCracked()) previous->SetState(TileState::BROKEN);
            previous->RemoveEntity(this);
            previous = nullptr;
        }
    }

    Event event;
    Direction direction = Direction::NONE;
    while (Renderer::GetInstance().GetWindow()->pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            Renderer::GetInstance().GetWindow()->close();
        }

        if (Event::KeyPressed == event.type)
        {
            if (moveKeyPressCooldown >= MOVE_KEY_PRESS_COOLDOWN)
            {
                if (Keyboard::Up == event.key.code)
                {
                    direction = Direction::UP;
                }
                else if (Keyboard::Left == event.key.code)
                {
                    direction = Direction::LEFT;
                }
                else if (Keyboard::Down == event.key.code)
                {
                    direction = Direction::DOWN;
                }
                else if (Keyboard::Right == event.key.code)
                {
                    direction = Direction::RIGHT;
                }
            }
            else /* Move key press cooldown not elapsed yet */
            {
                state = PlayerState::PLAYER_IDLE;
            }

            if (attackKeyPressCooldown >= ATTACK_KEY_PRESS_COOLDOWN)
            {
                if (Keyboard::Space == event.key.code)
                {
                    attackKeyPressCooldown = 0.0f;
                    chargeComponent.SetCharging(true);
                }
            }
        }
        else /* Not pressing a key, set player back to idle */
        {
            //Unless he's shooting
            if (state != PlayerState::PLAYER_SHOOTING)
            {
                state = PlayerState::PLAYER_IDLE;
            }
        }

        if (Event::KeyReleased == event.type)
        {
            if (Keyboard::Up == event.key.code)
            {
                direction = Direction::NONE;
            }
            else if (Keyboard::Left == event.key.code)
            {
                direction = Direction::NONE;
            }
            else if (Keyboard::Down == event.key.code)
            {
                direction = Direction::NONE;
            }
            else if (Keyboard::Right == event.key.code)
            {
                direction = Direction::NONE;
            }
            else if (Keyboard::Space == event.key.code)
            {
                Attack(chargeComponent.GetChargeCounter());
                chargeComponent.SetCharging(false);
                attackToIdleCooldown = 0.0f;
                state = PlayerState::PLAYER_SHOOTING;
            }
        }
    }

    if (direction != Direction::NONE && state != PlayerState::PLAYER_SHOOTING)
    {
        if (Move(direction))
        {
            state = PlayerState::PLAYER_MOVING;
        }
        else
        {
            state = PlayerState::PLAYER_IDLE;
        }
        moveKeyPressCooldown = 0.0f;
        moveLagCooldown = 0.0f;
    }

    if (attackToIdleCooldown >= ATTACK_TO_IDLE_COOLDOWN)
    {
        if (state == PlayerState::PLAYER_SHOOTING)
        {
            state = PlayerState::PLAYER_IDLE;
        }
    }

    RefreshTexture();
}

bool Player::Move(Direction _direction)
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
            {
                SetTile(next);
            }
            else
            {
                next = nullptr;
            }
        }
    }
    else if (_direction == Direction::LEFT)
    {
        if (tile->GetX() - 1 > 0)
        {
            next = field->GetAt(tile->GetX() - 1, tile->GetY());
            if (Teammate(next->GetTeam()) && next->IsWalkable())
            {
                SetTile(next);
            }
            else
            {
                next = nullptr;
            }
        }
    }
    else if (_direction == Direction::DOWN)
    {
        if (tile->GetY() + 1 <= (int)field->GetHeight())
        {
            next = field->GetAt(tile->GetX(), tile->GetY() + 1);
            if (Teammate(next->GetTeam()) && next->IsWalkable())
            {
                SetTile(next);
            }
            else
            {
                next = nullptr;
            }
        }
    }
    else if (_direction == Direction::RIGHT)
    {
        if (tile->GetX() + 1 <= static_cast<int>(field->GetWidth()))
        {
            next = field->GetAt(tile->GetX() + 1, tile->GetY());
            if (Teammate(next->GetTeam()) && next->IsWalkable())
            {
                SetTile(next);
            }
            else
            {
                next = nullptr;
            }
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

void Player::Attack(float _charge)
{
    if (tile->GetX() + 1 <= static_cast<int>(field->GetWidth()))
    {
        Spell* spell = new Buster(field, team, (_charge >= CHARGE_COUNTER_MAX));
        spell->SetDirection(Direction::RIGHT);
        field->AddEntity(spell, tile->GetX() + 1, tile->GetY());
    }
}

vector<Drawable*> Player::GetMiscComponents()
{
    vector<Drawable*> drawables = vector<Drawable*>();
    Drawable* component;
    while (healthUI->GetNextComponent(component))
    {
        drawables.push_back(component);
    }
    drawables.push_back(&chargeComponent.GetSprite());
    return drawables;
}

void Player::SetHealth(int _health)
{
    health = _health;
}

int Player::GetHealth() const
{
    return health;
}

int Player::Hit(int _damage)
{
    return (health - _damage < 0) ? health = 0 : health -= _damage;
}

void Player::RefreshTexture()
{
    static sf::Clock clock;
    animator.update(clock.restart());

    switch (state)
    {
    case PlayerState::PLAYER_IDLE:
        ttype = TextureType::NAVI_MEGAMAN_MOVE;
        break;
    case PlayerState::PLAYER_MOVING:
        ttype = TextureType::NAVI_MEGAMAN_MOVE;
        break;
    case PlayerState::PLAYER_SHOOTING:
        ttype = TextureType::NAVI_MEGAMAN_SHOOT;
        break;
    default:
        assert(false && "Invalid player state.");
    }

    if (!animator.isPlayingAnimation())
    {
        setTexture(*ResourceManager::GetInstance().GetTexture(ttype));
        animator.playAnimation(state);
    }

    if (tile != nullptr)
    {
        setPosition(tile->getPosition().x + 2.f, tile->getPosition().y - 76.f);
    }
    animator.animate(*this);
}

PlayerHealthUI* Player::GetHealthUI() const
{
    return healthUI;
}

int Player::getStateFromString(string _string)
{
    int size = 3;
    string PLAYER_STATE_STRINGS[] = { "PLAYER_IDLE", "PLAYER_MOVING", "PLAYER_SHOOTING" };
    for (int i = 0; i < size; i++)
    {
        if (_string == PLAYER_STATE_STRINGS[i])
        {
            return static_cast<PlayerState>(i);
        }
    }
    Logger::Log(string("Failed to find corresponding enum: " + _string));
    assert(false);
    return -1;
}

void Player::addAnimation(int _state, FrameAnimation _animation, float _duration)
{
    animator.addAnimation(static_cast<PlayerState>(_state), _animation, sf::seconds(_duration));
}