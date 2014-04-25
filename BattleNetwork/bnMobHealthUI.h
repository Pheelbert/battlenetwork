#pragma once
#include <SFML\Graphics.hpp>
using sf::Font;
using sf::Text;
class Mettaur;

class MobHealthUI : public Text
{
public:
    MobHealthUI(void);
    MobHealthUI(Mettaur* _mob);
    ~MobHealthUI(void);

    void Update();

private:
    Mettaur* mob;
    Font* font;
};