#include "bnEntity.h"
#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnChargeComponent.h"

ChargeComponent::ChargeComponent(Entity* _entity) {
  entity = _entity;
  charging = false;
  chargeCounter = 0.0f;
  animationProgress = 0.0f;
  chargeTexture = Texture();
  chargeSprite = Sprite();
  blueChargeAnimation = FrameAnimation();
  purpleChargeAnimation = FrameAnimation();;
  isCharged = isPartiallyCharged = false;
}

ChargeComponent::~ChargeComponent() {
}

void ChargeComponent::load() {
  chargeTexture = *TextureResourceManager::GetInstance().LoadTextureFromFile(CHARGE_TEXTURE);
  for (int x = 0; x < BLUE_CHARGE_FRAME_COUNT; x++) {
    blueChargeAnimation.addFrame(0.2f, IntRect(CHARGE_WIDTH*x, 0, CHARGE_WIDTH, CHARGE_HEIGHT));
  }

  int y = CHARGE_HEIGHT;
  for (int x = 0; x < 8; x++) {
    purpleChargeAnimation.addFrame(0.3f, IntRect(CHARGE_WIDTH*x, y, CHARGE_WIDTH, CHARGE_HEIGHT));
  }

  y += CHARGE_HEIGHT;
  for (int x = 0; x < 8; x++) {
    purpleChargeAnimation.addFrame(0.3f, IntRect(CHARGE_WIDTH*x, y, CHARGE_WIDTH, CHARGE_HEIGHT));
  }
}

void ChargeComponent::update(float _elapsed) {
  if (!charging) {
    chargeCounter = 0.0f;
    chargeSprite.setScale(0.0f, 0.0f);
    isCharged = false;
    isPartiallyCharged = false;
  } else {
    chargeCounter += _elapsed;
    if (chargeCounter >= CHARGE_COUNTER_MAX) {
      if (isCharged == false) {
        // We're switching states
        AudioResourceManager::GetInstance().Play(AudioType::BUSTER_CHARGED);
      }

      isCharged = true;
      animationProgress += 0.03f;
      (animationProgress >= 1.0f) ? animationProgress = 0.0f : animationProgress = animationProgress;
      chargeSprite.setTexture(chargeTexture);
      chargeSprite.setPosition(entity->getPosition().x - 18.0f, entity->getPosition().y);
      chargeSprite.setScale(2.0f, 2.0f);
      purpleChargeAnimation(chargeSprite, animationProgress);
    } else if (chargeCounter >= CHARGE_COUNTER_MIN) {
      if (isPartiallyCharged == false) {
        // Switching states
        AudioResourceManager::GetInstance().Play(AudioType::BUSTER_CHARGING);
      }

      isPartiallyCharged = true;
      animationProgress += 0.05f;
      (animationProgress >= 1.0f) ? animationProgress = 0.0f : animationProgress = animationProgress;
      chargeSprite.setTexture(chargeTexture);
      chargeSprite.setPosition(entity->getPosition().x - 18.0f, entity->getPosition().y);
      chargeSprite.setScale(2.0f, 2.0f);
      blueChargeAnimation(chargeSprite, animationProgress);
    }

  }
}

void ChargeComponent::SetCharging(bool _charging) {
  charging = _charging;
}

float ChargeComponent::GetChargeCounter() const {
  return chargeCounter;
}

const bool ChargeComponent::IsFullyCharged() const
{
  return isCharged;
}

Sprite& ChargeComponent::GetSprite() {
  return chargeSprite;
}