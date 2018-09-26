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
  blueChargeAnimation = FrameList();
  purpleChargeAnimation = FrameList();;
  isCharged = isPartiallyCharged = false;
}

ChargeComponent::~ChargeComponent() {
}

void ChargeComponent::load() {
  chargeTexture = *TEXTURES.LoadTextureFromFile(CHARGE_TEXTURE);
  for (int x = 0; x < BLUE_CHARGE_FRAME_COUNT; x++) {
    blueChargeAnimation.Add(0.2f, IntRect(CHARGE_WIDTH*x, 0, CHARGE_WIDTH, CHARGE_HEIGHT));
  }

  int y = CHARGE_HEIGHT;
  for (int x = 0; x < 8; x++) {
    purpleChargeAnimation.Add(0.3f, IntRect(CHARGE_WIDTH*x, y, CHARGE_WIDTH, CHARGE_HEIGHT));
  }

  y += CHARGE_HEIGHT;
  for (int x = 0; x < 8; x++) {
    purpleChargeAnimation.Add(0.3f, IntRect(CHARGE_WIDTH*x, y, CHARGE_WIDTH, CHARGE_HEIGHT));
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
        AUDIO.Play(AudioType::BUSTER_CHARGED);
      }

      isCharged = true;
      animationProgress += 5*_elapsed;
      (animationProgress >= 1.0f) ? animationProgress = 0.0f : animationProgress = animationProgress;
      chargeSprite.setTexture(chargeTexture);
      chargeSprite.setPosition(entity->getPosition().x + 9.0f, entity->getPosition().y - 36.f);
      chargeSprite.setScale(2.0f, 2.0f);
      chargeSprite.setOrigin(sf::Vector2f(chargeSprite.getLocalBounds().width/2.f, chargeSprite.getLocalBounds().height/2.f));
      animator(animationProgress, chargeSprite, purpleChargeAnimation);
    } else if (chargeCounter >= CHARGE_COUNTER_MIN) {
      if (isPartiallyCharged == false) {
        // Switching states
        AUDIO.Play(AudioType::BUSTER_CHARGING);
      }

      isPartiallyCharged = true;
      animationProgress += 4*_elapsed;
      (animationProgress >= 1.0f) ? animationProgress = 0.0f : animationProgress = animationProgress;
      chargeSprite.setTexture(chargeTexture);
      chargeSprite.setPosition(entity->getPosition().x + 9.0f, entity->getPosition().y - 36.f);
      chargeSprite.setScale(2.0f, 2.0f);
      chargeSprite.setOrigin(sf::Vector2f(chargeSprite.getLocalBounds().width/2.f, chargeSprite.getLocalBounds().height/2.f));
      animator(animationProgress, chargeSprite, blueChargeAnimation);
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