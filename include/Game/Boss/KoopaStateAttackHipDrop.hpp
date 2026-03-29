#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class Koopa;
class HitSensor;

class KoopaStateAttackHipDrop : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackHipDrop(Koopa*);

    virtual ~KoopaStateAttackHipDrop();
    virtual void init();
    virtual void appear();
    virtual void kill();

    void exeRun();
    void exeJumpStart();
    void exeJumpUp();
    void exeJumpWaitAir();
    void exeJumpAttackDown();
    void exeLand();
    void exeDamage();

    bool attackSensor(HitSensor*, HitSensor*);
    bool isDamage() const;
    bool isEnableGuard() const;

    /* 0x10 */ TVec3f mTargetPos;
    /* 0x1C */ s32 mAttackLimit;
    /* 0x20 */ s32 mAttackCount;
    /* 0x24 */ s32 mWaitStep;
    /* 0x28 */ s32 mMaxFallStep;
};
