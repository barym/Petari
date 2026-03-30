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
    bool isKoopaVs3(const Koopa*);
    bool isKoopaLv1(const Koopa*);
    bool isKoopaLv2(const Koopa*);
    bool isKoopaLv3(const Koopa*);

    void registerKoopaSwitchKeeper(LiveActor*);
    void registerKoopaViewSwitchKeeper(LiveActor*);

    void killKoopaFireStairsAll(Koopa*);
    bool tryKoopaReflectStarPiece(u32, HitSensor*, HitSensor*);

    void initKoopaAnimCamera(Koopa*, const char*);
    void startKoopaAnimCamera(Koopa*, const char*, s32);
    KoopaFireStairs* emitFireStairsToTarget(Koopa*, const KoopaBattleMapStair*, const TVec3f&, bool);
    void endKoopaAnimCamera(Koopa*, const char*, s32);
    KoopaSwitchKeeper* getKoopaSwitchKeeper(Koopa*);
    void startFaceCtrl(Koopa*);

    bool tryStartKoopaCameraDemo(Koopa*, const char*, const char*, const char*);
    LiveActor* getKoopaDemoMeteor1(Koopa*);
    LiveActor* getKoopaDemoMeteor2(Koopa*);
    LiveActor* getKoopaDemoMeteor3(Koopa*);
    LiveActor* getKoopaDemoPeach(Koopa*);
    LiveActor* getKoopaDemoKoopaJr(Koopa*);
    LiveActor* getKoopaDemoKoopaJrShip(Koopa*);
    bool tryEndKoopaCameraDemo(Koopa*, const char*, const char*);

    TVec3f& getKoopaFront(const Koopa*);
    TVec3f* getKoopaFrontPtr(Koopa*);

    void endFaceCtrlDirect(Koopa*);

    void registerKoopaPowerUpSwitch(LiveActor*);

    TVec3f* getPlanetCenterPos(const Koopa*);

    bool isKoopaAngry(const Koopa*);
    void changeBgmStateNormal(u32);
    void startKoopaAngry(Koopa*);
    bool tryKoopaPushPlayer(HitSensor*, HitSensor*);
    bool tryKoopaAttackMapObj(HitSensor*, HitSensor*);
    bool tryKoopaBodyAttackPlayer(HitSensor*, HitSensor*);

    void emitKoopaFireShortSlow(Koopa*);
    void emitKoopaFireShortFast(Koopa*);
    void emitKoopaFireShortCurve(Koopa*);
    void emitKoopaFireLongTime(Koopa*);

    bool tryKoopaBodyAttackPlayerMaximum(HitSensor*, HitSensor*);
    void startKoopaCamera(Koopa*, const char*);
    HitSensor* getKoopaMessageSensor(Koopa*);
    void emitKoopaShockWave(Koopa*);
};  // namespace KoopaFunction
