#pragma once
#include "bnMeta.h"
#include "bnEntity.h"
#include "bnAIState.h"
#include "bnAudioResourceManager.h"
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
  sf::Shader pixelated;
  float factor;
  AIState<Any>* firstState;
  FinishNotifier callback;
public:
  PixelInState(FinishNotifier onFinish);
  ~PixelInState();

  void OnEnter(Any& e);
  void OnUpdate(float _elapsed, Any& e);
  void OnLeave(Any& e);
};

#include "bnField.h"
#include "bnLogger.h"

#define SHADER_FRAG_PATH "resources/shaders/pixel_blur.frag.txt"

template<typename Any>
PixelInState<Any>::PixelInState(FinishNotifier onFinish) : AIState<Any>() {
  // Enforce template constraints on class
  _DerivedFrom<Any, Entity>();

  callback = onFinish;
  factor = 400.f;

  if (!pixelated.loadFromFile(SHADER_FRAG_PATH, sf::Shader::Fragment)) {
    Logger::Log("Error loading shader: " SHADER_FRAG_PATH);
  }
  else {
    pixelated.setUniform("texture", sf::Shader::CurrentTexture);
    pixelated.setUniform("pixel_threshold", 1.f);
  }
}

template<typename Any>
PixelInState<Any>::~PixelInState() {
}

template<typename Any>
void PixelInState<Any>::OnEnter(Any& e) {
  firstState = e.GetDefaultState();
  e.Lock(); // Lock AI state until complete
  AudioResourceManager::GetInstance().Play(AudioType::APPEAR);
}

template<typename Any>
void PixelInState<Any>::OnUpdate(float _elapsed, Any& e) {
  /* freeze frame */
  e.SetShader(&pixelated);

  /* If progress is 1, pop state and move onto original state*/
  factor -= _elapsed;

  if (factor <= 0.f) {
    factor = 0.f;

    if (firstState) {
      e.Unlock();
      e.StateChange(firstState);
      return;
    }
  }

  pixelated.setUniform("pixel_threshold", (float)(factor/400.f));
}

template<typename Any>
void PixelInState<Any>::OnLeave(Any& e) {
  e.SetShader(nullptr);
  
  if (callback) { callback(); }
}