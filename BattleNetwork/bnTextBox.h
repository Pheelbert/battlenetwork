#pragma once

#include "bnTextureResourceManager.h"
#include "bnAudioResourceManager.h"
#include <SFML\Graphics.hpp>

class TextBox : public sf::Drawable, public sf::Transformable {
private:
  sf::Font* font;
  mutable sf::Text text;
  double speed; // characters per second (default is 10)
  double progress;
  int areaWidth, areaHeight;
  std::string message;
  int charIndex;
  bool play;
  bool mute;
  int charSize;
public:
  TextBox(int width, int height, int characterSize = 15, std::string fontPath = "resources/fonts/dr_cain_terminal.ttf") {
    font = TEXTURES.LoadFontFromFile(fontPath);
    text = sf::Text();
    message = "";
    areaWidth = width;
    areaHeight = height;
    speed = 10;
    charIndex = 0;
    play = true;
    mute = false;
    progress = 0;
    charSize = characterSize;
  }

  ~TextBox() {
    delete font;
  }

  void Mute(bool enabled = true) {
    mute = enabled;
  }

  void Unmute() {
    Mute(false);
  }

  void FormatToFit() {
    if (message.empty())
      return;
 
    text = sf::Text(message, *font);
    text.setCharacterSize(charSize);

    sf::Text prevText = text;

    int index = 0;
    int line = 0;
    int wordIndex = -1; // If we are breaking on a word
    while (index < message.size()) {
      if (message[index] != ' ' && wordIndex == -1) {
        wordIndex = index;
      }
      else if (message[index] == ' ' && wordIndex > -1) {
        wordIndex = -1;
      }

      if(index > 0)
        prevText.setString(message.substr(0, index - 1));

      text.setString(message.substr(0, index));

      double prevWidth = (index > 0) ? prevText.getGlobalBounds().width : 0;
      double prevHeight = (index > 0) ? prevText.getGlobalBounds().height : 0;
      double width = text.getGlobalBounds().left + prevWidth;
      double height = text.getGlobalBounds().top + prevHeight;

      if (width > areaWidth) {
        if (wordIndex > -1) {
          // Line break at the last word
          while (message[index] == ' ' && index < message.size()) { index++; }
          message.insert(wordIndex, "\n");
          index = wordIndex; // Start counting from here
          while (message[index] == ' ' && index < message.size()) { index++; }
        }
        else {
          // Line break at the next word
          while (message[index] == ' ' && index < message.size()) { index++; }
          message.insert(index, "\n");
          while (message[index] == ' ' && index < message.size()) { index++; }
        }
        line++;
        wordIndex = -1;
      }

      index++;
    }

    // update final text 
    text.setString(message);
  }

  void SetSpeed(const double speed) {
    this->speed = speed;
  }

  void SetMessage(const std::string message) {
    this->message = message;
    charIndex = 0;
    progress = 0;
    FormatToFit();
  }

  void Play(const bool play = true) {
    this->play = play;
  }

  void Stop() {
    Play(false);
  }

  void Update(const double elapsed) {
    if (!play) return;

    double charIndexIter = 0;
    progress += elapsed;

    double simulate = progress;
    while (simulate > 0 && speed > 0) {
      simulate -= 1.0/speed;

      if (charIndexIter++ > charIndex && charIndex < message.size()) {
        charIndex++;
        text.setString(message.substr(0, charIndex));

        while (message[charIndex] == ' ' && charIndex < message.size())
          charIndex++;

        if (!mute && message[charIndex] != ' ') {
          AUDIO.Play(AudioType::TEXT);
        }
      }
    }
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
  {
    if (message.empty())
      return;

    // apply the transform
    states.transform *= getTransform();

    text.setPosition(this->getPosition());
    text.setScale(this->getScale());
    text.setRotation(this->getRotation());

    target.draw(text);
  }
};
