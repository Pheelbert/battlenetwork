#pragma once
#include <vector>
using std::vector;
#include "bnControllableEvent.h"

class ControllableComponent {
public:
  static ControllableComponent& GetInstance();
  ~ControllableComponent();
  void update();
  bool has(ControllableEvent _event);
  bool empty();

private:
  ControllableComponent();
  vector<ControllableEvent> events;
};

