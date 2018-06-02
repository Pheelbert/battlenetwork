#pragma once
#include <vector>
using std::vector;
#include "bnInputEvent.h"
#include "bnChronoXConfigReader.h"

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

  // Support for ChronoX config.ini files
  ChronoXConfigReader config;
};

#define INPUT InputManager::GetInstance()