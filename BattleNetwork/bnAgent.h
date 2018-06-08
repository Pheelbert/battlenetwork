class Entity;

class Agent {
private:
  Entity * target;
public:
  void SetTarget(Entity* _target) {
    target = _target;
  }

  void FreeTarget() {
    SetTarget(nullptr);
  }

  Entity* GetTarget() const { return target; }
};