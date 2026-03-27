#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class Koopa;
class HitSensor;
class KoopaBattleMapStair;

class KoopaBattleStairsBase : public NerveExecutor {
public:
    KoopaBattleStairsBase(Koopa*);

    virtual ~KoopaBattleStairsBase();

    void calcFireStartPos(TVec3f* pOut, const char* pJointName) const;
    void update();
    void end();
    void receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    s32 calcFireAttackStep(KoopaBattleMapStair*, f32, s32, const TVec3f&) const;

    /* 0x08 */ Koopa* mKoopa;
    /* 0x0C */ LiveActorGroup* mStairGroup;
};
