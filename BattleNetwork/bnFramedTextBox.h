#pragma once
#include "bnTextBox.h"

class FramedTextBox : public TextBox {
private:
  mutable sf::Sprite frame; // Size is inherited from this object 

public:
  FramedTextBox(sf::Texture& frameTexture, int characterSize = 15, std::string fontPath = "resources/fonts/dr_cain_terminal.ttf") : TextBox(frameTexture.getSize().x, frameTexture.getSize().y, characterSize.fontPath) {
    frame = sf::Sprite(frameTexture);
  }

  virtual ~FramedTextBox() { }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
  {
    frame.setScale(this->getScale());
    frame.setPosition(this->getPosition());
    frame.setRotation(this->getRotation());

    target.draw(frame);
  }
};