#include "bnCharacter.h"
#include "bnTile.h"
#include "bnField.h"

Character::Character() :
  health(0),
  counterable(false),
  stunCooldown(0),
  name("unnamed") {
}

Character::~Character() {
}

void Character::Update(float _elapsed) {
  Entity::Update(_elapsed);
}

bool Character::Move(Direction _direction) {
  assert(false && "Move shouldn't be called directly from Character");
  return false;
}

vector<Drawable*> Character::GetMiscComponents() {
  assert(false && "GetMiscComponents shouldn't be called directly from Character");
  return vector<Drawable*>();
}

void Character::AddAnimation(string _state, FrameList _frameList, float _duration) {
  assert(false && "AddAnimation shouldn't be called directly from Character");
}

void Character::SetAnimation(string _state) {
  assert(false && "SetAnimation shouldn't be called directly from Character");
}

void Character::SetCounterFrame(int frame)
{
  assert(false && "SetCounterFrame shouldn't be called directly from Character");
}

int Character::GetHealth() {
  return health;
}

const float Character::GetHitHeight() const {
  //assert(false && "GetHitHeight shouldn't be called directly from Entity");
  return 0;
}

const bool Character::Hit(int damage) {
  //assert(false && "Hit shouldn't be called directly from Entity");
  return false;
}

int* Character::GetAnimOffset() {
  return nullptr;
}

void Character::SetHealth(const int _health) {
  health = _health;
}

void Character::TryDelete() {
  deleted = (health <= 0);
}

void Character::ToggleCounter(bool on)
{
  counterable = on;
}

void Character::Stun(double maxCooldown)
{
  stunCooldown = maxCooldown;
}

bool Character::IsCountered()
{
  return (counterable && stunCooldown <= 0);
}

const std::string Character::GetName() const
{
  return name;
}
