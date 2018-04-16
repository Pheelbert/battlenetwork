#include <Thor/Animations.hpp>
using thor::FrameAnimation;
using thor::Animator;
#include <SFML\Graphics.hpp>
using sf::Sprite;
using sf::IntRect;

#include "bnResourceComponent.h"
#include "bnTextureResourceManager.h"
#include "bnLogger.h"
#include "bnEntity.h"

ResourceComponent::ResourceComponent(Entity* _entity) {
  entity = _entity;
}

ResourceComponent::~ResourceComponent() {
}

void ResourceComponent::setup(string _name, string _path) {
  name = _name;
  path = _path;
}

void ResourceComponent::load() {
  int frameAnimationIndex = -1;
  vector<FrameAnimation> animations;
  string currentState = "";
  float currentDuration = 0.0f;
  int currentWidth = 0;
  int currentHeight = 0;

  string data = TextureResourceManager::GetInstance().LoadDataFromFile(path);
  int endline = 0;
  do {
    endline = data.find("\n");
    string line = data.substr(0, endline);

    if (line.find("entity") != string::npos) {
      string sname = valueOf("name", line);
      assert(name == sname && "Wrong class name specified in .animation file");
    } else if (line.find("animation") != string::npos) {
      if (!animations.empty()) {
        entity->addAnimation(entity->GetStateFromString(currentState), animations.at(frameAnimationIndex), currentDuration);
      }
      string state = valueOf("state", line);
      string duration = valueOf("duration", line);
      string width = valueOf("width", line);
      string height = valueOf("height", line);
      currentState = state;
      currentDuration = (float)atof(duration.c_str());
      currentWidth = atoi(width.c_str());
      currentHeight = atoi(height.c_str());
      animations.push_back(FrameAnimation());
      frameAnimationIndex++;
    } else if (line.find("frame") != string::npos) {
      string relative = valueOf("relative", line);
      string startx = valueOf("startx", line);
      string starty = valueOf("starty", line);
      float currentRelative = (float)atof(relative.c_str());
      int currentStartx = atoi(startx.c_str());
      int currentStarty = atoi(starty.c_str());
      animations.at(frameAnimationIndex).addFrame(currentRelative, IntRect(currentStartx, currentStarty, currentWidth, currentHeight));
    }

    data = data.substr(endline + 1);
  } while (endline > -1);
  entity->addAnimation(entity->GetStateFromString(currentState), animations.at(frameAnimationIndex), currentDuration);
}

string ResourceComponent::valueOf(string _key, string _line) {
  int keyIndex = _line.find(_key);
  assert(keyIndex > -1 && "Key was not found in .animation file.");
  string s = _line.substr(keyIndex + _key.size() + 2);
  return s.substr(0, s.find("\""));
}