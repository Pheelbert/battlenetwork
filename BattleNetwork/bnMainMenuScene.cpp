#include <time.h>
#include "bnMainMenuScene.h"
#include "bnSelectMobScene.h"
#include "bnMemory.h"
#include "bnCamera.h"
#include "bnInputManager.h"
#include "bnAudioResourceManager.h"
#include "bnShaderResourceManager.h"
#include "bnTextureResourceManager.h"
#include "bnEngine.h"
#include "bnAnimation.h"
#include "bnLanBackground.h"
#include <SFML/Graphics.hpp>
using sf::RenderWindow;
using sf::VideoMode;
using sf::Clock;
using sf::Event;
using sf::Font;

class OWTile {
  double x;
  double y;
  double w;
  double h;

public:
  OWTile(double x, double y) : x(x), y(y) {
    w = 48 / 2.0;
    h = 30 / 2.0;
  }

  const double GetY() const { return y; } 
  const double GetX() const { return x; } 
  const double GetW() const { return w; } 
  const double GetH() const { return h; }

  void Move(double offx, double offy) {
    x += offx;
    y += offy;
  }

};

bool SortByDepth(const OWTile* lhs, const OWTile* rhs)
{
  return lhs->GetY() < rhs->GetY();
}

std::vector<OWTile*> map;

int MainMenuScene::Run()
{
  // Begin the chain
  map.push_back(new OWTile(240, 160));

  while (map.size() < 10) {
    double x = 0;
    double y = 0;
    x += map.back()->GetX() + (map.back()->GetW()*1.9);
    y += map.back()->GetY() + (map.back()->GetH()*1.5);

    OWTile* tile = new OWTile(x, y);

    map.push_back(tile);
  }

  std::sort(map.begin(), map.end());

  OWTile* mainStrip = map.back();

  Camera& camera(ENGINE.GetCamera());

  // Selection input delays
  double maxSelectInputCooldown = 1000.0f / 2.f; // half of a second
  double selectInputCooldown = maxSelectInputCooldown;                                     

  // ui sprite maps
  sf::Sprite ui(*TEXTURES.GetTexture(TextureType::MAIN_MENU_UI));
  ui.setScale(2.f, 2.f);
  Animation uiAnimator("ui", "resources/ui/main_menu_ui.animation");
  uiAnimator.Load();

  // Stream menu music 
  AUDIO.Stream("resources/loops/loop_navi_customizer.ogg", true);

  // Transition
  sf::Shader& transition = *SHADERS.GetShader(ShaderType::TRANSITION);
  transition.setUniform("texture", sf::Shader::CurrentTexture);
  transition.setUniform("map", *TEXTURES.GetTexture(TextureType::NOISE_TEXTURE));
  transition.setUniform("progress", 0.f);
  float transitionProgress = 1.f;
  ENGINE.RevokeShader();

  Clock clock;
  float elapsed = 0.0f;
  float totalTime = 0.f;
  int menuSelectionIndex = 0;

  sf::Sprite overlay(*TEXTURES.GetTexture(TextureType::MAIN_MENU));
  overlay.setScale(2.f, 2.f);

  sf::Sprite ow(*TEXTURES.GetTexture(TextureType::MAIN_MENU_OW));
  ow.setScale(2.f, 2.f);

  Background* bg = new LanBackground();

  sf::Sprite owNavi(*TEXTURES.GetTexture(TextureType::NAVI_STARMAN_ATLAS));
  owNavi.setScale(2.f, 2.f);
  Animation naviAnimator("owNavi", "resources/navis/starman/starman.animation");
  naviAnimator.Load();
  naviAnimator.SetAnimation("navi_starman_walk_DownLR");
  naviAnimator << Animate::Mode(Animate::Mode::Loop);

  bool gotoNextScene = false;

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
    ENGINE.SetView(camera.GetView());

    camera.Update(elapsed);

    if(map.size() < 100) {
      double x = 0;
      double y = 0;

      int dir = rand() % 3;

      x += mainStrip->GetX() + (mainStrip->GetW()*1.9);
      y += mainStrip->GetY() + (mainStrip->GetH()*1.5);

      OWTile* tile = new OWTile(x, y);

      map.push_back(tile);

      mainStrip = tile;

      int depth = 0;

      OWTile* chain = tile;

      while (rand() % 50 > 10 && depth < 10) {
        depth++;

        x = 0;
        y = 0;

        if (dir == 0) {
          x += chain->GetX() + (chain->GetW()*1.9);
          y += chain->GetY() - (chain->GetH()*1.5);

          OWTile* tile = new OWTile(x, y);
          chain = tile;

          map.push_back(tile);
        }
        else if(dir ==1) {
          x += chain->GetX() - (chain->GetW()*1.9);
          y += chain->GetY() + (chain->GetH()*1.5);

          OWTile* tile = new OWTile(x, y);
          chain = tile;

          map.push_back(tile);
        }
        else if(depth > 1) {
          x += chain->GetX() + (chain->GetW()*1.9);
          y += chain->GetY() + (chain->GetH()*1.5);

          OWTile* tile = new OWTile(x, y);
          chain = tile;

          map.push_back(tile);
        }
      }

      std::sort(map.begin(), map.end(), SortByDepth);
    }

    std::vector<OWTile*>::iterator iter = map.begin();

    bg->Draw();

    while (iter != map.end()) {
      if ((*iter)->GetY() < 10 || (*iter)->GetX() < -50) {
        delete (*iter);
        *iter = nullptr;
        iter = map.erase(iter);

        continue;
      }

      ow.setOrigin(24.f, 15.f);
      ow.setPosition((float)((*iter)->GetX()), (float)((*iter)->GetY()));


      int alpha = (int)std::min((double)255, ((*iter)->GetY() / 50.0) * 255);

      alpha = (int)std::min((double)alpha, ((*iter)->GetY() / 220.0) * 255);

      ow.setColor(sf::Color(255, 255, 255, alpha));

      (*iter)->Move(-50.0*(elapsed/1000.0), (-24.495*(elapsed / 1000.0)));
      ENGINE.Draw(ow);

      iter++;
    }

    // Draw navi moving
    naviAnimator.Update(elapsed, &owNavi);
    owNavi.setPosition(320, 195);
    ENGINE.Draw(owNavi);

    int lastMenuSelectionIndex = menuSelectionIndex;

    // Scene keyboard controls
    if (!gotoNextScene && transitionProgress == 0.f) {
      if (INPUT.has(PRESSED_UP)) {
        selectInputCooldown -= elapsed;

        if (selectInputCooldown <= 0) {
          // Go to previous mob 
          selectInputCooldown = maxSelectInputCooldown;
          menuSelectionIndex--;
        }
      }
      else if (INPUT.has(PRESSED_DOWN)) {
        selectInputCooldown -= elapsed;

        if (selectInputCooldown <= 0) {
          // Go to next mob 
          selectInputCooldown = maxSelectInputCooldown;
          menuSelectionIndex++;
        }
      }
      else {
        selectInputCooldown = 0;
      }

      if (INPUT.has(PRESSED_ACTION1)) {
        // Mob select
        if (menuSelectionIndex == 3) {
          gotoNextScene = true;
          AUDIO.Play(AudioType::CHIP_DESC);
        }
      }
    }

    if (gotoNextScene) {
      transitionProgress += 0.05f;
    }
    else {
      transitionProgress -= 0.05f;
    }

    transitionProgress = std::max(0.f, transitionProgress);
    transitionProgress = std::min(1.f, transitionProgress);

    if (transitionProgress == 1.f) {
      int result = SelectMobScene::Run();

      // reset internal clock (or everything will teleport)
      elapsed = static_cast<float>(clock.getElapsedTime().asMilliseconds());
      std::cout << "time slept: " << elapsed << "\n";
      clock.restart();
      elapsed = 0;

      if (result == 0) {
        break;
      }

      gotoNextScene = false;
    }

    menuSelectionIndex = std::max(0, menuSelectionIndex);
    menuSelectionIndex = std::min(3, menuSelectionIndex);

    if (menuSelectionIndex != lastMenuSelectionIndex) {
      AUDIO.Play(AudioType::CHIP_SELECT, 1);
    }

    ENGINE.Draw(overlay);

    uiAnimator.SetAnimation("CHIP_FOLDER");

    if (menuSelectionIndex == 0) {
      uiAnimator.SetFrame(2, &ui);
      ui.setPosition(50.f, 50.f);
      ENGINE.Draw(ui);

      uiAnimator.SetAnimation("CHIP_FOLDER_LABEL");
      uiAnimator.SetFrame(2, &ui);
      ui.setPosition(100.f, 50.f);
      ENGINE.Draw(ui);
    }
    else {
      uiAnimator.SetFrame(1, &ui);
      ui.setPosition(20.f, 50.f);
      ENGINE.Draw(ui);

      uiAnimator.SetAnimation("CHIP_FOLDER_LABEL");
      uiAnimator.SetFrame(1, &ui);
      ui.setPosition(100.f, 50.f);
      ENGINE.Draw(ui);
    }

    uiAnimator.SetAnimation("LIBRARY");

    if (menuSelectionIndex == 1) {
      uiAnimator.SetFrame(2, &ui);
      ui.setPosition(50.f, 120.f);
      ENGINE.Draw(ui);

      uiAnimator.SetAnimation("LIBRARY_LABEL");
      uiAnimator.SetFrame(2, &ui);
      ui.setPosition(100.f, 120.f);
      ENGINE.Draw(ui);
    }
    else {
      uiAnimator.SetFrame(1, &ui);
      ui.setPosition(20.f, 120.f);
      ENGINE.Draw(ui);

      uiAnimator.SetAnimation("LIBRARY_LABEL");
      uiAnimator.SetFrame(1, &ui);
      ui.setPosition(100.f, 120.f);
      ENGINE.Draw(ui);
    }

    uiAnimator.SetAnimation("NAVI");

    if (menuSelectionIndex == 2) {
      uiAnimator.SetFrame(2, &ui);
      ui.setPosition(50.f, 190.f);
      ENGINE.Draw(ui);

      uiAnimator.SetAnimation("NAVI_LABEL");
      uiAnimator.SetFrame(2, &ui);
      ui.setPosition(100.f, 190.f);
      ENGINE.Draw(ui);
    }
    else {
      uiAnimator.SetFrame(1, &ui);
      ui.setPosition(20.f, 190.f);
      ENGINE.Draw(ui);

      uiAnimator.SetAnimation("NAVI_LABEL");
      uiAnimator.SetFrame(1, &ui);
      ui.setPosition(100.f, 190.f);
      ENGINE.Draw(ui);
    }

    uiAnimator.SetAnimation("MOB_SELECT");

    if (menuSelectionIndex == 3) {
      uiAnimator.SetFrame(2, &ui);
      ui.setPosition(50.f, 260.f);
      ENGINE.Draw(ui);

      uiAnimator.SetAnimation("MOB_SELECT_LABEL");
      uiAnimator.SetFrame(2, &ui);
      ui.setPosition(100.f, 260.f);
      ENGINE.Draw(ui);
    }
    else {
      uiAnimator.SetFrame(1, &ui);
      ui.setPosition(20.f, 260.f);
      ENGINE.Draw(ui);

      uiAnimator.SetAnimation("MOB_SELECT_LABEL");
      uiAnimator.SetFrame(1, &ui);
      ui.setPosition(100.f, 260.f);
      ENGINE.Draw(ui);
    }

    ENGINE.Draw(ui);

    sf::Texture postprocessing = ENGINE.GetPostProcessingBuffer().getTexture(); // Make a copy
    sf::Sprite transitionPost;
    transitionPost.setTexture(postprocessing);

    transition.setUniform("progress", transitionProgress);

    LayeredDrawable* bake = new LayeredDrawable(transitionPost);
    bake->SetShader(&transition);

    ENGINE.Draw(bake);
    delete bake;

    // Write contents to screen (always last step)
    ENGINE.Display();

    elapsed = static_cast<float>(clock.getElapsedTime().asMilliseconds());
  }

  AUDIO.StopStream();
  ENGINE.RevokeShader();

  return 0; // signal game over to the main stack
}