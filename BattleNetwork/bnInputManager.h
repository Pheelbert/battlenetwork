#pragma once
#include <vector>
using std::vector;
#include "bnInputEvent.h"

class InputManager {
public:
  static InputManager& GetInstance();
  ~InputManager();
  void update();
  bool has(InputEvent _event);
  bool empty();

private:
  InputManager();
  vector<InputEvent> events;
};

#define INPUT InputManager::GetInstance()