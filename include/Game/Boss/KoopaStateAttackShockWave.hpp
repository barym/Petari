#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;
class HitSensor;

class KoopaStateAttackShockWave : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackShockWave(Koopa*);

    bool attackSensor(HitSensor*, HitSensor*);

    /* 0x10 */ char _10[0x14];
};
