#include <SFML\Graphics.hpp>
using sf::Sprite;
using sf::IntRect;

#include "bnAnimationComponent.h"
#include "bnFileUtil.h"
#include "bnLogger.h"
#include "bnEntity.h"

AnimationComponent::AnimationComponent(Entity* _entity) {
  entity = _entity;
  currAnimationID = 0;
}

AnimationComponent::~AnimationComponent() {
}

void AnimationComponent::setup(string _name, string _path) {
  name = _name;
  path = _path;
}

void AnimationComponent::load() {
  int frameAnimationIndex = -1;
  vector<FrameList> frameLists;
  string currentState = "";
  float currentAnimationDuration = 0.0f;
  int currentWidth = 0;
  int currentHeight = 0;
  bool legacySupport = true;

  string data = FileUtil::Read(path);
  int endline = 0;
  do {
    endline = (int) data.find("\n");
    string line = data.substr(0, endline);

    // NOTE: Support older animation files until we upgrade completely...
    if (line.find("VERSION") != string::npos) {
      string version = valueOf("VERSION", line);

      std::cout << "version: " << version << "\n";

      if (version != "1.0") legacySupport = false;

    } else if (line.find("entity") != string::npos) {
      string sname = valueOf("name", line);
      assert(name == sname && "Wrong class name specified in .animation file");
    } else if (line.find("animation") != string::npos) {
      if (!frameLists.empty()) {
        //std::cout << "animation total seconds: " << sf::seconds(currentAnimationDuration).asSeconds() << "\n";
        //std::cout << "animation name push " << currentState << endl;
        animations.insert(std::make_pair(entity->GetStateFromString(currentState), frameLists.at(frameAnimationIndex)));
        currentAnimationDuration = 0.0f;
      }
      string state = valueOf("state", line);
      currentState = state;

      if (legacySupport) {
        string width = valueOf("width", line);
        string height = valueOf("height", line);
        //std::cout << "animation name in " << currentState << endl;

        currentWidth = atoi(width.c_str());
        currentHeight = atoi(height.c_str());
      }

      frameLists.push_back(FrameList());
      frameAnimationIndex++;
    }
    else if (line.find("frame") != string::npos) {
      string duration = valueOf("duration", line);
      float currentFrameDuration = (float)atof(duration.c_str());

      int currentStartx = 0;
      int currentStarty = 0;
      int originX = 0;
      int originY = 0;

      if (legacySupport) {
        string startx = valueOf("startx", line);
        string starty = valueOf("starty", line);

        currentStartx = atoi(startx.c_str());
        currentStarty = atoi(starty.c_str());
      }
      else {
        string x = valueOf("x", line);
        string y = valueOf("y", line);
        string w = valueOf("w", line);
        string h = valueOf("h", line);
        string ox = valueOf("originx", line);
        string oy = valueOf("originy", line);

        currentStartx = atoi(x.c_str());
        currentStarty = atoi(y.c_str());
        currentWidth = atoi(w.c_str());
        currentHeight = atoi(h.c_str());
        originX = atoi(ox.c_str());
        originY = atoi(oy.c_str());
      }

      currentAnimationDuration += currentFrameDuration;

      if (legacySupport) {
        frameLists.at(frameAnimationIndex).Add(currentFrameDuration, IntRect(currentStartx, currentStarty, currentWidth, currentHeight));
      }
      else {
        frameLists.at(frameAnimationIndex).Add(currentFrameDuration, IntRect(currentStartx, currentStarty, currentWidth, currentHeight), sf::Vector2i(originX, originY));
      }
    }

    data = data.substr(endline + 1);
  } while (endline > -1);

  // One more addAnimation to do
  animations.insert(std::make_pair(entity->GetStateFromString(currentState), frameLists.at(frameAnimationIndex)));
}

string AnimationComponent::valueOf(string _key, string _line) {
  int keyIndex = (int) _line.find(_key);
  // assert(keyIndex > -1 && "Key was not found in .animation file.");
  string s = _line.substr(keyIndex + _key.size() + 2);
  return s.substr(0, s.find("\""));
}

void AnimationComponent::update(float elapsed) {
  progress += elapsed / 1000.f; // to ms

  animator(progress, *entity, animations[currAnimationID], finishCallback);
}

void AnimationComponent::setAnimation(int state, std::function<void()> onFinish)
{
  progress = 0.0f;
  currAnimationID = state; 
  finishCallback = onFinish;
}
