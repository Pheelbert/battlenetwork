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

#define UI_LEFT_POS_MAX 10
#define UI_RIGHT_POS_MAX 300
#define UI_TOP_POS_MAX 0
#define UI_SPACING 55

#define UI_LEFT_POS_START -300
#define UI_RIGHT_POS_START 640
#define UI_TOP_POS_START 250

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
  sf::Text *naviLabel = new sf::Text("No Data", *naviFont);
  naviLabel->setPosition(30.f, 18.f);
  naviLabel->setOutlineColor(sf::Color(48, 56, 80));
  naviLabel->setOutlineThickness(2.f);
  naviLabel->setScale(0.8f, 0.8f);

  sf::Text *attackLabel = new sf::Text("1", *naviFont);
  attackLabel->setPosition(410.f, 15.f);
  attackLabel->setOutlineColor(sf::Color(48, 56, 80));
  attackLabel->setOutlineThickness(2.f);
  attackLabel->setScale(0.8f, 0.8f);

  sf::Text *speedLabel = new sf::Text("1", *naviFont);
  speedLabel->setPosition(410.f, 60.f);
  speedLabel->setOutlineColor(sf::Color(48, 56, 80));
  speedLabel->setOutlineThickness(2.f);
  speedLabel->setScale(0.8f, 0.8f);

  sf::Text *hpLabel = new sf::Text("20", *naviFont);
  hpLabel->setOutlineColor(sf::Color(48, 56, 80));
  hpLabel->setOutlineThickness(2.f);
  hpLabel->setScale(0.8f, 0.8f);
  //hpLabel->setOrigin(hpLabel->getLocalBounds().width, 0);
  hpLabel->setPosition(sf::Vector2f(480.f, 120.0f));

  float maxNumberCooldown = 0.5;
  float numberCooldown = maxNumberCooldown; // half a second
  SelectedNavi naviSelectionIndex = (SelectedNavi)currentNavi;

  // select menu graphic
  sf::Sprite bg(LOAD_TEXTURE(NAVI_SELECT_BG));
  bg.setScale(2.f, 2.f);

  // UI Sprites
  double UI_RIGHT_POS = UI_RIGHT_POS_START;
  double UI_LEFT_POS = UI_LEFT_POS_START;
  double UI_TOP_POS = UI_TOP_POS_START;

  sf::Sprite charName(LOAD_TEXTURE(CHAR_NAME));
  charName.setScale(2.f, 2.f);
  charName.setPosition(UI_LEFT_POS, 10);

  sf::Sprite charElement(LOAD_TEXTURE(CHAR_ELEMENT));
  charElement.setScale(2.f, 2.f);
  charElement.setPosition(UI_LEFT_POS, 80);

  sf::Sprite charStat(LOAD_TEXTURE(CHAR_STAT));
  charStat.setScale(2.f, 2.f);
  charStat.setPosition(UI_RIGHT_POS, UI_TOP_POS);

  sf::Sprite charInfo(LOAD_TEXTURE(CHAR_INFO_BOX));
  charInfo.setScale(2.f, 2.f);
  charInfo.setPosition(UI_RIGHT_POS, 170);

  // Current navi graphic
  sf::Sprite navi(LOAD_TEXTURE(NAVI_MEGAMAN_ATLAS));
  navi.setScale(2.f, 2.f);
  navi.setOrigin(navi.getLocalBounds().width / 2.f, navi.getLocalBounds().height / 2.f);
  navi.setPosition(100.f, 150.f);

  // Animator for navi
  Animation naviAnimator;

  navi.setTexture(LOAD_TEXTURE(NAVI_MEGAMAN_ATLAS), true);
  naviLabel->setString("Megaman.EXE");
  speedLabel->setString("2");
  attackLabel->setString("1");
  hpLabel->setString("300");

  naviAnimator = Animation("resources/navis/megaman/megaman.animation");
  naviAnimator.Load();
  naviAnimator.SetAnimation("PLAYER_IDLE");
  naviAnimator << Animate::Mode(Animate::Mode::Loop);

  naviAnimator.SetFrame(1, &navi);

  // Distortion effect
  double factor = 125;

  // Transition
  sf::Shader& transition = LOAD_SHADER(TRANSITION);
  transition.setUniform("texture", sf::Shader::CurrentTexture);
  transition.setUniform("map", LOAD_TEXTURE(NOISE_TEXTURE));
  transition.setUniform("progress", 0.f);
  float transitionProgress = 0.9f;
  ENGINE.RevokeShader();

  bool gotoNextScene = false;

  SmartShader pixelated = LOAD_SHADER(TEXEL_PIXEL_BLUR);

  // Load glowing pad animation (never changes/always plays)
  Animation glowpadAnimator = Animation("resources/backgrounds/select/glow_pad.animation");
  glowpadAnimator.Load();
  glowpadAnimator.SetAnimation("GLOW");
  glowpadAnimator << Animate::Mode(Animate::Mode::Loop);

  sf::Sprite glowpad(LOAD_TEXTURE(GLOWING_PAD_ATLAS));
  glowpad.setScale(2.f, 2.f);
  glowpad.setPosition(37, 135);

  sf::Sprite glowbase(LOAD_TEXTURE(GLOWING_PAD_BASE));
  glowbase.setScale(2.f, 2.f);
  glowbase.setPosition(40, 200);

  sf::Sprite glowbottom(LOAD_TEXTURE(GLOWING_PAD_BOTTOM));
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

    charName.setPosition(UI_LEFT_POS, charName.getPosition().y);
    ENGINE.Draw(charName);

    charElement.setPosition(UI_LEFT_POS, charElement.getPosition().y);
    ENGINE.Draw(charElement);

    // Draw stat box three times for three diff. properties
    double charStat1Max = 10;

    if (UI_TOP_POS < charStat1Max)
      charStat.setPosition(UI_RIGHT_POS, charStat1Max);
    else
      charStat.setPosition(UI_RIGHT_POS, UI_TOP_POS);
    ENGINE.Draw(charStat);

    // 2nd stat box
    double charStat2Max = 10 + UI_SPACING;

    if (UI_TOP_POS < charStat2Max)
      charStat.setPosition(UI_RIGHT_POS, charStat2Max);
    else
      charStat.setPosition(UI_RIGHT_POS, UI_TOP_POS);
    ENGINE.Draw(charStat);

    // 3rd stat box
    double charStat3Max = 10 + (UI_SPACING * 2);

    if (UI_TOP_POS < charStat3Max)
      charStat.setPosition(UI_RIGHT_POS, charStat3Max);
    else
      charStat.setPosition(UI_RIGHT_POS, UI_TOP_POS);
    ENGINE.Draw(charStat);

    // SP. Info box
    charInfo.setPosition(UI_RIGHT_POS, charInfo.getPosition().y);
    ENGINE.Draw(charInfo);

    // Draw glow pad behind everything 
    glowpadAnimator.Update(elapsed, &glowpad);
    ENGINE.Draw(glowpad);

    // Draw labels
    ENGINE.Draw(naviLabel);
    ENGINE.Draw(hpLabel);
    ENGINE.Draw(speedLabel);
    ENGINE.Draw(attackLabel);

    ENGINE.DrawUnderlay();
    ENGINE.DrawLayers();
    ENGINE.DrawOverlay();

    naviAnimator.Update(elapsed, &navi);

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

      if (INPUT.has(PRESSED_B)) {
        gotoNextScene = true;
        AUDIO.Play(AudioType::CHIP_DESC_CLOSE);
      }
    }

    if (elapsed > 0) {
      if (gotoNextScene) {
        if (UI_TOP_POS > 250 && UI_LEFT_POS < -300) {
          transitionProgress += 1 * elapsed;
        }
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

      if (naviSelectionIndex == SelectedNavi::MEGAMAN) {
        naviAnimator = Animation("resources/navis/megaman/megaman.animation");
        naviAnimator.Load();
        naviAnimator.SetAnimation("PLAYER_IDLE");
        naviAnimator << Animate::Mode(Animate::Mode::Loop);
      }
      else if (naviSelectionIndex == SelectedNavi::STARMAN) {
        naviAnimator = Animation("resources/navis/starman/starman.animation");
        naviAnimator.Load();
        naviAnimator.SetAnimation("PLAYER_IDLE");
        naviAnimator << Animate::Mode(Animate::Mode::Loop);
      }
    }

    if (naviSelectionIndex == SelectedNavi::MEGAMAN) {
      navi.setTexture(*TEXTURES.GetTexture(TextureType::NAVI_MEGAMAN_ATLAS), false);
      naviLabel->setString("Megaman");
      speedLabel->setString("2x");
      attackLabel->setString("1 - 10");
      hpLabel->setString("300");
    }
    else if (naviSelectionIndex == SelectedNavi::STARMAN) {
      navi.setTexture(*TEXTURES.GetTexture(TextureType::NAVI_STARMAN_ATLAS), false);
      naviLabel->setString("Starman");
      speedLabel->setString("4x");
      attackLabel->setString("1 - 10");
      hpLabel->setString("200");
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
            AUDIO.Play(AudioType::TEXT, AudioPriority::LOWEST);
          }
          else {
            newstr += ' ';
          }
        }
      }

      int randAttack = rand() % 10;
      int randSpeed = rand() % 10;

      //attackLabel->setString(std::to_string(randAttack));
      //speedLabel->setString(std::to_string(randSpeed));
      naviLabel->setString(sf::String(newstr));
    }

    float progress = (maxNumberCooldown - numberCooldown) / maxNumberCooldown;

    if (progress > 1.f) progress = 1.f;

    // Update UI slide in
    if (!gotoNextScene) {
      if (UI_RIGHT_POS > UI_RIGHT_POS_MAX) {
        UI_RIGHT_POS -= elapsed * 500;
      }
      else {
        UI_RIGHT_POS = UI_RIGHT_POS_MAX;
        UI_TOP_POS -= elapsed * 500;

        if(UI_TOP_POS < UI_TOP_POS_MAX) 
          UI_TOP_POS = UI_TOP_POS_MAX;
      }

      if (UI_LEFT_POS < UI_LEFT_POS_MAX) {
        static bool once = true;

        if (once) {
          AUDIO.Play(AudioType::TOSS_ITEM); once = false;
        }

        UI_LEFT_POS += elapsed * 500;
      }
      else {
        UI_LEFT_POS = UI_LEFT_POS_MAX;
      }
    }
    else {
      if (UI_TOP_POS < UI_TOP_POS_START) {
        UI_TOP_POS += elapsed * 500;
      }
      else {
        UI_RIGHT_POS += elapsed * 500;

        if(UI_RIGHT_POS > UI_RIGHT_POS_START/2) // Be quicker at leave than startup
          UI_LEFT_POS -= elapsed * 500;
      }
    }

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
    float xpos = ((glowbase.getTextureRect().width / 2.0f)*glowbase.getScale().x) + glowbase.getPosition().x;
    navi.setPosition(xpos, glowbase.getPosition().y+10);
    hpLabel->setOrigin(hpLabel->getLocalBounds().width, 0);

    LayeredDrawable* bake = new LayeredDrawable(sf::Sprite(navi));
    bake->SetShader(pixelated);

    ENGINE.Draw(bake);
    delete bake;

    // Make a selection
    if (INPUT.has(PRESSED_A)) {
      AUDIO.Play(AudioType::CHIP_CONFIRM, AudioPriority::LOW);

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
  delete naviLabel;
  delete attackLabel;
  delete speedLabel;
  delete menuLabel;
  delete hpLabel;

  ENGINE.RevokeShader();

  return naviSelectionIndex;
}