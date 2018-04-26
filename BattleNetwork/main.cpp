#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnControllableComponent.h"
#include "bnEngine.h"
#include "bnBattleScene.h"
#include "SFML/System.hpp"
#include <time.h>

using sf::Clock;

#define SHADER_FRAG_WHITE_PATH "resources/shaders/white_fade.frag.txt"

void RunTextureResourceInit(unsigned * progress) {
  sf::sleep(sf::milliseconds(2000)); // Simulate long loading to see title better
  const clock_t begin_time = clock();
  TextureResourceManager::GetInstance().LoadAllTextures(*progress);
  Logger::Logf("Loaded textures: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);
}

void RunAudioResourceInit(unsigned * progress) {
  const clock_t begin_time = clock();
  AudioResourceManager::GetInstance().LoadAllSources(*progress);
  Logger::Logf("Loaded audio sources: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);
}

int main(int argc, char** argv) {
  // Render context must:
  //                    1) always run from main thread and 
  //                    2) load before we do any loading screen rendering
  const clock_t begin_time = clock();
  Engine::GetInstance().Initialize();
  Logger::Logf("Engine initialized: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);

  // lazy init
  //TextureResourceManager::GetInstance();
  AudioResourceManager::GetInstance();

  // Title screen logo
  sf::Texture* logo = TextureResourceManager::GetInstance().LoadTextureFromFile("resources/backgrounds/title/tile.png");
  LayeredDrawable logoSprite;
  logoSprite.setTexture(*logo);
  logoSprite.setOrigin(logoSprite.getLocalBounds().width / 2, logoSprite.getLocalBounds().height / 2);
  sf::Vector2f logoPos = (sf::Vector2f)((sf::Vector2i)Engine::GetInstance().GetWindow()->getSize() / 2);
  logoSprite.setPosition(logoPos);

  // Title screen background
  // This will be loaded from the resource manager AFTER it's ready
  sf::Texture* bg = nullptr;
  LayeredDrawable bgSprite;

  // TODO: Add shaders to this list
  unsigned totalObjects = (unsigned)TextureType::TEXTURE_TYPE_SIZE + (unsigned)AudioType::AUDIO_TYPE_SIZE;
  unsigned progress = 0;

  sf::Thread textureLoad(&RunTextureResourceInit, &progress);
  sf::Thread audioLoad(&RunAudioResourceInit, &progress);

  textureLoad.launch(); 
  audioLoad.launch();

  // play some music while we wait
  AudioResourceManager::GetInstance().SetStreamVolume(10);
  AudioResourceManager::GetInstance().Stream("resources/loops/loop_theme.ogg", true);

  // Draw some stats while we wait 
  // TODO: Drawing actual logs from logger here instead of console
  bool inLoadState = true;
  bool ready = false;

  double shaderCooldown = 2000; // 2 seconds

  sf::Shader whiteShader;
  if (!whiteShader.loadFromFile(SHADER_FRAG_WHITE_PATH, sf::Shader::Fragment)) {
    Logger::Log("Error loading shader: " SHADER_FRAG_WHITE_PATH);
  }
  else {
    whiteShader.setUniform("texture", sf::Shader::CurrentTexture);
    whiteShader.setUniform("opacity", 0.0f);
  }

  Clock clock;
  float elapsed = 0.0f;

  while(inLoadState && Engine::GetInstance().Running()) {
    clock.restart();

    float percentage = (float)progress / (float)totalObjects;
    std::string percentageStr = std::to_string(percentage*100);
    percentageStr.resize(4);
    Engine::GetInstance().GetWindow()->setTitle(sf::String(std::string("Loading: ") + percentageStr + "%"));

    ControllableComponent::GetInstance().update();

    // Prepare for next call
    Engine::GetInstance().Clear();

    // if background is ready and loaded from threads...
    if (ready) {
      // show it 
      Engine::GetInstance().LayUnder(&bgSprite);
    }

    Engine::GetInstance().Push(&logoSprite);

    Engine::GetInstance().DrawUnderlay();
    Engine::GetInstance().DrawLayers();
    Engine::GetInstance().DrawOverlay();

    // Write contents to screen
    Engine::GetInstance().Display();

    if (progress == totalObjects) {
      if (!ready) {
        ready = true;
        Engine::GetInstance().SetShader(&whiteShader);
      }
      else { // Else we are ready next frame
        if (!bg) {
          // NOW we can load resources from internal storage throughout the game
          try {
            bg = TextureResourceManager::GetInstance().GetTexture(TextureType::BACKGROUND_TITLE);
            bgSprite.setTexture(*bg);
            bgSprite.setScale(2.f, 2.f);
          }
          catch (std::exception e) {
            // didnt catchup? debug
          }
        }

        shaderCooldown -= elapsed;

        if (shaderCooldown < 0) {
          shaderCooldown = 0;
        }

        whiteShader.setUniform("opacity", (float)(shaderCooldown / 1000.f)*0.5f);
      }

      if (ControllableComponent::GetInstance().has(PRESSED_ACTION3)) {
        inLoadState = false;
      }
    }

    elapsed = static_cast<float>(clock.getElapsedTime().asMilliseconds());
  }

  // Stop music and go to battle 
  // TODO: wait for keypress?
  AudioResourceManager::GetInstance().StopStream();

  // Make sure we didn't quit the loop prematurely
  if (Engine::GetInstance().Running()) {
    return BattleScene::Run();
  }
  
  return EXIT_SUCCESS;
}