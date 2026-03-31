#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;
class HitSensor;

class KoopaStateAttackShockWave : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackShockWave(Koopa*);

    virtual ~KoopaStateAttackShockWave();
    virtual void init();
    virtual void appear();
    virtual void kill();

    bool attackSensor(HitSensor*, HitSensor*);

    void exeJumpStart();
    void exeJump();
    void exeJumpFast();
    void exeLand();

    /* 0x10 */ s32 mAttackLimit;
    /* 0x14 */ s32 mAttackCount;
    /* 0x18 */ f32 mJumpVelocity;
    /* 0x1C */ f32 mGravity;
    /* 0x20 */ s32 mWaitStep;
};
