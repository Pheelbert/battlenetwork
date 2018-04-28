#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnControllableComponent.h"
#include "bnEngine.h"
#include "bnBattleScene.h"
#include "bnMettaur.h"
#include "SFML/System.hpp"
#include <Thor/Animations.hpp>
#include <time.h>
#include <queue>

using sf::Clock;

#define TITLE_ANIM_CHAR_SPRITES 14
#define TITLE_ANIM_CHAR_WIDTH 128
#define TITLE_ANIM_CHAR_HEIGHT 221
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

  // Log output text
  sf::Font* font = TextureResourceManager::GetInstance().LoadFontFromFile("resources/fonts/mmbnthin_regular.ttf");
  sf::Text* logLabel = new sf::Text("...", *font);
  logLabel->setCharacterSize(10);
  logLabel->setOrigin(0.f, logLabel->getLocalBounds().height);
  std::vector<std::string> logs;

  // Title screen background
  // This will be loaded from the resource manager AFTER it's ready
  sf::Texture* bg = nullptr;
  sf::Texture* progs = nullptr;
  thor::FrameAnimation progAnim;
  float progAnimProgress = 0.f;
  LayeredDrawable bgSprite;
  LayeredDrawable progSprite;

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
  double logFadeOutTimer = 4000;
  double logFadeOutSpeed = 2000;

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
    std::string percentageStr = std::to_string((int)(percentage*100));
    Engine::GetInstance().GetWindow()->setTitle(sf::String(std::string("Loading: ") + percentageStr + "%"));

    ControllableComponent::GetInstance().update();

    /*
      Get next logs. One at a time for effect.
    */
    std::string log;

    Logger::GetMutex()->lock();
    if(Logger::GetNextLog(log)) {
      logs.insert(logs.begin(), log);
    }
    Logger::GetMutex()->unlock();


    if (progress == totalObjects) {
      if (!ready) {
        ready = true;
        Engine::GetInstance().SetShader(&whiteShader);
      }
      else { // Else we are ready next frame
        if (!bg) {
          // NOW we can load resources from internal storage throughout the game
          try {
            bg = TextureResourceManager::GetInstance().GetTexture(TextureType::BACKGROUND_BLUE);
            bgSprite.setTexture(*bg);
            bgSprite.setScale(2.f, 2.f);
          }
          catch (std::exception e) {
            // didnt catchup? debug
          }
        }

        if (!progs) {
          // NOW we can load resources from internal storage throughout the game
          try {
            progs = TextureResourceManager::GetInstance().GetTexture(TextureType::TITLE_ANIM_CHAR);

            progSprite.setTexture(*progs);
            progSprite.setPosition(200.f, 0.f);
            progSprite.setScale(2.f, 2.f);

            int i = 0;
            for (int x = 0; x < TITLE_ANIM_CHAR_SPRITES; x++) {
              progAnim.addFrame(1.f/(float)TITLE_ANIM_CHAR_SPRITES, sf::IntRect(TITLE_ANIM_CHAR_WIDTH*i, 0, TITLE_ANIM_CHAR_WIDTH, TITLE_ANIM_CHAR_HEIGHT));
              i++;
            }

          }
          catch (std::exception e) {
            // didnt catchup? debug
          }
        }

        shaderCooldown -= elapsed;
        progAnimProgress += elapsed/2000.f;

        if (shaderCooldown < 0) {
          shaderCooldown = 0;
        }

        // Just a bunch of timers for events on screen
        if (shaderCooldown == 0) {
          logFadeOutTimer -= elapsed;
        }

        if (logFadeOutTimer <= 0) {
          logFadeOutSpeed -= elapsed;
        }

        if (logFadeOutSpeed < 0) {
          logFadeOutSpeed = 0;
        }

        // keep animation in bounds
        if (progAnimProgress > 1.f) {
          progAnimProgress = 0.f;
        }

        // update shader 
        whiteShader.setUniform("opacity", (float)(shaderCooldown / 1000.f)*0.5f);
      }

      if (ControllableComponent::GetInstance().has(PRESSED_ACTION3)) {
        inLoadState = false;
      }
    }

    // Prepare for next draw calls
    Engine::GetInstance().Clear();

    // if background is ready and loaded from threads...
    if (ready) {
      // show it 
      Engine::GetInstance().Draw(&bgSprite);
    }

    // Draw logs on top of bg
    for (int i = 0; i < logs.size(); i++) {
      // fade from newest to oldest
      // newest at bottom full opacity 
      // oldest at the top (at most 30 on screen) at full transparency
      logLabel->setString(logs[i]);
      logLabel->setPosition(0.f, 320 - (i * 10.f) - 5.f);
      logLabel->setFillColor(sf::Color(255, 255, 255, (float)(logFadeOutSpeed/2000.f)*fmax(0, 255 - (255 / 30)*i)));
      Engine::GetInstance().Draw(logLabel);
    }

    if (progs) {
      progAnim(progSprite, progAnimProgress);
      Engine::GetInstance().Draw(&progSprite);
    }

    Engine::GetInstance().Draw(&logoSprite);

    Engine::GetInstance().DrawUnderlay();
    Engine::GetInstance().DrawLayers();
    Engine::GetInstance().DrawOverlay();

    // Write contents to screen
    Engine::GetInstance().Display();

    elapsed = static_cast<float>(clock.getElapsedTime().asMilliseconds());
  }

  // Cleanup
  Engine::GetInstance().RevokeShader();
  Engine::GetInstance().Clear();
  delete logLabel;
  delete font;
  delete logo;

  // Stop music and go to battle 
  AudioResourceManager::GetInstance().StopStream();

  // Make sure we didn't quit the loop prematurely
  if (Engine::GetInstance().Running()) {

    Mob* mob = new Mob();
    mob->Add<Mettaur>(6, 2)->Add<Mettaur>(4, 2)->Add<Mettaur>(6, 1)->Add<Mettaur>(5, 1)->Add<Mettaur>(5, 2)->Add<Mettaur>(5, 3)->Add<Mettaur>(6, 3);

    return BattleScene::Run(mob);
  }
  
  return EXIT_SUCCESS;
}