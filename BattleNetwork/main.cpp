#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include "bnShaderResourceManager.h"
#include "bnControllableComponent.h"
#include "bnEngine.h"
#include "bnBattleScene.h"
#include "bnMobFactory.h"
#include "bnRandomMettaurMob.h"
#include "bnTwoMettaurMob.h"
#include "SFML/System.hpp"
#include <Thor/Animations.hpp>
#include <time.h>
#include <queue>
#include <atomic>

using sf::Clock;

#define TITLE_ANIM_CHAR_SPRITES 14
#define TITLE_ANIM_CHAR_WIDTH 128
#define TITLE_ANIM_CHAR_HEIGHT 221
#define SHADER_FRAG_WHITE_PATH "resources/shaders/white_fade.frag.txt"

void RunGraphicsInit(std::atomic<int> * progress) {
  sf::sleep(sf::milliseconds(1000)); // Simulate long loading to see title better
  clock_t begin_time = clock();
  TEXTURES.LoadAllTextures(*progress);
  Logger::Logf("Loaded textures: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);

  begin_time = clock();
  SHADERS.LoadAllShaders(*progress);
  Logger::Logf("Loaded shaders: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);
}

void RunAudioInit(std::atomic<int> * progress) {
  const clock_t begin_time = clock();
  AUDIO.LoadAllSources(*progress);
  Logger::Logf("Loaded audio sources: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);
}

int main(int argc, char** argv) {
  // Render context must:
  //                    1) always run from main thread and 
  //                    2) load before we do any loading screen rendering
  const clock_t begin_time = clock();
  ENGINE.Initialize();
  Logger::Logf("Engine initialized: %f secs", float(clock() - begin_time) / CLOCKS_PER_SEC);

  // lazy init
  //TextureResourceManager::GetInstance();
  ShaderResourceManager::GetInstance();
  AudioResourceManager::GetInstance();

  // Title screen logo
  sf::Texture* logo = TEXTURES.LoadTextureFromFile("resources/backgrounds/title/tile_en.png");
  LayeredDrawable logoSprite;
  logoSprite.setTexture(*logo);
  logoSprite.setOrigin(logoSprite.getLocalBounds().width / 2, logoSprite.getLocalBounds().height / 2);
  sf::Vector2f logoPos = (sf::Vector2f)((sf::Vector2i)ENGINE.GetWindow()->getSize() / 2);
  logoSprite.setPosition(logoPos);

  // Log output text
  sf::Font* font = TEXTURES.LoadFontFromFile("resources/fonts/mmbnthin_regular.ttf");
  sf::Text* logLabel = new sf::Text("...", *font);
  logLabel->setCharacterSize(10);
  logLabel->setOrigin(0.f, logLabel->getLocalBounds().height);
  std::vector<std::string> logs;

  // Press Start text
  sf::Font* startFont = TEXTURES.LoadFontFromFile("resources/fonts/mmbnthick_regular.ttf");
  sf::Text* startLabel = new sf::Text("PRESS START", *startFont);
  startLabel->setCharacterSize(24);
  startLabel->setOrigin(0.f, startLabel->getLocalBounds().height);
  startLabel->setPosition(sf::Vector2f(180.0f, 240.f));

  // Title screen background
  // This will be loaded from the resource manager AFTER it's ready
  sf::Texture* bg = nullptr;
  sf::Texture* progs = nullptr;
  thor::FrameAnimation progAnim;
  float progAnimProgress = 0.f;
  LayeredDrawable bgSprite;
  LayeredDrawable progSprite;

  int totalObjects = (unsigned)TextureType::TEXTURE_TYPE_SIZE + (unsigned)AudioType::AUDIO_TYPE_SIZE + (unsigned)ShaderType::SHADER_TYPE_SIZE;
  std::atomic<int> progress = 0;

  sf::Thread graphicsLoad(&RunGraphicsInit, &progress);
  sf::Thread audioLoad(&RunAudioInit, &progress);

  graphicsLoad.launch();
  audioLoad.launch();

  // play some music while we wait
  AUDIO.SetStreamVolume(10);
  AUDIO.Stream("resources/loops/loop_theme.ogg", true);

  // Draw some stats while we wait 
  bool inLoadState = true;
  bool ready = false;

  double shaderCooldown = 2000; // 2 seconds
  double logFadeOutTimer = 4000;
  double logFadeOutSpeed = 2000;

  sf::Shader* whiteShader = nullptr;

  Clock clock;
  float elapsed = 0.0f;

  while(inLoadState && ENGINE.Running()) {
    clock.restart();

    float percentage = (float)progress / (float)totalObjects;
    std::string percentageStr = std::to_string((int)(percentage*100));
    ENGINE.GetWindow()->setTitle(sf::String(std::string("Loading: ") + percentageStr + "%"));

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
      }
      else { // Else we are ready next frame
        if (!bg) {
          // NOW we can load resources from internal storage throughout the game
          try {
            bg = TEXTURES.GetTexture(TextureType::BACKGROUND_BLUE);
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
            progs = TEXTURES.GetTexture(TextureType::TITLE_ANIM_CHAR);

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

        if (!whiteShader) {
          try {
            whiteShader = SHADERS.GetShader(ShaderType::WHITE_FADE);
            whiteShader->setUniform("opacity", 0.0f);
            ENGINE.SetShader(whiteShader);
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
        whiteShader->setUniform("opacity", (float)(shaderCooldown / 1000.f)*0.5f);
      }

      if (ControllableComponent::GetInstance().has(PRESSED_ACTION3)) {
        inLoadState = false;
      }
    }

    // Prepare for next draw calls
    ENGINE.Clear();

    // if background is ready and loaded from threads...
    if (ready) {
      // show it 
      ENGINE.Draw(&bgSprite);
    }

    // Draw logs on top of bg
    for (int i = 0; i < logs.size(); i++) {
      // fade from newest to oldest
      // newest at bottom full opacity 
      // oldest at the top (at most 30 on screen) at full transparency
      logLabel->setString(logs[i]);
      logLabel->setPosition(0.f, 320 - (i * 10.f) - 5.f);
      logLabel->setFillColor(sf::Color(255, 255, 255, (sf::Uint8)((logFadeOutSpeed/2000.f)*fmax(0, 255 - (255 / 30)*i))));
      ENGINE.Draw(logLabel);
    }

    if (progs) {
      progAnim(progSprite, progAnimProgress);
      ENGINE.Draw(&progSprite);
      ENGINE.Draw(startLabel);
    }

    ENGINE.Draw(&logoSprite);

    ENGINE.DrawUnderlay();
    ENGINE.DrawLayers();
    ENGINE.DrawOverlay();

    // Write contents to screen
    ENGINE.Display();

    elapsed = static_cast<float>(clock.getElapsedTime().asMilliseconds());
  }

  // Cleanup
  ENGINE.RevokeShader();
  ENGINE.Clear();
  delete logLabel;
  delete font;
  delete logo;

  // Stop music and go to battle 
  AUDIO.StopStream();

  // Make sure we didn't quit the loop prematurely
  while(ENGINE.Running()) {

    Field* field(new Field(6, 3));
    // see how the random mob works around holes
    field->GetAt((rand())%3+4, (rand()%3)+1)->SetState(TileState::EMPTY);

    // MobFactory* factory = new TwoMettaurMob(field);
    MobFactory* factory = new RandomMettaurMob(field);
    Mob* mob = factory->Build();

    BattleScene::Run(mob);

    delete mob;
    delete factory;
    delete field;
  }
  
  return EXIT_SUCCESS;
}