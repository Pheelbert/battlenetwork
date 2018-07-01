#include "bnCanodumb.h"
#include "bnCanodumbIdleState.h"
#include "bnCanodumbAttackState.h"
#include "bnExplodeState.h"
#include "bnTile.h"
#include "bnField.h"
#include "bnWave.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnEngine.h"

#define RESOURCE_NAME "canodumb"
#define RESOURCE_PATH "resources/mobs/canodumb/canodumb.animation"

Canodumb::Canodumb(void)
  : animationComponent(this), AI<Canodumb>(this) {
  this->StateChange<CanodumbIdleState>();
  name = "Canodumb";
  Entity::team = Team::BLUE;
  health = 40;
  hitHeight = 0;
  textureType = TextureType::MOB_CANODUMB_ATLAS;
  healthUI = new MobHealthUI(this);

  setTexture(*TEXTURES.GetTexture(textureType));
  setScale(2.f, 2.f);

  this->SetHealth(health);

  //Components setup and load
  animationComponent.Setup(RESOURCE_PATH);
  animationComponent.Load();
  animationComponent.SetAnimation(MOB_CANODUMB_IDLE_1);

  whiteout = SHADERS.GetShader(ShaderType::WHITE);
  stun = SHADERS.GetShader(ShaderType::YELLOW);
}

Canodumb::~Canodumb(void) {

}

int* Canodumb::GetAnimOffset() {
  return nullptr;
}

void Canodumb::Update(float _elapsed) {
  this->SetShader(nullptr);

  if (stunCooldown > 0) {
    stunCooldown -= _elapsed;
    healthUI->Update();
    Entity::Update(_elapsed);

    if (stunCooldown <= 0) {
      stunCooldown = 0;
      animationComponent.Update(_elapsed);
    }

    if ((int)stunCooldown % 5 == 0) {
      this->SetShader(stun);
    }

    if (GetHealth() > 0) {
      return;
    }
  }

  this->StateUpdate(_elapsed);

  // Explode if health depleted
  if (GetHealth() <= 0) {
    this->StateChange<ExplodeState<Canodumb>>();
    this->Lock();
  }
  else {
    this->RefreshTexture();
    animationComponent.Update(_elapsed);
  }

  healthUI->Update();
  Entity::Update(_elapsed);
}

void Canodumb::RefreshTexture() {
  setPosition(tile->getPosition().x + tile->GetWidth() / 2.0f - 1.0f , tile->getPosition().y + tile->GetHeight() / 2.0f - 5.0f);
  hitHeight = getLocalBounds().height;
}

vector<Drawable*> Canodumb::GetMiscComponents() {
  vector<Drawable*> drawables = vector<Drawable*>();
  drawables.push_back(healthUI);

  return drawables;
}

void Canodumb::SetAnimation(string _state, std::function<void()> onFinish) {
  state = _state;
  animationComponent.SetAnimation(_state, onFinish);
}

void Canodumb::SetCounterFrame(int frame)
{
  auto onFinish = [&]() { this->ToggleCounter(); };
  auto onNext = [&]() { this->ToggleCounter(false); };
  animationComponent.AddCallback(frame, onFinish, onNext);
}

TextureType Canodumb::GetTextureType() const {
  return textureType;
}

int Canodumb::GetHealth() const {
  return health;
}

void Canodumb::SetHealth(int _health) {
  health = _health;
}

const bool Canodumb::Hit(int _damage) {
  (health - _damage < 0) ? health = 0 : health -= _damage;
  SetShader(whiteout);

  return health;
}

const float Canodumb::GetHitHeight() const {
  return hitHeight;
}