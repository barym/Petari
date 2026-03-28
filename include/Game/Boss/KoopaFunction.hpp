#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class LiveActor;
class Koopa;
class KoopaBattleMapStair;
class HitSensor;
class KoopaFireStairs;
class KoopaSwitchKeeper;

namespace KoopaFunction {
    s32 registerBattleMapStair(KoopaBattleMapStair*);

    void initKoopaCamera(Koopa*, const char*);
    void startKoopaTargetCamera(Koopa*, const char*);
    void endKoopaCamera(Koopa*, const char*, bool, s32);
    LiveActor* getKoopaPowerUpSwitch(Koopa*);

    void setKoopaPos(Koopa*, const char*);

    void startRecoverKoopaArmor(Koopa*);
    void startRecoverKoopaTailThorn(Koopa*);
    void endFaceCtrl(Koopa*, s32);

    bool tryRestartKoopa();

    bool isKoopaVs1(const Koopa*);
    bool isKoopaVs2(const Koopa*);
    bool isKoopaLv3(const Koopa*);

    void registerKoopaSwitchKeeper(LiveActor*);
    void registerKoopaViewSwitchKeeper(LiveActor*);

    void killKoopaFireStairsAll(Koopa*);
    void tryKoopaReflectStarPiece(u32, HitSensor*, HitSensor*);

    void initKoopaAnimCamera(Koopa*, const char*);
    void startKoopaAnimCamera(Koopa*, const char*, s32);
    KoopaFireStairs* emitFireStairsToTarget(Koopa*, const KoopaBattleMapStair*, const TVec3f&, bool);
    void endKoopaAnimCamera(Koopa*, const char*, s32);
    KoopaSwitchKeeper* getKoopaSwitchKeeper(Koopa*);
    void startFaceCtrl(Koopa*);
};  // namespace KoopaFunction
