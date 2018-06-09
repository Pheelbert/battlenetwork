#include <SFML\Graphics.hpp>
using sf::Sprite;
using sf::IntRect;

#include "bnAnimation.h"
#include "bnFileUtil.h"
#include "bnLogger.h"
#include "bnEntity.h"

Animation::Animation() : animator(), name(""), path("") {
  ;
}

Animation::Animation(string _name, string _path) : animator(), name(_name), path(_path) {
}

Animation::~Animation() {
}

void Animation::Load() {
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
    endline = (int)data.find("\n");
    string line = data.substr(0, endline);

    // NOTE: Support older animation files until we upgrade completely...
    if (line.find("VERSION") != string::npos) {
      string version = ValueOf("VERSION", line);

      std::cout << "version: " << version << "\n";

      if (version != "1.0") legacySupport = false;

    }
    else if (line.find("entity") != string::npos) {
      string sname = ValueOf("name", line);
      assert(name == sname && "Wrong class name specified in .animation file");
    }
    else if (line.find("animation") != string::npos) {
      if (!frameLists.empty()) {
        //std::cout << "animation total seconds: " << sf::seconds(currentAnimationDuration).asSeconds() << "\n";
        //std::cout << "animation name push " << currentState << endl;
        animations.insert(std::make_pair(currentState, frameLists.at(frameAnimationIndex)));
        currentAnimationDuration = 0.0f;
      }
      string state = ValueOf("state", line);
      currentState = state;

      if (legacySupport) {
        string width = ValueOf("width", line);
        string height = ValueOf("height", line);
        //std::cout << "animation name in " << currentState << endl;

        currentWidth = atoi(width.c_str());
        currentHeight = atoi(height.c_str());
      }

      frameLists.push_back(FrameList());
      frameAnimationIndex++;
    }
    else if (line.find("frame") != string::npos) {
      string duration = ValueOf("duration", line);
      float currentFrameDuration = (float)atof(duration.c_str());

      int currentStartx = 0;
      int currentStarty = 0;
      int originX = 0;
      int originY = 0;

      if (legacySupport) {
        string startx = ValueOf("startx", line);
        string starty = ValueOf("starty", line);

        currentStartx = atoi(startx.c_str());
        currentStarty = atoi(starty.c_str());
      }
      else {
        string x = ValueOf("x", line);
        string y = ValueOf("y", line);
        string w = ValueOf("w", line);
        string h = ValueOf("h", line);
        string ox = ValueOf("originx", line);
        string oy = ValueOf("originy", line);

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

  // One more addAnimation to do if file is good
  if (frameAnimationIndex >= 0) {
    animations.insert(std::make_pair(currentState, frameLists.at(frameAnimationIndex)));
  }
}

string Animation::ValueOf(string _key, string _line) {
  int keyIndex = (int)_line.find(_key);
  // assert(keyIndex > -1 && "Key was not found in .animation file.");
  string s = _line.substr(keyIndex + _key.size() + 2);
  return s.substr(0, s.find("\""));
}

void Animation::Update(float elapsed, sf::Sprite* target) {
  progress += elapsed / 1000.f; // to ms

  animator(progress, *target, animations[currAnimation]);
}

void Animation::SetFrame(int frame, sf::Sprite * target)
{
  animator.SetFrame(frame, *target, animations[currAnimation]);
}

void Animation::SetAnimation(string state) {
   animator.Clear();
   progress = 0.0f;
   currAnimation = state;
}

Animation & Animation::operator<<(Animate::On & rhs)
{
  animator << rhs;
  return *this;
}

void Animation::operator<<(std::function<void()> onFinish)
{
  animator << onFinish;
}
