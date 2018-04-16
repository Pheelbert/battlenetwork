#pragma once
#include <SFML\Graphics.hpp>
using sf::Drawable;
using sf::RenderWindow;
using sf::VideoMode;
using sf::Event;
#include <vector>
using std::vector;

#include "bnLayered.h"

class Engine {
public:
  static Engine& GetInstance();
  void Initialize();
  void Draw(Drawable& _drawable, bool applyShaders = true);
  void Draw(Drawable* _drawable, bool applyShaders = true);
  void Draw(vector<Drawable*> _drawable, bool applyShaders = true);
  void Draw(LayeredDrawable * _drawable);
  void Draw(vector<LayeredDrawable*> _drawable);
  void Display();
  bool Running();
  void Clear();
  RenderWindow* GetWindow() const;

  void Push(LayeredDrawable* _drawable);
  void Lay(LayeredDrawable* _drawable);
  void Lay(vector<sf::Drawable*> _drawable);
  void LayUnder(sf::Drawable* _drawable);
  void DrawLayers();
  void DrawOverlay();
  void DrawUnderlay();

  void SetShader(sf::Shader* _shader);
  void RevokeShader();
private:
  Engine(void);
  ~Engine(void);

  RenderWindow* window;
  Underlay underlay;
  Layers layers;
  Overlay overlay;
  sf::RenderStates state;
};