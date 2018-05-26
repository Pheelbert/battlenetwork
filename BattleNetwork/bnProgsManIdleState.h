#pragma once
#include "bnAIState.h"
class ProgsMan;

class ProgsManIdleState : public AIState<ProgsMan>
{
private:
  float cooldown;

public:
  ProgsManIdleState();
  ~ProgsManIdleState();

  void OnEnter(ProgsMan& player);
  void OnUpdate(float _elapsed, ProgsMan& player);
  void OnLeave(ProgsMan& player);
};

