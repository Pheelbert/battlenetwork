#pragma once
#include <SFML\Graphics.hpp>

class Camera
{
sf::Vector2f dest;
sf::Vector2f origin;
sf::Time dur;
sf::View focus;
float progress;

public:
  Camera(sf::View view);
  ~Camera();

  void Update(float elapsed);
  void MoveCamera(sf::Vector2f destination, sf::Time duration);
  void PlaceCamera(sf::Vector2f post);
  const sf::View GetView() const;
};

