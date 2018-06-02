#include <SFML\Window.hpp>
using sf::Event;
using sf::Keyboard;
#include "bnEngine.h"
#include "bnInputManager.h"
#include "bnDirection.h"

InputManager& InputManager::GetInstance() {
  static InputManager instance;
  return instance;
}

InputManager::InputManager()
  : events(vector<InputEvent>()),
    config("options.ini") {
}


InputManager::~InputManager() {
}

void InputManager::update() {
  this->events.clear();
  Event event;
  while (ENGINE.GetWindow()->pollEvent(event)) {
    if (event.type == Event::Closed) {
      ENGINE.GetWindow()->close();
    }

    if (Event::KeyPressed == event.type) {
      std::string action = "";
      if (config.IsOK()) {
        action = config.GetPairedAction(event.key.code);

        if (action == "Select") {
          events.push_back(PRESSED_PAUSE);
        } else if (action == "Start") {
          events.push_back(PRESSED_ACTION3);
        } else if (action == "Left") {
          events.push_back(PRESSED_LEFT);
        }else if (action == "Right") {
          events.push_back(PRESSED_RIGHT);
        } else if (action == "Up") {
          events.push_back(PRESSED_UP);
        } else if (action == "Down") {
          events.push_back(PRESSED_DOWN);
        } else if (action == "A") {
          events.push_back(PRESSED_ACTION1);
        } else if (action == "B") {
          events.push_back(PRESSED_ACTION2);
        } 
      }
      else {
        if (Keyboard::Up == event.key.code) {
          events.push_back(PRESSED_UP);
        }
        else if (Keyboard::Left == event.key.code) {
          events.push_back(PRESSED_LEFT);
        }
        else if (Keyboard::Down == event.key.code) {
          events.push_back(PRESSED_DOWN);
        }
        else if (Keyboard::Right == event.key.code) {
          events.push_back(PRESSED_RIGHT);
        }
        else if (Keyboard::Space == event.key.code) {
          events.push_back(PRESSED_ACTION1); // shoot 
        }
        else if (Keyboard::RControl == event.key.code) {
          events.push_back(PRESSED_ACTION2); // use chip 
        }
        else if (Keyboard::Return == event.key.code) {
          events.push_back(PRESSED_ACTION3); // chip select
        }
        else if (Keyboard::P == event.key.code) {
          events.push_back(PRESSED_PAUSE);
        }
      }
    }

    if (Event::KeyReleased == event.type) {
      std::string action = "";
      if (config.IsOK()) {
        action = config.GetPairedAction(event.key.code);

        if (action == "Select") {
          events.push_back(RELEASED_PAUSE);
        }
        else if (action == "Start") {
          events.push_back(RELEASED_ACTION3);
        }
        else if (action == "Left") {
          events.push_back(RELEASED_LEFT);
        }
        else if (action == "Right") {
          events.push_back(RELEASED_RIGHT);
        }
        else if (action == "Up") {
          events.push_back(RELEASED_UP);
        }
        else if (action == "Down") {
          events.push_back(RELEASED_DOWN);
        }
        else if (action == "A") {
          events.push_back(RELEASED_ACTION1);
        }
        else if (action == "B") {
          events.push_back(RELEASED_ACTION2);
        }
      }
      else {
        if (Keyboard::Up == event.key.code) {
          events.push_back(RELEASED_UP);
        }
        else if (Keyboard::Left == event.key.code) {
          events.push_back(RELEASED_LEFT);
        }
        else if (Keyboard::Down == event.key.code) {
          events.push_back(RELEASED_DOWN);
        }
        else if (Keyboard::Right == event.key.code) {
          events.push_back(RELEASED_RIGHT);
        }
        else if (Keyboard::Space == event.key.code) {
          events.push_back(RELEASED_ACTION1);
        }
        else if (Keyboard::RControl == event.key.code) {
          events.push_back(RELEASED_ACTION2);
        }
        else if (Keyboard::Return == event.key.code) {
          events.push_back(RELEASED_ACTION3);
        }
        else if (Keyboard::P == event.key.code) {
          events.push_back(RELEASED_PAUSE);
        }
      }
    }
  }
}

bool InputManager::has(InputEvent _event) {
  return events.end() != find(events.begin(), events.end(), _event);
}

bool InputManager::empty() {
  return events.empty();
}