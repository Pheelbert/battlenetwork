#include <SFML\Window.hpp>
using sf::Event;
using sf::Keyboard;
#include "bnEngine.h"
#include "bnControllableComponent.h"
#include "bnDirection.h"

ControllableComponent& ControllableComponent::GetInstance() {
  static ControllableComponent instance;
  return instance;
}

ControllableComponent::ControllableComponent()
  : events(vector<ControllableEvent>()) {
}


ControllableComponent::~ControllableComponent() {
}

void ControllableComponent::update() {
  this->events.clear();
  Event event;
  while (ENGINE.GetWindow()->pollEvent(event)) {
    if (event.type == Event::Closed) {
      ENGINE.GetWindow()->close();
    }

    if (Event::KeyPressed == event.type) {
      if (Keyboard::Up == event.key.code) {
        events.push_back(PRESSED_UP);
      } else if (Keyboard::Left == event.key.code) {
        events.push_back(PRESSED_LEFT);
      } else if (Keyboard::Down == event.key.code) {
        events.push_back(PRESSED_DOWN);
      } else if (Keyboard::Right == event.key.code) {
        events.push_back(PRESSED_RIGHT);
      } else if (Keyboard::Space == event.key.code) {
        events.push_back(PRESSED_ACTION1); // shoot 
      } else if (Keyboard::RControl == event.key.code) {
        events.push_back(PRESSED_ACTION2); // use chip 
      } else if (Keyboard::Return == event.key.code) {
        events.push_back(PRESSED_ACTION3); // chip select
      } else if (Keyboard::P == event.key.code) {
        events.push_back(PRESSED_PAUSE);
      }
    }

    if (Event::KeyReleased == event.type) {
      if (Keyboard::Up == event.key.code) {
        events.push_back(RELEASED_UP);
      } else if (Keyboard::Left == event.key.code) {
        events.push_back(RELEASED_LEFT);
      } else if (Keyboard::Down == event.key.code) {
        events.push_back(RELEASED_DOWN);
      } else if (Keyboard::Right == event.key.code) {
        events.push_back(RELEASED_RIGHT);
      } else if (Keyboard::Space == event.key.code) {
        events.push_back(RELEASED_ACTION1);
      } else if (Keyboard::RControl == event.key.code) {
        events.push_back(RELEASED_ACTION2);
      } else if (Keyboard::Return == event.key.code) {
        events.push_back(RELEASED_ACTION3);
      } else if (Keyboard::P == event.key.code) {
        events.push_back(RELEASED_PAUSE);
      }
    }
  }
}

bool ControllableComponent::has(ControllableEvent _event) {
  return events.end() != find(events.begin(), events.end(), _event);
}

bool ControllableComponent::empty() {
  return events.empty();
}