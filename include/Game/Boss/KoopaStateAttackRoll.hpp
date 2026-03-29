#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;
class HitSensor;

class KoopaStateAttackRoll : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackRoll(Koopa*);

    bool tryCalcAndSetBaseMtx();
    bool attackSensor(HitSensor*, HitSensor*);
    bool isEnableGuard() const;
    bool isDamage(u32, HitSensor*, HitSensor*) const;

    /* 0x10 */ char _10[0x10];
};
