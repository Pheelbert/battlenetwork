#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::Clock;
using sf::Event;
using sf::Font;

#include <time.h>
#include "bnBattleScene.h"
#include "bnMob.h"
#include "bnField.h"
#include "bnPlayer.h"
#include "bnMemory.h"
#include "bnMettaur.h"
#include "bnProgsMan.h"
#include "bnBackgroundUI.h"
#include "bnPlayerHealthUI.h"
#include "bnCamera.h"
#include "bnInputManager.h"
#include "bnChipSelectionCust.h"
#include "bnTimer.h"
#include "bnShaderResourceManager.h"
#include "bnPA.h"
#include "bnEngine.h"
#include "bnBattleResults.h"

int BattleScene::Run(Mob* mob) {
  /*
  Program Advance + labels
  */
  PA programAdvance;
  PASteps paSteps;
  programAdvance.LoadPA();
  bool isPAComplete = false;
  bool hasPA = false;
  int paStepIndex = 0;

  float listStepCooldown = 500.f;
  float listStepCounter  = listStepCooldown;

  /*
  Mob labels*/
  std::vector<std::string> mobNames;

  Camera& camera(ENGINE.GetCamera());

  /*
  Chips + Chip select setup*/
  ChipSelectionCust chipCustGUI(5);
  Chip** chips = 0;
  int chipCount = 0;

  /*
  Battle results pointer */
  BattleResults* battleResults = nullptr;
  Timer battleTimer;

  /*
  Set Scene*/
  Field* field = mob->GetField();

  Player* player(new Player());
  player->StateChange<PlayerIdleState>();
  field->OwnEntity(player, 2, 2);

  BackgroundUI background = BackgroundUI();

  // PAUSE
  sf::Font* font = TEXTURES.LoadFontFromFile("resources/fonts/dr_cain_terminal.ttf");
  sf::Text* pauseLabel = new sf::Text("paused", *font);
  pauseLabel->setOrigin(pauseLabel->getLocalBounds().width / 2, pauseLabel->getLocalBounds().height * 2);
  pauseLabel->setPosition(sf::Vector2f(240.f, 160.f));

  // CHIP CUST
  sf::Texture* customBarTexture = TEXTURES.LoadTextureFromFile("resources/ui/custom.png");
  LayeredDrawable customBarSprite;
  customBarSprite.setTexture(*customBarTexture);
  customBarSprite.setOrigin(customBarSprite.getLocalBounds().width / 2, 0);
  sf::Vector2f customBarPos = sf::Vector2f(240.f, 0.f);
  customBarSprite.setPosition(customBarPos);
  customBarSprite.setScale(2.f, 2.f);
  
  // MOB UI
  sf::Font *mobFont = TEXTURES.LoadFontFromFile("resources/fonts/mmbnthick_regular.ttf");

  // Stream battle music 
  AUDIO.Stream("resources/loops/loop_battle.ogg", true);

  Clock clock;
  float elapsed = 0.0f;
  bool isPaused = false;
  bool isInChipSelect = false;
  bool isChipSelectReady = false;
  bool isPlayerDeleted = false;
  bool isMobDeleted = false;
  bool isBattleRoundOver = false;
  bool isMobFinished = false;
  double customProgress = 0; // in mili seconds 
  double customDuration = 10 * 1000; // 10 seconds
  bool initFadeOut = false;

  // Special: Load shaders if supported 
  double shaderCooldown = 0; // half a second

  sf::Shader& pauseShader = *SHADERS.GetShader(ShaderType::BLACK_FADE);
  pauseShader.setUniform("texture", sf::Shader::CurrentTexture);
  pauseShader.setUniform("opacity", 0.5f);

  sf::Shader& whiteShader = *SHADERS.GetShader(ShaderType::WHITE_FADE);
  whiteShader.setUniform("texture", sf::Shader::CurrentTexture);
  whiteShader.setUniform("opacity", 0.5f);

  sf::Shader& yellowShader = *SHADERS.GetShader(ShaderType::YELLOW);
  whiteShader.setUniform("texture", sf::Shader::CurrentTexture);

  sf::Shader& customBarShader = *SHADERS.GetShader(ShaderType::CUSTOM_BAR);
  customBarShader.setUniform("texture", sf::Shader::CurrentTexture);
  customBarShader.setUniform("factor", 0);
  customBarSprite.SetShader(&customBarShader);

  bool inBattleState = true;
  while (ENGINE.Running() && inBattleState) {
    // check every frame 
    if (!isPlayerDeleted) {
      isPlayerDeleted = player->IsDeleted();

      if (isPlayerDeleted) {
        AUDIO.Play(AudioType::DELETED);
      }
    }

    isBattleRoundOver = (isPlayerDeleted || isMobDeleted);

    float elapsedSeconds = clock.restart().asSeconds();
    float FPS = 0.f;

    if (elapsedSeconds > 0.f) {
      FPS = 1.0f / elapsedSeconds;
      std::string fpsStr = std::to_string(FPS);
      fpsStr.resize(4);
      ENGINE.GetWindow()->setTitle(sf::String(std::string("FPS: ") + fpsStr));
    }

    if (mob->NextMobReady()) {
      Mob::MobData* data = mob->GetNextMob();
      
      // TODO Use a base type that has a target instead of dynamic casting? Or force all Entity classes to have a state.
      // Therefore spells and artifacts will no longer be Entities and instead they will be SceneNodes
      Mettaur* cast = dynamic_cast<Mettaur*>(data->mob);

      if (cast) {
        cast->SetTarget(player);
      }

      ProgsMan* cast2 = dynamic_cast<ProgsMan*>(data->mob);

      if (cast2) {
        cast2->SetTarget(player);
      }

      field->AddEntity(data->mob, data->tileX, data->tileY);
      mobNames.push_back(data->mob->GetName());
    }

    // Check if entire mob is deleted
    if (mob->IsCleared()) {
      isMobDeleted = true; // Hack. Just to trigger fade out and spawn a new mob
    }

    INPUT.update();

    camera.Update(elapsed);

    // Do not update when paused or in chip select
    if (!(isPaused || isInChipSelect)) {
      field->Update(elapsed);
    }

    ENGINE.Clear();
    ENGINE.SetView(camera.GetView());

    background.Draw();

    sf::Vector2f cameraAntiOffset = -ENGINE.GetViewOffset();

    Tile* tile = nullptr;
    while (field->GetNextTile(tile)) {
      tile->move(ENGINE.GetViewOffset());

      if (tile->IsHighlighted()) {
        LayeredDrawable* coloredTile = new LayeredDrawable(*(sf::Sprite*)tile);
        coloredTile->SetShader(&yellowShader);
        ENGINE.Draw(coloredTile);
        delete coloredTile;
      }
      else {
        ENGINE.LayUnder(tile);
      }
    }

    for (int d = 1; d <= field->GetHeight(); d++) {
      Entity* entity = nullptr;
      while (field->GetNextEntity(entity, d)) {
        if (!entity->IsDeleted()) {
          ENGINE.Push(entity);
          ENGINE.Lay(entity->GetMiscComponents());
        }
      }
    }

    // NOTE: Although HUD, it fades dark when on chip cust screen and paused.
    if(!isBattleRoundOver && !isInChipSelect)
      ENGINE.Push(&customBarSprite);

    if (isPaused || isInChipSelect) {
      // apply shader on draw calls below
      ENGINE.SetShader(&pauseShader);
    }

    ENGINE.DrawUnderlay();
    ENGINE.DrawLayers();
    ENGINE.DrawOverlay();

    float nextLabelHeight = 0;
    if (!mob->IsSpawningDone() || isInChipSelect) {
      for (int i = 0; i < mob->GetMobCount(); i++) {
        if (mob->GetMobAt(i).IsDeleted())
          continue;

        sf::Text mobLabel = sf::Text(mob->GetMobAt(i).GetName(), *mobFont);

        mobLabel.setOrigin(mobLabel.getLocalBounds().width, 0);
        mobLabel.setPosition(470.0f, -1.f + nextLabelHeight);
        mobLabel.setScale(0.8f, 0.8f);
        mobLabel.setOutlineColor(sf::Color(48, 56, 80));
        mobLabel.setOutlineThickness(2.f);
        ENGINE.Draw(mobLabel, false);

        // make the next label relative to this one
        nextLabelHeight += mobLabel.getLocalBounds().height;
      }
    }

    if (!isPlayerDeleted) {
      if (player->GetChipsUI()) {
        player->GetChipsUI()->Update(); // DRAW 
      }
    }

    if (isPaused) {
      // render on top 
      ENGINE.Draw(pauseLabel, false);
    }
  

    // Draw cust GUI on top of scene. No shaders affecting.
    chipCustGUI.Draw();

    // Scene keyboard controls
    if (INPUT.has(PRESSED_PAUSE) && !isInChipSelect && !isBattleRoundOver) {
      isPaused = !isPaused;

      if (!isPaused) {
        ENGINE.RevokeShader();
      } else {
        AUDIO.Play(AudioType::PAUSE);
      }
    } else if ((!isMobFinished && mob->IsSpawningDone()) || (INPUT.has(PRESSED_ACTION3) && customProgress >= customDuration && !isInChipSelect && !isBattleRoundOver)) {
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
      
      if (isInChipSelect == false && !isBattleRoundOver) {
        AUDIO.Play(AudioType::CHIP_SELECT);
        // slide up the screen a hair
        //camera.MoveCamera(sf::Vector2f(240.f, 140.f), sf::seconds(0.5f));
        isInChipSelect = true;

        // Clear any chip UI queues. they will contain null data. 
        player->GetChipsUI()->LoadChips(nullptr, 0);

        // Reset PA system
        isPAComplete = false;
        hasPA = false;
        paStepIndex = 0;
        listStepCounter = listStepCooldown;

        // Load the next chips
        chipCustGUI.ResetState();
        chipCustGUI.GetNextChips();
      }

      // NOTE: Need a battle scene state manager to handle going to and from one controll scheme to another. 
      // Plus would make more sense to revoke shaders once complete transition 

    } else if (isInChipSelect && chipCustGUI.IsInView()) {
      if (INPUT.has(PRESSED_LEFT)) {
        chipCustGUI.CursorLeft() ? AUDIO.Play(AudioType::CHIP_SELECT) : 1;
      } else if (INPUT.has(PRESSED_RIGHT)) {
        chipCustGUI.CursorRight() ? AUDIO.Play(AudioType::CHIP_SELECT) : 1;
      } else if (INPUT.has(PRESSED_ACTION1)) {
        bool performed = chipCustGUI.CursorAction();

        if (chipCustGUI.AreChipsReady()) {
          AUDIO.Play(AudioType::CHIP_CONFIRM, 0);
          customProgress = 0; // NOTE: Hack. Need one more state boolean
          //camera.MoveCamera(sf::Vector2f(240.f, 160.f), sf::seconds(0.5f)); 
        } else if(performed){
          AUDIO.Play(AudioType::CHIP_CHOOSE, 0);
        }
        else {
          AUDIO.Play(AudioType::CHIP_ERROR, 0);
        }
      } else if (INPUT.has(PRESSED_ACTION2)) {
        
        chipCustGUI.CursorCancel() ? AUDIO.Play(AudioType::CHIP_CANCEL, 0) : 1;
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
        // Start Program Advance checks
        if(isPAComplete && !hasPA) {
          // Return to game
          isInChipSelect = false;
          player->GetChipsUI()->LoadChips(chips, chipCount);
          ENGINE.RevokeShader();
        }
        else if (!isPAComplete) {
          chips = chipCustGUI.GetChips();
          chipCount = chipCustGUI.GetChipCount();

          hasPA = programAdvance.FindPA(chips, chipCount);

          if(hasPA) {
            paSteps = programAdvance.GetMatchingSteps();
            Chip* paChip = programAdvance.GetAdvanceChip();

            // For now do not use all other chips. 
            // TODO: Only remove the chips involved in the program advance. Replace them with the new PA chip.
            chips = &paChip;
            chipCount = 1;
          }

          isPAComplete = true;
        }
        else if (hasPA) {
          static bool advanceSoundPlay = false;
          static float increment = 0;

          float nextLabelHeight = 0;

          if (paStepIndex <= paSteps.size()) {
            for (int i = 0; i < paStepIndex; i++) {
              sf::Text stepLabel = sf::Text(paSteps[i].first, *mobFont);

              stepLabel.setOrigin(0, 0);
              stepLabel.setPosition(40.0f, 40.f + nextLabelHeight);
              stepLabel.setScale(0.8f, 0.8f);
              stepLabel.setOutlineColor(sf::Color(48, 56, 80));
              stepLabel.setOutlineThickness(2.f);
              ENGINE.Draw(stepLabel, false);

              // make the next label relative to this one
              nextLabelHeight += stepLabel.getLocalBounds().height;
            }
            increment = 0;
          }
          else {
            if (!advanceSoundPlay) {
              AUDIO.Play(AudioType::PA_ADVANCE);
              advanceSoundPlay = true;
            }

            increment += elapsed/500.f;

            sf::Text stepLabel = sf::Text(programAdvance.GetAdvanceChip()->GetShortName(), *mobFont);

            stepLabel.setOrigin(0, 0);
            stepLabel.setPosition(40.0f, 40.f + nextLabelHeight);
            stepLabel.setScale(0.8f, 0.8f);
            stepLabel.setOutlineColor(sf::Color((sf::Uint32)(sin(increment) * 255), (sf::Uint32)(cos(increment+90*(22.f/7.f)) * 255), (sf::Uint32)(sin(increment+180*(22.f/7.f)) * 255)));
            stepLabel.setOutlineThickness(2.f);
            ENGINE.Draw(stepLabel, false);
          }

          if (listStepCounter > 0.f) {
            listStepCounter -= elapsed;
          }
          else {

            if (paStepIndex > paSteps.size()) {
              hasPA = false; // state over 
              advanceSoundPlay = false;
              isPAComplete = true;
            }
            else {
              paStepIndex++;
              listStepCounter = listStepCooldown;

              if (paStepIndex <= paSteps.size()) {
                AUDIO.Play(AudioType::POINT);
              }
            }
          }
        }
      }
    }

    if (isBattleRoundOver && !isPlayerDeleted) {
      if (!battleResults) {
        sf::Time totalBattleTime = sf::milliseconds((sf::Int32)battleTimer.GetElapsed());

        // TODO: GetCounterCount()
        //       GetDoubleDelete()
        //       GetTripleDelete()
        battleResults = new BattleResults(totalBattleTime, player->GetMoveCount(), player->GetHitCount(), 0, false, false, mob);
        
        AUDIO.StopStream();
        AUDIO.Stream("resources/loops/enemy_deleted.ogg");
      }
      else {
        battleResults->Draw();

        if (!battleResults->IsInView()) {
          float amount = 100.f / elapsed;
          battleResults->Move(sf::Vector2f(amount, 0));
        }
        else {
          if (INPUT.has(PRESSED_ACTION3)) {
            // Have to hit twice
            if (battleResults->IsFinished()) {
              // TODO: sent the battle item off to the player's gaming session for storage
              BattleItem* reward = battleResults->GetReward();
              if (reward) delete reward;

              inBattleState = false;
            }
            else {
              AUDIO.Play(AudioType::ITEM_GET);
              battleResults->CursorAction();
            }
          }
        }
      }
    }
    else if (isBattleRoundOver && isPlayerDeleted) {
      if (!initFadeOut) {
        AUDIO.StopStream();
        shaderCooldown = 1000;
        ENGINE.SetShader(&whiteShader);
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

    // Write contents to screen (always last step)
    ENGINE.Display();

    // TODO: make camera effects apply only to individual scenes that request them
    // This will avoid this hack here to move elements around on screen
    tile = nullptr;
    while (field->GetNextTile(tile)) {
      tile->move(cameraAntiOffset);
    }

    // update the cust if not paused nor in chip select nor in mob intro nor battle results
    if (!(isBattleRoundOver || isPaused || isInChipSelect || !mob->IsSpawningDone())) {
      customProgress += elapsed;

      if (battleTimer.IsPaused()) {
        // start counting seconds again 
        battleTimer.Start();
      }
    }
    else {
      battleTimer.Pause();
    }

    if (customProgress / customDuration >= 1.0) {
      if (isChipSelectReady == false) {
        AUDIO.Play(AudioType::CUSTOM_BAR_FULL);
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

  if (battleResults) { delete battleResults; }

  AUDIO.StopStream();
  ENGINE.RevokeShader();

  return !isPlayerDeleted;
}