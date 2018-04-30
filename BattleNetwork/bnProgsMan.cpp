#include "bnProgsMan.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnWave.h"
#include "bnProgBomb.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnEngine.h"
#include "bnExplodeState.h"

#define RESOURCE_NAME "progsman"
#define RESOURCE_PATH "resources/mobs/progsman/progsman.animation"
#define SHADER_FRAG_PATH "resources/shaders/white.frag.txt"

#define PROGS_COOLDOWN 1000.0f
#define PROGS_ATTACK_COOLDOWN 2222.f
#define PROGS_WAIT_COOLDOWN 100.0f
#define PROGS_ATTACK_DELAY 500.0f

ProgsMan::ProgsMan(void)
  : animationComponent(AnimationComponent(this)),
    AI<ProgsMan>(this) {
  name = "ProgsMan";
  Entity::team = Team::RED;
  health = 20;
  hitHeight = 0;
  direction = Direction::DOWN;
  state = MobState::MOB_IDLE;
  textureType = TextureType::MOB_PROGSMAN_IDLE;
  healthUI = new MobHealthUI(this);

  StateChange<ProgsManIdleState>();

  cooldown = 0;
  attackCooldown = 0;
  waitCooldown = 0;
  attackDelay = 0.0f;

  setTexture(*TextureResourceManager::GetInstance().GetTexture(textureType));
  setScale(2.f, 2.f);

  this->SetHealth(health);

  //Components setup and load
  animationComponent.setup(RESOURCE_NAME, RESOURCE_PATH);
  animationComponent.load();

  if (!whiteout.loadFromFile(SHADER_FRAG_PATH, sf::Shader::Fragment)) {
    Logger::Log("Error loading shader: " SHADER_FRAG_PATH);
  } else {
    whiteout.setUniform("texture", sf::Shader::CurrentTexture);
  }

  target = nullptr;
}

ProgsMan::~ProgsMan(void) {
}

void ProgsMan::SetTarget(Entity* _target) {
  target = _target;
}

int* ProgsMan::GetAnimOffset() {
  ProgsMan* mob = this;

  int* res = new int[2];

  if (mob->GetTextureType() == TextureType::MOB_PROGSMAN_IDLE) {
    res[0] = 75;
    res[1] = 115;
  } else if (mob->GetTextureType() == TextureType::MOB_PROGSMAN_PUNCH) {
    res[0] = 125;
    res[1] = 125;
  } else if (mob->GetTextureType() == TextureType::MOB_PROGSMAN_MOVE) {
    res[0] = 75;
    res[1] = 115;
  } else if (mob->GetTextureType() == TextureType::MOB_PROGSMAN_THROW) {
    res[0] = 75;
    res[1] = 115;
  }

  return res;
}

void ProgsMan::Update(float _elapsed) {
  SetShader(nullptr);

  this->StateUpdate(_elapsed);

  // Explode if health depleted
  if (GetHealth() <= 0) {
    this->StateChange<ExplodeState<ProgsMan>>();
    this->Lock();
  }
  else {
    this->RefreshTexture();
    animationComponent.update(_elapsed);
  }

  healthUI->Update();
  Entity::Update(_elapsed);
}

void ProgsMan::Attack() {
  if (state == MobState::MOB_THROW) {
    if (target != nullptr) {
      Tile* targetTile = target->GetTile();
      Tile* aimTile = field->GetAt(targetTile->GetX(), targetTile->GetY());
      ProgBomb* spell = new ProgBomb(field, team, aimTile, 1.0);
      spell->SetDirection(Direction::LEFT);
      field->AddEntity(spell, tile->GetX(), tile->GetY());
      spell->PrepareThrowPath();
    }
  } else {
    if (tile->GetX() + 1 <= (int)field->GetWidth() + 1) {
      Spell* spell = new Wave(field, team);
      spell->SetDirection(Direction::LEFT);
      field->AddEntity(spell, tile->GetX() - 1, tile->GetY());

      Tile* next = 0;
      next = field->GetAt(tile->GetX() - 1, tile->GetY());

      Entity* entity = 0;

      while (next->GetNextEntity(entity)) {
        Player* isPlayer = dynamic_cast<Player*>(entity);

        if (isPlayer) {
          isPlayer->Move(Direction::LEFT);
          isPlayer->Hit(20);
        }
      }
    }
  }
}

void ProgsMan::RefreshTexture() {
  if (state == MobState::MOB_IDLE) {
    textureType = TextureType::MOB_PROGSMAN_IDLE;
  } else if (state == MobState::MOB_MOVING) {
    textureType = TextureType::MOB_PROGSMAN_MOVE;
  } else if (state == MobState::MOB_ATTACKING) {
    textureType = TextureType::MOB_PROGSMAN_PUNCH;
  } else if (state == MobState::MOB_THROW) {
    textureType = TextureType::MOB_PROGSMAN_THROW;
  }

  setTexture(*TextureResourceManager::GetInstance().GetTexture(textureType));

  if (textureType == TextureType::MOB_PROGSMAN_IDLE) {
    setPosition(tile->getPosition().x + tile->GetWidth() / 2.0f - 65.0f, tile->getPosition().y + tile->GetHeight() / 2.0f - 115.0f);
    hitHeight = getLocalBounds().height;
  } else if (textureType == TextureType::MOB_PROGSMAN_MOVE) {
    setPosition(tile->getPosition().x + tile->GetWidth() / 2.0f - 65.0f, tile->getPosition().y + tile->GetHeight() / 2.0f - 125.0f);
  } else if (textureType == TextureType::MOB_PROGSMAN_PUNCH) {
    setPosition(tile->getPosition().x + tile->GetWidth() / 2.0f - 115.0f, tile->getPosition().y + tile->GetHeight() / 2.0f - 125.0f);
    hitHeight = getLocalBounds().height;
  } else if (textureType == TextureType::MOB_PROGSMAN_THROW) {
    setPosition(tile->getPosition().x + tile->GetWidth() / 2.0f - 115.0f, tile->getPosition().y + tile->GetHeight() / 2.0f - 125.0f);
    hitHeight = getLocalBounds().height;
  }
}

vector<Drawable*> ProgsMan::GetMiscComponents() {
  vector<Drawable*> drawables = vector<Drawable*>();
  drawables.push_back(healthUI);

  return drawables;
}

int ProgsMan::GetStateFromString(string _string) {
  int size = 5;
  string MOB_STATE_STRINGS[] = { "MOB_MOVING", "MOB_IDLE", "MOB_HIT", "MOB_ATTACKING", "MOB_THROW" };

  for (int i = 0; i < size; i++) {
    if (_string == MOB_STATE_STRINGS[i]) {
      return static_cast<MobState>(i);
    }
  }
  Logger::Log(string("Failed to find corresponding enum: " + _string));
  assert(false);
  return -1;
}

TextureType ProgsMan::GetTextureType() const {
  return textureType;
}

MobState ProgsMan::GetMobState() const {
  return state;
}

int ProgsMan::GetHealth() const {
  return health;
}

void ProgsMan::SetHealth(int _health) {
  health = _health;
}

int ProgsMan::Hit(int _damage) {
  SetShader(&whiteout);
  (health - _damage < 0) ? health = 0 : health -= _damage;
  return health;
}

float ProgsMan::GetHitHeight() const {
  return hitHeight;
}

void ProgsMan::SetAnimation(int _state, std::function<void()> onFinish) {
  this->state = static_cast<MobState>(_state);
  animationComponent.setAnimation(_state, onFinish);
}