#include "bnCamera.h"
#include <iostream>

Camera::Camera(const sf::View& view) : focus(view)
{
  progress = 1.f;
  dest = origin = focus.getCenter();
  dur = sf::milliseconds((sf::Int32)progress);
  shakeProgress = 1.f;
  shakeDur = dur;
}

void Camera::operator=(const Camera& rhs) {
  progress = rhs.progress;
  dest = rhs.dest;
  dur = rhs.dur;
  focus = rhs.focus;
  shakeProgress = rhs.shakeProgress;
  shakeDur = rhs.shakeDur;
}

Camera::~Camera()
{
}

void Camera::Update(float elapsed) {
  progress += elapsed;
  shakeProgress += elapsed;

  if (sf::Time(sf::milliseconds((sf::Int32)progress)) >= dur) {
    PlaceCamera(dest);
  }
  else {
    sf::Vector2f delta = (dest - origin) * (progress / dur.asMilliseconds()) + origin;
    PlaceCamera(delta);
  }

  if (sf::Time(sf::milliseconds((sf::Int32)shakeProgress)) < shakeDur) {
    // Drop off to zero by end of shake
    double currStress = stress * (1 - (shakeProgress / shakeDur.asMilliseconds()));

    double factor = (-currStress) + (double)((double)(currStress - (-currStress)) * (rand() / (RAND_MAX + 1.0)));
    double factor2 = (-currStress) + (double)((double)(currStress - (-currStress)) * (rand() / (RAND_MAX + 1.0)));
    sf::Vector2f offset = sf::Vector2f((float)factor, (float)factor2);

    PlaceCamera(focus.getCenter() + offset);
  }
}

void Camera::MoveCamera(sf::Vector2f destination, sf::Time duration) {
  origin = focus.getCenter();
  progress = 0;
  dest = destination;
  dur = duration;
}

void Camera::PlaceCamera(sf::Vector2f pos) { 
  focus.setCenter(pos);
  dest = focus.getCenter();
}

void Camera::OffsetCamera(sf::Vector2f offset)
{
  origin = focus.getCenter();
  focus.setCenter(origin + offset);
  dest = focus.getCenter();
}

bool Camera::IsInView(sf::Sprite& sprite) {
  float camW = focus.getSize().x;
  float camH = focus.getSize().y;

  float spriteW = sprite.getLocalBounds().width  * sprite.getScale().x;
  float spriteH = sprite.getLocalBounds().height * sprite.getScale().y;
  float spriteX = sprite.getPosition().x;
  float spriteY = sprite.getPosition().y;

  return (spriteX+spriteW >= 0 && spriteX-spriteW <= camW && spriteY+spriteH >= 0 && spriteY-spriteH <= camH);
}

void Camera::ShakeCamera(double stress, sf::Time duration)
{
  this->stress = stress;
  shakeDur = duration;
  shakeProgress = 0;
}

const sf::View Camera::GetView() const
{
  return focus;
}
