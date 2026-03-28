#pragma once

#include "Game/System/NerveExecutor.hpp"

class Koopa;
class JMapInfoIter;
class HitSensor;
class KoopaDemoPowerUp;
class KoopaDemoJumpToPlanet;

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

    /* 0x08 */ Koopa* mKoopa;
    /* 0x0C */ KoopaDemoPowerUp* mDemoPowerUp;
    /* 0x10 */ void* _10;
    /* 0x14 */ KoopaDemoJumpToPlanet* mDemoJumpToPlanet;
    /* 0x18 */ s32 mLevel;
    /* 0x1C */ s32 _1C;
    /* 0x20 */ bool _20;
};
