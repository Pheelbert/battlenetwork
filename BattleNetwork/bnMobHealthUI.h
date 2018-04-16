#pragma once
#include <SFML\Graphics.hpp>
using sf::Font;
using sf::Text;
class Entity;

class MobHealthUI : public Text {
public:
  MobHealthUI(void);
  MobHealthUI(Entity* _mob);
  ~MobHealthUI(void);

  void Update();

private:
  Entity * mob;
  Font* font;
};