#pragma once

#include "Game/Boss/KoopaBattleBase.hpp"
#include "revolution/types.h"

class Koopa;
class KoopaStateChaseRoll;
class KoopaStateAttackShockWave;

class KoopaBattleVs3Lv1 : public KoopaBattleBase {
public:
    KoopaBattleVs3Lv1(Koopa*);

    virtual ~KoopaBattleVs3Lv1();
    virtual void init();
    virtual void appear();

    void exeChaseRoll();
    void exeAttackFire();
    void exeAttackShockWave();
    void exeDamageReverse();
    void exeJumpAway();
    void exeGuard();
    void exeRecover();

    bool tryCalcAndSetBaseMtx();
    void attackSensor(HitSensor*, HitSensor*);
    bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    /* 0x1C */ KoopaStateChaseRoll* mStateChaseRoll;
    /* 0x20 */ KoopaStateAttackShockWave* mStateAttackShockWave;
};
