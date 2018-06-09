#include <SFML\Graphics.hpp>
using sf::Sprite;
using sf::IntRect;

#include "bnAnimationComponent.h"
#include "bnFileUtil.h"
#include "bnLogger.h"
#include "bnEntity.h"

AnimationComponent::AnimationComponent(Entity* _entity) {
  entity = _entity;
}

AnimationComponent::~AnimationComponent() {
}

void AnimationComponent::Update(float _elapsed)
{
  animation.Update(_elapsed, entity);
}

void AnimationComponent::Setup(string _path)
{
  entityName = entity->GetName();
  path = _path;
}

void AnimationComponent::Load() {
  animation = Animation(entityName, path);
  animation.Load();
}

void AnimationComponent::SetAnimation(string state, std::function<void()> onFinish)
{
  animation.SetAnimation(state);
  animation << onFinish;
}

void AnimationComponent::AddCallback(int frame, std::function<void()> onFrame, std::function<void()> outFrame) {
  animation << Animate::On(frame, onFrame) << Animate::On(frame+1, outFrame);
}