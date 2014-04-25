#pragma once
#include <SFML\Graphics.hpp>
using sf::Font;
using sf::Text;
class Mob;

class MobHealthUI : public Text
{
public:
    MobHealthUI(void);
    MobHealthUI(Mob* _mob);
    ~MobHealthUI(void);

    void Update();

private:
    Mob* mob;
    Font* font;
};