#pragma once
enum MobState
{
    MOB_MOVING,
    MOB_IDLE,
    MOB_HIT,
    MOB_ATTACKING,
	MOB_THROW // TODO: navis should have their own state enum since they are unique...
};