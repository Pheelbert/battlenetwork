#include <Thor/Animations.hpp>
using thor::FrameAnimation;
using thor::Animator;
#include <SFML\Graphics.hpp>
using sf::Sprite;
using sf::IntRect;

#include "bnAnimationComponent.h"
#include "bnTextureResourceManager.h"
#include "bnLogger.h"
#include "bnEntity.h"

AnimationComponent::AnimationComponent(Entity* _entity) {
  entity = _entity;
  animator = new Animator<sf::Sprite, int>(animations);
}

AnimationComponent::~AnimationComponent() {
  if (animator) {
    delete animator;
  }
}

void AnimationComponent::setup(string _name, string _path) {
  name = _name;
  path = _path;
}

void AnimationComponent::load() {
  int frameAnimationIndex = -1;
  vector<FrameAnimation> frames;
  string currentState = "";
  float currentAnimationDuration = 0.0f;
  int currentWidth = 0;
  int currentHeight = 0;

  string data = TextureResourceManager::GetInstance().LoadDataFromFile(path);
  int endline = 0;
  do {
    endline = (int) data.find("\n");
    string line = data.substr(0, endline);

    if (line.find("entity") != string::npos) {
      string sname = valueOf("name", line);
      assert(name == sname && "Wrong class name specified in .animation file");
    } else if (line.find("animation") != string::npos) {
      if (!frames.empty()) {
        // std::cout << "animation total seconds: " << sf::seconds(currentAnimationDuration).asSeconds() << "\n";
        animations.addAnimation(entity->GetStateFromString(currentState), frames.at(frameAnimationIndex), sf::seconds(currentAnimationDuration));
        currentAnimationDuration = 0.0f;
      }
      string state = valueOf("state", line);
      string width = valueOf("width", line);
      string height = valueOf("height", line);
      currentState = state;
      currentWidth = atoi(width.c_str());
      currentHeight = atoi(height.c_str());
      frames.push_back(FrameAnimation());
      frameAnimationIndex++;
    } else if (line.find("frame") != string::npos) {
      string duration = valueOf("duration", line);
      string startx = valueOf("startx", line);
      string starty = valueOf("starty", line);
      float currentFrameDuration = (float)atof(duration.c_str());
      // std::cout << "seconds: " << sf::seconds(currentFrameDuration).asSeconds() << "\n";

      currentAnimationDuration += currentFrameDuration;
      int currentStartx = atoi(startx.c_str());
      int currentStarty = atoi(starty.c_str());
      frames.at(frameAnimationIndex).addFrame(currentFrameDuration, IntRect(currentStartx, currentStarty, currentWidth, currentHeight));
    }

    data = data.substr(endline + 1);
  } while (endline > -1);

  // One more addAnimation to do
  animations.addAnimation(entity->GetStateFromString(currentState), frames.at(frameAnimationIndex), sf::seconds(currentAnimationDuration));
}

string AnimationComponent::valueOf(string _key, string _line) {
  int keyIndex = (int) _line.find(_key);
  assert(keyIndex > -1 && "Key was not found in .animation file.");
  string s = _line.substr(keyIndex + _key.size() + 2);
  return s.substr(0, s.find("\""));
}

void AnimationComponent::update(float elapsed) {
  animator->update(sf::milliseconds((sf::Int32)elapsed));
  animator->animate(*entity);
}

void AnimationComponent::setAnimation(int state, std::function<void()> onFinish)
{
  /*
    NOTE: Very hack-ish to use thor::Playback::loop() like this. We queue the animation 
    along with a callback after the animation completes. Then we make it play again and loop forever. 
    This is because some animations are short, single frames (like IDLE states) and while we'd like 
    them to stop on end, Thor gives us no immediate way to do this. In fact Thor dequeues the animation 
    completely and our Entitys draw empty. 

    There might be a way we can add an onFinish event notifier for one-frame animations to prevent the update() 
    timer for the animator. No further work has been done to confirm this solution.
  */
  if (onFinish) {
    animator->play() << state << thor::Playback::notify(onFinish) << thor::Playback::loop(state);
  }
  else {
    animator->play() << state << thor::Playback::loop(state);
  }
}
