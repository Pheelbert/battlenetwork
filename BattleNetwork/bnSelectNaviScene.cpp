#include <time.h>
#include "bnSelectNaviScene.h"
#include "bnMemory.h"
#include "bnCamera.h"
#include "bnAnimation.h"
#include "bnInputManager.h"
#include "bnAudioResourceManager.h"
#include "bnShaderResourceManager.h"
#include "bnTextureResourceManager.h"
#include "bnEngine.h"

#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::Clock;
using sf::Event;
using sf::Font;

SelectedNavi SelectNaviScene::Run(SelectedNavi currentNavi) {
  Camera camera(ENGINE.GetDefaultView());

  // Menu name font
  sf::Font* font = TEXTURES.LoadFontFromFile("resources/fonts/dr_cain_terminal.ttf");
  sf::Text* menuLabel = new sf::Text("BATTLE SELECT", *font);
  menuLabel->setCharacterSize(15);
  menuLabel->setPosition(sf::Vector2f(20.f, 5.0f));

  // Selection input delays
  double maxSelectInputCooldown = 0.5; // half of a second
  double selectInputCooldown = maxSelectInputCooldown;

  // NAVI UI font
  sf::Font *naviFont = TEXTURES.LoadFontFromFile("resources/fonts/mmbnthick_regular.ttf");
  sf::Text *naviLabel = new sf::Text("Mettaur", *naviFont);
  naviLabel->setPosition(275.f, 15.f);

  sf::Text *attackLabel = new sf::Text("1", *naviFont);
  attackLabel->setPosition(325.f, 30.f);

  sf::Text *speedLabel = new sf::Text("1", *naviFont);
  speedLabel->setPosition(325.f, 45.f);

  sf::Font *hpFont = TEXTURES.LoadFontFromFile("resources/fonts/mgm_nbr_pheelbert.ttf");
  sf::Text *hpLabel = new sf::Text("20", *hpFont);
  hpLabel->setOutlineColor(sf::Color(48, 56, 80));
  hpLabel->setOutlineThickness(2.f);
  hpLabel->setScale(0.8f, 0.8f);
  hpLabel->setOrigin(hpLabel->getLocalBounds().width, 0);
  hpLabel->setPosition(sf::Vector2f(180.f, 33.0f));

  float maxNumberCooldown = 0.5;
  float numberCooldown = maxNumberCooldown; // half a second
  SelectedNavi naviSelectionIndex = (SelectedNavi)currentNavi;

  // select menu graphic
  sf::Sprite bg(*TEXTURES.GetTexture(TextureType::NAVI_SELECT_BG));
  bg.setScale(2.f, 2.f);

  // UI Sprites
  sf::Sprite charName(LOAD_TEXTURE(CHAR_NAME));
  charName.setScale(2.f, 2.f);
  charName.setPosition(10, 0);

  sf::Sprite charElement(LOAD_TEXTURE(CHAR_ELEMENT));
  charElement.setScale(2.f, 2.f);
  charElement.setPosition(10, 80);

  sf::Sprite charStat(LOAD_TEXTURE(CHAR_STAT));
  charStat.setScale(2.f, 2.f);
  charStat.setPosition(250, 0);

  sf::Sprite charInfo(LOAD_TEXTURE(CHAR_INFO_BOX));
  charInfo.setScale(2.f, 2.f);
  charInfo.setPosition(250, 250);

  // Current navi graphic
  sf::Sprite navi(*TEXTURES.GetTexture(TextureType::NAVI_MEGAMAN_ATLAS));
  navi.setScale(2.f, 2.f);
  navi.setOrigin(navi.getLocalBounds().width / 2.f, navi.getLocalBounds().height / 2.f);
  navi.setPosition(100.f, 150.f);

  // Animator for navi
  Animation naviAnimator;

  navi.setTexture(*TEXTURES.GetTexture(TextureType::NAVI_MEGAMAN_ATLAS), true);
  naviLabel->setString("Megaman.EXE");
  speedLabel->setString("2");
  attackLabel->setString("1");
  hpLabel->setString("300");

  naviAnimator = Animation("resources/navis/megaman/megaman.animation");
  naviAnimator.Load();
  naviAnimator.SetAnimation("PLAYER_IDLE");

  naviAnimator.SetFrame(1, &navi);

  // Distortion effect
  double factor = 125;

  // Transition
  sf::Shader& transition = *SHADERS.GetShader(ShaderType::TRANSITION);
  transition.setUniform("texture", sf::Shader::CurrentTexture);
  transition.setUniform("map", *TEXTURES.GetTexture(TextureType::NOISE_TEXTURE));
  transition.setUniform("progress", 0.f);
  float transitionProgress = 0.9f;
  ENGINE.RevokeShader();

  bool gotoNextScene = false;

  SmartShader pixelated = *SHADERS.GetShader(ShaderType::TEXEL_PIXEL_BLUR);

  // Load glowing pad animation (never changes/always plays)
  Animation glowpadAnimator = Animation("resources/backgrounds/select/glow_pad.animation");
  glowpadAnimator.Load();
  glowpadAnimator.SetAnimation("GLOW");
  glowpadAnimator << Animate::Mode(Animate::Mode::Loop);

  sf::Sprite glowpad(*TEXTURES.GetTexture(TextureType::GLOWING_PAD_ATLAS));
  glowpad.setScale(2.f, 2.f);
  glowpad.setPosition(40, 150);

  sf::Sprite glowbase(*TEXTURES.GetTexture(TextureType::GLOWING_PAD_BASE));
  glowbase.setScale(2.f, 2.f);
  glowbase.setPosition(40, 200);

  sf::Sprite glowbottom(*TEXTURES.GetTexture(TextureType::GLOWING_PAD_BOTTOM));
  glowbottom.setScale(2.f, 2.f);
  glowbottom.setPosition(40, 200);

  Clock clock;
  float elapsed = 0.0f;
  float totalTime = 0.f;
  while (ENGINE.Running()) {

    float elapsedSeconds = clock.restart().asSeconds();
    totalTime += elapsedSeconds;

    float FPS = 0.f;

    if (elapsedSeconds > 0.f) {
      FPS = 1.0f / elapsedSeconds;
      std::string fpsStr = std::to_string(FPS);
      fpsStr.resize(4);
      ENGINE.GetWindow()->setTitle(sf::String(std::string("FPS: ") + fpsStr));
    }

    INPUT.update();

    ENGINE.Clear();
    //ENGINE.SetView(camera.GetView());

    camera.Update(elapsed);

    ENGINE.Draw(bg);
    ENGINE.Draw(glowbottom);
    ENGINE.Draw(glowbase);
    ENGINE.Draw(charName);
    ENGINE.Draw(charElement);
    ENGINE.Draw(charStat);
    ENGINE.Draw(charInfo);

    // Draw glow pad behind everything 
    glowpadAnimator.Update(elapsed, &glowpad);
    ENGINE.Draw(glowpad);

    // Draw hp
    ENGINE.Draw(hpLabel);

    ENGINE.DrawUnderlay();
    ENGINE.DrawLayers();
    ENGINE.DrawOverlay();

    SelectedNavi prevSelect = naviSelectionIndex;

    // Scene keyboard controls
    if (!gotoNextScene && transitionProgress == 0.f) {
      if (INPUT.has(PRESSED_LEFT)) {
        selectInputCooldown -= elapsed;

        if (selectInputCooldown <= 0) {
          // Go to previous mob 
          selectInputCooldown = maxSelectInputCooldown;
          naviSelectionIndex = static_cast<SelectedNavi>((int)naviSelectionIndex - 1);

          // Number scramble effect
          numberCooldown = maxNumberCooldown;
        }
      }
      else if (INPUT.has(PRESSED_RIGHT)) {
        selectInputCooldown -= elapsed;

        if (selectInputCooldown <= 0) {
          // Go to next mob 
          selectInputCooldown = maxSelectInputCooldown;
          naviSelectionIndex = static_cast<SelectedNavi>((int)naviSelectionIndex + 1);

          // Number scramble effect
          numberCooldown = maxNumberCooldown;
        }
      }
      else {
        selectInputCooldown = 0;
      }

      if (INPUT.has(PRESSED_ACTION2)) {
        gotoNextScene = true;
        AUDIO.Play(AudioType::CHIP_DESC_CLOSE);
      }
    }

    if (elapsed > 0) {
      if (gotoNextScene) {
        transitionProgress += 1 * elapsed;
      }
      else {
        transitionProgress -= 1 * elapsed;
      }
    }

    transitionProgress = std::max(0.f, transitionProgress);
    transitionProgress = std::min(1.f, transitionProgress);

    if (transitionProgress >= 1.f) {
      break;
    }

    naviSelectionIndex = (SelectedNavi)std::max(0, (int)naviSelectionIndex);
    naviSelectionIndex = (SelectedNavi)std::min(1, (int)naviSelectionIndex);

    if (naviSelectionIndex != prevSelect) {
      factor = 125;
    }

    if (naviSelectionIndex == SelectedNavi::MEGAMAN) {
      navi.setTexture(*TEXTURES.GetTexture(TextureType::NAVI_MEGAMAN_ATLAS), true);
      naviLabel->setString("Megaman.EXE");
      speedLabel->setString("2");
      attackLabel->setString("1");
      hpLabel->setString("300");

      naviAnimator = Animation("resources/navis/megaman/megaman.animation");
      naviAnimator.Load();
      naviAnimator.SetAnimation("PLAYER_IDLE");

      naviAnimator.SetFrame(1, &navi);

    }
    else if (naviSelectionIndex == SelectedNavi::STARMAN) {
      navi.setTexture(*TEXTURES.GetTexture(TextureType::NAVI_STARMAN_ATLAS), true);
      naviLabel->setString("Starman");
      speedLabel->setString("4");
      attackLabel->setString("1");
      hpLabel->setString("200");

      naviAnimator = Animation("resources/navis/starman/starman.animation");
      naviAnimator.Load();
      naviAnimator.SetAnimation("PLAYER_IDLE");

      naviAnimator.SetFrame(1, &navi);
    }

    if (numberCooldown > 0) {
      numberCooldown -= elapsed;
      std::string newstr;

      for (int i = 0; i < naviLabel->getString().getSize(); i++) {
        double progress = (maxNumberCooldown - numberCooldown) / maxNumberCooldown;
        double index = progress * naviLabel->getString().getSize();

        if (i < (int)index) {
          newstr += naviLabel->getString()[i];
        }
        else {
          if (naviLabel->getString()[i] != ' ') {
            newstr += (char)(((rand() % (90 - 65)) + 65) + 1);
            AUDIO.Play(AudioType::TEXT, 0);
          }
          else {
            newstr += ' ';
          }
        }
      }

      int randAttack = rand() % 10;
      int randSpeed = rand() % 10;

      attackLabel->setString(std::to_string(randAttack));
      speedLabel->setString(std::to_string(randSpeed));
      naviLabel->setString(sf::String(newstr));
    }

    float progress = (maxNumberCooldown - numberCooldown) / maxNumberCooldown;

    if (progress > 1.f) progress = 1.f;

    float range = (125.f - factor) / 125.f;
    navi.setColor(sf::Color(255, 255, 255, (sf::Uint8)(255 * range)));

    sf::IntRect t = navi.getTextureRect();
    sf::Vector2u size = navi.getTexture()->getSize();
    pixelated.SetUniform("x", (float)t.left / (float)size.x);
    pixelated.SetUniform("y", (float)t.top / (float)size.y);
    pixelated.SetUniform("w", (float)t.width / (float)size.x);
    pixelated.SetUniform("h", (float)t.height / (float)size.y);
    pixelated.SetUniform("pixel_threshold", (float)(factor / 400.f));

    factor -= elapsed * 180.f;

    if (factor <= 0.f) {
      factor = 0.f;
    }

    // Refresh mob graphic origin every frame as it may change
    navi.setOrigin(navi.getTextureRect().width / 2.f, navi.getTextureRect().height / 2.f);
    hpLabel->setOrigin(hpLabel->getLocalBounds().width, 0);

    LayeredDrawable* bake = new LayeredDrawable(sf::Sprite(navi));
    bake->SetShader(pixelated);

    ENGINE.Draw(bake);
    delete bake;

    // Make a selection
    if (INPUT.has(PRESSED_ACTION1)) {
      AUDIO.Play(AudioType::CHIP_CONFIRM, 0);

      // TODO: Highlight the chosen navi symbol
    } 

    sf::Texture postprocessing = ENGINE.GetPostProcessingBuffer().getTexture(); // Make a copy
    sf::Sprite transitionPost;
    transitionPost.setTexture(postprocessing);

    transition.setUniform("progress", transitionProgress);

    bake = new LayeredDrawable(transitionPost);
    bake->SetShader(&transition);

    ENGINE.Draw(bake);
    delete bake;

    // Write contents to screen (always last step)
    ENGINE.Display();

    elapsed = static_cast<float>(clock.getElapsedTime().asSeconds());
  }
  delete font;
  delete naviFont;
  delete hpFont;
  delete naviLabel;
  delete attackLabel;
  delete speedLabel;
  delete menuLabel;
  delete hpLabel;

  ENGINE.RevokeShader();

  return naviSelectionIndex;
}