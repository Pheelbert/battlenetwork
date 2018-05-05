#include "bnCamera.h"
#include <iostream>

Camera::Camera(sf::View view)
{
  focus = view;
  progress = 1.f;
  dest = origin = focus.getCenter();
  dur = sf::milliseconds((sf::Int32)progress);
}


Camera::~Camera()
{
}

void Camera::Update(float elapsed) {
  progress += elapsed;

  if (sf::Time(sf::milliseconds((sf::Int32)progress)) > dur) {
    PlaceCamera(dest);
    return;
  }

  sf::Vector2f delta = (dest - origin) * (progress / dur.asMilliseconds()) + origin;
  PlaceCamera(delta);
}

void Camera::MoveCamera(sf::Vector2f destination, sf::Time duration) {
  origin = focus.getCenter();
  progress = 0;
  dest = destination;
  dur = duration;
}

void Camera::PlaceCamera(sf::Vector2f pos) { 
  focus.setCenter(pos);
}

const sf::View Camera::GetView() const
{
  return focus;
}
