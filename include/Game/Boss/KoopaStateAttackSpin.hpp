#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/Util/ActorMovementUtil.hpp"

class Koopa;
class HitSensor;

class KoopaStateAttackSpin : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackSpin(Koopa*);

    virtual ~KoopaStateAttackSpin();
    virtual void init();
    virtual void appear();
    virtual void kill();

    bool attackSensor(HitSensor*, HitSensor*);
    bool tryDamage(u32, HitSensor*, HitSensor*);

    void exeWaitToStart();
    void exeRun();
    void exeStart();
    void exeSpin();
    void exeEnd();
    void exeEven();

    /* 0x10 */ s32 mSpinLimit;
    /* 0x14 */ s32 mSpinCount;
    /* 0x18 */ const MR::ActorMoveParam* mMoveParam;
    /* 0x1C */ s32 mWaitStep;
};
