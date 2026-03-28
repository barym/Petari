#pragma once

#include "Game/System/NerveExecutor.hpp"

class Koopa;
class JMapInfoIter;
class HitSensor;

class KoopaSequencer : public NerveExecutor {
public:
    KoopaSequencer(const char*, s32);

    virtual ~KoopaSequencer();
    virtual void init(Koopa*, const JMapInfoIter&);
    virtual void update();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void startBattleLevel2();
    void startBattleLevel3();
};
