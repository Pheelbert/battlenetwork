#pragma once
#include <SFML\Graphics.hpp>

class Camera
{
sf::Vector2f dest;
sf::Vector2f origin;
sf::Time dur;
sf::Time shakeDur;
double stress;
sf::View& focus;
float progress;
float shakeProgress;

public:
  Camera(sf::View& view);
  void operator=(const Camera& rhs); // copy ctor
  ~Camera();

  void Update(float elapsed);
  void MoveCamera(sf::Vector2f destination, sf::Time duration);
  void PlaceCamera(sf::Vector2f pos);
  void ShakeCamera(double stress, sf::Time duration);
  const sf::View GetView() const;
};

