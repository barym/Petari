#pragma once

#include "Game/Boss/KoopaBattleStairsBase.hpp"
#include "Game/Boss/KoopaFireStairs.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class KoopaBattleMapStair;

class KoopaBattleStairsVs1 : public KoopaBattleStairsBase {
public:
    KoopaBattleStairsVs1(Koopa*);

    virtual ~KoopaBattleStairsVs1();
    virtual void registerStair(KoopaBattleMapStair*);

    void exeWaitDemo();
    void exeDemo();
    void exeWait();
    void exeAttackFire();
    void exeJumpToNextPosStart();
    void exeJumpToNextPosLoop();
    void exeJumpToNextPosEnd();
    bool tryAttackRequest();
    bool tryRequestedToMoveNextPos();

    /* 0x10 */ KoopaBattleMapStair* mDemoFarStair;
    /* 0x14 */ KoopaBattleMapStair* mDemoNearStair;
    /* 0x18 */ KoopaFireStairs* mFireStairs0;
    /* 0x1C */ KoopaFireStairs* mFireStairs1;
    /* 0x20 */ KoopaBattleMapStair* mTargetStair;
    /* 0x24 */ bool mHasStarted;
    /* 0x28 */ TVec3f mFireStartPos0;
    /* 0x34 */ TVec3f mFireStartPos1;
    /* 0x40 */ TVec3f mFireStartPos2;
    /* 0x4C */ s32 _4C;
    /* 0x50 */ s32 _50;
    /* 0x54 */ s32 _54;
    /* 0x58 */ s32 _58;
    /* 0x5C */ s32 mPosProgress;
    /* 0x60 */ TVec3f mJumpStartPos;
    /* 0x6C */ TVec3f mJumpTargetPos;
    /* 0x78 */ TVec3f mJumpFront;
};
