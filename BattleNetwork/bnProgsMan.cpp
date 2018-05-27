#include "bnProgsMan.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnWave.h"
#include "bnProgBomb.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnShaderResourceManager.h"
#include "bnEngine.h"
#include "bnExplodeState.h"

#define RESOURCE_NAME "progsman"
#define RESOURCE_PATH "resources/mobs/progsman/progsman.animation"

#define PROGS_COOLDOWN 1000.0f
#define PROGS_ATTACK_COOLDOWN 2222.f
#define PROGS_WAIT_COOLDOWN 100.0f
#define PROGS_ATTACK_DELAY 500.0f

ProgsMan::ProgsMan(void)
  : animationComponent(this),
    AI<ProgsMan>(this) {
  name = "ProgsMan";
  Entity::team = Team::RED;
  health = 300;
  hitHeight = 0;
  state = MobState::MOB_IDLE;
  textureType = TextureType::MOB_PROGSMAN_IDLE;
  healthUI = new MobHealthUI(this);

  this->StateChange<ProgsManIdleState>();

  setTexture(*TEXTURES.GetTexture(textureType));
  setScale(2.f, 2.f);

  this->SetHealth(health);

  //Components setup and load
  animationComponent.setup(RESOURCE_NAME, RESOURCE_PATH);
  animationComponent.load();

  whiteout = SHADERS.GetShader(ShaderType::WHITE);
}

ProgsMan::~ProgsMan(void) {
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

  setTexture(*TEXTURES.GetTexture(textureType));

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

int ProgsMan::GetHealth() const {
  return health;
}

void ProgsMan::SetHealth(int _health) {
  health = _health;
}

int ProgsMan::Hit(int _damage) {
  SetShader(whiteout);
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