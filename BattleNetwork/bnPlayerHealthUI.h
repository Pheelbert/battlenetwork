#pragma once
#include <SFML\Graphics.hpp>
using sf::Font;
using sf::Text;
using sf::Sprite;
using sf::Texture;
using sf::Drawable;
#include <sstream>
using std::ostringstream;
#include <vector>
using std::vector;

class Entity;
class Player;

class PlayerHealthUI
{
public:
    PlayerHealthUI(Entity* _entity);
    PlayerHealthUI(Player* _player);
    ~PlayerHealthUI(void);

    bool GetNextComponent(Drawable*& out);
    void Update();

private:
	int lastHP;
	int currHP;
    Player* player;
    Font* font;
    Text text;
    Sprite sprite;
    Texture* texture;
    vector<Drawable*> components;
};
