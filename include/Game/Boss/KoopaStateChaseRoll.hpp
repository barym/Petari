#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;
class HitSensor;

class KoopaStateChaseRoll : public ActorStateBase< Koopa > {
public:
    KoopaStateChaseRoll(Koopa*);

    bool tryCalcAndSetBaseMtx();
    bool attackSensor(HitSensor*, HitSensor*);
    bool tryDamage(u32, HitSensor*, HitSensor*);
    bool isEnableGuard() const;

    /* 0x10 */ char _10[0x8];
};
