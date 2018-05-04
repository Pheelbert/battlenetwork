#pragma once
#include "bnTile.h"
#include "bnField.h"
class ProgsMan;

class ProgsManPunchState : public AIState<ProgsMan>
{
public:
  ProgsManPunchState();
  ~ProgsManPunchState();

  void OnEnter(ProgsMan& progs);
  void OnUpdate(float _elapsed, ProgsMan& progs);
  void OnLeave(ProgsMan& progs);
  void Attack(ProgsMan& progs);
};

