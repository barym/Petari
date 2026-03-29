#pragma once

#include "Game/Boss/KoopaBattleBase.hpp"

class Koopa;
class KoopaStateAttackRoll;

class KoopaBattleVs3Lv2 : public KoopaBattleBase {
public:
    KoopaBattleVs3Lv2(Koopa*);

    virtual ~KoopaBattleVs3Lv2();
    virtual void init();
    virtual void appear();

    void exeAttackRoll();
    void exeAttackFire();
    void exeAttackShockWave();
    void exeDamageReverse();
    void exeJumpAway();
    void exeGuard();
    void exeRecover();

    bool tryCalcAndSetBaseMtx();
    void attackSensor(HitSensor*, HitSensor*);
    bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    /* 0x1C */ KoopaStateAttackRoll* mStateAttackRoll;
};
