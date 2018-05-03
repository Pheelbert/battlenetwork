#pragma once
#include "bnMeta.h"
#include "bnEntity.h"
#include "bnAIState.h"
#include "bnAudioResourceManager.h"
#include "bnShaderResourceManager.h"
#include <iostream>

/*
This state can be used by any Entity in the engine.
It uses constraints to ensure the type passed in Any
is a subclass of Entity.

This state locks the entity until it comes into focus
when pixel shader's factor is equal to 1
*/


typedef std::function<void()> FinishNotifier;

template<typename Any>
class PixelInState : public AIState<Any>
{
private:
  sf::Shader* pixelated;
  float factor;
  FinishNotifier callback;
  bool playedFX;
public:
  PixelInState(FinishNotifier onFinish);
  ~PixelInState();

  void OnEnter(Any& e);
  void OnUpdate(float _elapsed, Any& e);
  void OnLeave(Any& e);
};

#include "bnField.h"
#include "bnLogger.h"

template<typename Any>
PixelInState<Any>::PixelInState(FinishNotifier onFinish) : AIState<Any>() {
  // Enforce template constraints on class
  _DerivedFrom<Any, Entity>();

  callback = onFinish;
  factor = 400.f;

  pixelated = ShaderResourceManager::GetInstance().GetShader(ShaderType::PIXEL_BLUR);
}

template<typename Any>
PixelInState<Any>::~PixelInState() {
}

template<typename Any>
void PixelInState<Any>::OnEnter(Any& e) {
  // play swoosh
  AudioResourceManager::GetInstance().Play(AudioType::APPEAR);
}

template<typename Any>
void PixelInState<Any>::OnUpdate(float _elapsed, Any& e) {
  /* freeze frame */
  e.SetShader(pixelated);

  /* If progress is 1, pop state and move onto original state*/
  factor -= _elapsed;

  if (factor <= 0.f) {
    factor = 0.f;

    if (callback) { callback(); callback = nullptr; /* do once */ }
  }

  pixelated->setUniform("pixel_threshold", (float)(factor/400.f));
}

template<typename Any>
void PixelInState<Any>::OnLeave(Any& e) {
  e.SetShader(nullptr);
}