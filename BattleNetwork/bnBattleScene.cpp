#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::Clock;
using sf::Event;
using sf::Font;

#include <time.h>

#include "bnBattleScene.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnMemory.h"
#include "bnMettaur.h"
#include "bnProgsMan.h"
#include "bnBackgroundUI.h"
#include "bnPlayerHealthUI.h"
#include "bnCamera.h"
#include "bnControllableComponent.h"
#include "bnEngine.h"
#include "bnChipSelectionCust.h"

#define SHADER_FRAG_PIXEL_PATH "resources/shaders/pixel_blur.frag.txt"
#define SHADER_FRAG_BLACK_PATH "resources/shaders/black_fade.frag.txt"
#define SHADER_FRAG_WHITE_PATH "resources/shaders/white_fade.frag.txt"
#define SHADER_FRAG_BAR_PATH "resources/shaders/custom_bar.frag.txt"

int BattleScene::Run(Mob* mob) {
  std::vector<std::string> mobNames;

  Camera camera(Engine::GetInstance().GetDefaultView());

  ChipSelectionCust chipCustGUI(5);

  Field* field = mob->GetField();

  Player* player(new Player());
  player->StateChange<PlayerIdleState>();
  field->AddEntity(player, 2, 2);

  BackgroundUI background = BackgroundUI();

  // PAUSE
  sf::Font* font = TextureResourceManager::GetInstance().LoadFontFromFile("resources/fonts/dr_cain_terminal.ttf");
  sf::Text* pauseLabel = new sf::Text("paused", *font);
  pauseLabel->setOrigin(pauseLabel->getLocalBounds().width / 2, pauseLabel->getLocalBounds().height * 2);
  pauseLabel->setPosition(sf::Vector2f(240.f, 160.f));

  // CHIP CUST
  sf::Texture* customBarTexture = TextureResourceManager::GetInstance().LoadTextureFromFile("resources/ui/custom.png");
  LayeredDrawable customBarSprite;
  customBarSprite.setTexture(*customBarTexture);
  customBarSprite.setOrigin(customBarSprite.getLocalBounds().width / 2, 0);
  sf::Vector2f customBarPos = sf::Vector2f(240.f, 0.f);
  customBarSprite.setPosition(customBarPos);
  customBarSprite.setScale(2.f, 2.f);
  
  // MOB UI
  sf::Font *mobFont = TextureResourceManager::GetInstance().LoadFontFromFile("resources/fonts/mmbnthick_regular.ttf");

  // Stream battle music 
  AudioResourceManager::GetInstance().Stream("resources/loops/loop_battle.ogg", true);

  Clock clock;
  float elapsed = 0.0f;
  bool isPaused = false;
  bool isInChipSelect = false;
  bool isChipSelectReady = false;
  bool isPlayerDeleted = false;
  bool isMobFinished = false;
  double customProgress = 0; // in mili seconds 
  double customDuration = 10 * 1000; // 10 seconds
  bool initFadeOut = false;

  // Special: Load shaders if supported 
  double shaderCooldown = 0; // half a second

  sf::Shader pauseShader;
  if (!pauseShader.loadFromFile(SHADER_FRAG_BLACK_PATH, sf::Shader::Fragment)) {
    Logger::Log("Error loading shader: " SHADER_FRAG_BLACK_PATH);
  } else {
    pauseShader.setUniform("texture", sf::Shader::CurrentTexture);
    pauseShader.setUniform("opacity", 0.5f);
  }

  sf::Shader whiteShader;
  if (!whiteShader.loadFromFile(SHADER_FRAG_WHITE_PATH, sf::Shader::Fragment)) {
    Logger::Log("Error loading shader: " SHADER_FRAG_WHITE_PATH);
  }
  else {
    whiteShader.setUniform("texture", sf::Shader::CurrentTexture);
    whiteShader.setUniform("opacity", 0.5f);
  }

  sf::Shader customBarShader;
  if (!customBarShader.loadFromFile(SHADER_FRAG_BAR_PATH, sf::Shader::Fragment)) {
    Logger::Log("Error loading shader: " SHADER_FRAG_BAR_PATH);
  } else {
    customBarShader.setUniform("texture", sf::Shader::CurrentTexture);
    customBarShader.setUniform("factor", 0);
    customBarSprite.SetShader(&customBarShader);
  }

  bool inBattleState = true;
  while (Engine::GetInstance().Running() && inBattleState) {
    // check evert frame 
    if (!isPlayerDeleted) {
      isPlayerDeleted = player->IsDeleted();

      if (isPlayerDeleted) {
        AudioResourceManager::GetInstance().Play(AudioType::DELETED);
      }
    }

    float elapsedSeconds = clock.restart().asSeconds();
    float FPS = 0.f;

    if (elapsedSeconds > 0.f) {
      FPS = 1.0f / elapsedSeconds;
      std::string fpsStr = std::to_string(FPS);
      fpsStr.resize(4);
      Engine::GetInstance().GetWindow()->setTitle(sf::String(std::string("FPS: ") + fpsStr));
    }

    if (mob->NextMobReady()) {
      Mob::MobData* data = mob->GetNextMob();
      
      // TODO Use a base type that has a target instead of dynamic casting? Or force all Entity classes to have a state.
      // Therefore spells and artifacts will no longer be Entities and instead they will be SceneNodes
      Mettaur* cast = dynamic_cast<Mettaur*>(data->mob);

      if (cast) {
        cast->SetTarget(player);
      }

      field->AddEntity(data->mob, data->tileX, data->tileY);
      mobNames.push_back(data->mob->GetName());
    }

    // Check if entire mob is deleted
    if (mob->IsCleared()) {
      isPlayerDeleted = true; // Hack. Just to trigger fade out and spawn a new mob
    }

    ControllableComponent::GetInstance().update();

    camera.Update(elapsed);

    // Do not update when paused or in chip select
    if (!(isPaused || isInChipSelect)) {
      field->Update(elapsed);
    }

    Engine::GetInstance().Clear();
    Engine::GetInstance().SetView(camera.GetView());

    background.Draw();

    Tile* tile = nullptr;
    while (field->GetNextTile(tile)) {
      Engine::GetInstance().LayUnder(tile);
    }

    for (int d = 1; d <= field->GetHeight(); d++) {
      Entity* entity = nullptr;
      while (field->GetNextEntity(entity, d)) {
        if (!entity->IsDeleted()) {
          Engine::GetInstance().Push(entity);
          Engine::GetInstance().Lay(entity->GetMiscComponents());
        }
      }
    }

    if (!mob->IsSpawningDone() || isInChipSelect) {
      for (int i = 0; i < mobNames.size(); i++) {
        sf::Text mobLabel = sf::Text(mobNames[i], *mobFont);
        mobLabel.setOrigin(mobLabel.getLocalBounds().width, 0);
        mobLabel.setPosition(470.0f, -1.f + (i*mobLabel.getLocalBounds().height));
        mobLabel.setScale(0.8f, 0.8f);
        mobLabel.setOutlineColor(sf::Color(48, 56, 80));
        mobLabel.setOutlineThickness(2.f);
        Engine::GetInstance().Draw(mobLabel, false);
      }
    }

    // NOTE: Although HUD, it fades dark when on chip cust screen and paused.
    if(!isPlayerDeleted && !isInChipSelect)
      Engine::GetInstance().Push(&customBarSprite);

    if (isPaused || isInChipSelect) {
      // apply shader on draw calls below
      Engine::GetInstance().SetShader(&pauseShader);
    }

    Engine::GetInstance().DrawUnderlay();
    Engine::GetInstance().DrawLayers();
    Engine::GetInstance().DrawOverlay();

    if (!isPlayerDeleted) {
      if (player->GetChipsUI()) {
        player->GetChipsUI()->Update(); // DRAW 
      }
    }

    if (isPaused) {
      // render on top 
      Engine::GetInstance().Draw(pauseLabel, false);
    }

    // Draw cust GUI on top of scene. No shaders affecting.
    chipCustGUI.Draw();

    // Write contents to screen (always last step)
    Engine::GetInstance().Display();

    // Scene keyboard controls
    if (ControllableComponent::GetInstance().has(PRESSED_PAUSE) && !isInChipSelect && !isPlayerDeleted) {
      isPaused = !isPaused;

      if (!isPaused) {
        Engine::GetInstance().RevokeShader();
      } else {
        AudioResourceManager::GetInstance().Play(AudioType::PAUSE);
      }
    } else if ((!isMobFinished && mob->IsSpawningDone()) || (ControllableComponent::GetInstance().has(PRESSED_ACTION3) && customProgress >= customDuration && !isInChipSelect && !isPlayerDeleted)) {
       // enemy intro finished
      if (!isMobFinished) { 
        // toggle the flag
        isMobFinished = true; 
        // allow the player to be controlled by keys
        player->StateChange<PlayerControlledState>(); 
        // Move mob out of the PixelInState 
        mob->DefaultState();
        // show the chip select screen
        customProgress = customDuration; 
      }

      if (isInChipSelect == false) {
        AudioResourceManager::GetInstance().Play(AudioType::CHIP_SELECT);
        // slide up the screen a hair
        //camera.MoveCamera(sf::Vector2f(240.f, 140.f), sf::seconds(0.5f));
        isInChipSelect = true;

        // Clear any chip UI queues. they will contain null data. 
        player->GetChipsUI()->LoadChips(nullptr, 0);

        // Load the next chips
        chipCustGUI.ResetState();
        chipCustGUI.GetNextChips();
      }

      // NOTE: Need a battle scene state manager to handle going to and from one controll scheme to another. 
      // Plus would make more sense to revoke shaders once complete transition 

    } else if (isInChipSelect) {
      if (ControllableComponent::GetInstance().has(PRESSED_LEFT)) {
        chipCustGUI.CursorLeft() ? AudioResourceManager::GetInstance().Play(AudioType::CHIP_SELECT) : 1;
      } else if (ControllableComponent::GetInstance().has(PRESSED_RIGHT)) {
        chipCustGUI.CursorRight() ? AudioResourceManager::GetInstance().Play(AudioType::CHIP_SELECT) : 1;
      } else if (ControllableComponent::GetInstance().has(PRESSED_ACTION1)) {
        bool performed = chipCustGUI.CursorAction();

        if (chipCustGUI.AreChipsReady()) {
          AudioResourceManager::GetInstance().Play(AudioType::CHIP_CONFIRM);
          customProgress = 0; // NOTE: Hack. Need one more state boolean
          //camera.MoveCamera(sf::Vector2f(240.f, 160.f), sf::seconds(0.5f)); 
        } else if(performed){
          AudioResourceManager::GetInstance().Play(AudioType::CHIP_CHOOSE);
        }
      } else if (ControllableComponent::GetInstance().has(PRESSED_ACTION2)) {
        
        chipCustGUI.CursorCancel() ? AudioResourceManager::GetInstance().Play(AudioType::CHIP_CANCEL) : 1;
      }
    }

    if (isInChipSelect && customProgress > 0.f) {
      if (!chipCustGUI.IsInView()) {
        chipCustGUI.Move(sf::Vector2f(150.f / elapsed, 0));
      }
    } else {
      if (!chipCustGUI.IsOutOfView()) {
        chipCustGUI.Move(sf::Vector2f(-150.f / elapsed, 0));
      } else if (isInChipSelect) { // we're leaving a state
        // Return to game
        isInChipSelect = false;
        player->GetChipsUI()->LoadChips(chipCustGUI.GetChips(), chipCustGUI.GetChipCount());
        Engine::GetInstance().RevokeShader();
      }
    }

    if (isPlayerDeleted) {
      if (!initFadeOut) {
        AudioResourceManager::GetInstance().StopStream();
        shaderCooldown = 1000;
        Engine::GetInstance().SetShader(&whiteShader);
        initFadeOut = true;
      }
      else {
        if (shaderCooldown < 0) {
          shaderCooldown = 0;
          inBattleState = false;
        }
      }

      shaderCooldown -= elapsed;

      whiteShader.setUniform("opacity", 1.f - (float)(shaderCooldown / 1000.f)*0.5f);

    }

    // update the cust if not paused nor in chip select nor in mob intro
    if (!(isPaused || isInChipSelect || !mob->IsSpawningDone())) customProgress += elapsed;

    if (customProgress / customDuration >= 1.0) {
      if (isChipSelectReady == false) {
        AudioResourceManager::GetInstance().Play(AudioType::CUSTOM_BAR_FULL);
        isChipSelectReady = true;
      }
    } else {
      isChipSelectReady = false;
    }

    customBarShader.setUniform("factor", (float)(customProgress / customDuration));

    elapsed = static_cast<float>(clock.getElapsedTime().asMilliseconds());
  }

  delete pauseLabel;
  delete font;
  delete mobFont;
  delete customBarTexture;

  AudioResourceManager::GetInstance().StopStream();
  Engine::GetInstance().RevokeShader();

  return EXIT_SUCCESS;
}