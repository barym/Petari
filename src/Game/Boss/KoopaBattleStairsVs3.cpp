#include "Game/Boss/KoopaBattleStairsVs3.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/KoopaBattleMapStair.hpp"

namespace NrvKoopaBattleStairsVs3 {
    NEW_NERVE(KoopaBattleStairsVs3NrvWaitDemo, KoopaBattleStairsVs3, WaitDemo);
    NEW_NERVE(KoopaBattleStairsVs3NrvDemo, KoopaBattleStairsVs3, Demo);
    NEW_NERVE(KoopaBattleStairsVs3NrvWait, KoopaBattleStairsVs3, Wait);
};  // namespace NrvKoopaBattleStairsVs3

KoopaBattleStairsVs3::KoopaBattleStairsVs3(Koopa* pKoopa) : KoopaBattleStairsBase(pKoopa) {
    mTargetPos.set(0.0f);
    initNerve(&NrvKoopaBattleStairsVs3::KoopaBattleStairsVs3NrvWaitDemo::sInstance);
    MR::findNamePos("クッパ階段戦の砲弾出現", &mTargetPos, 0);
    KoopaFunction::initKoopaAnimCamera(mKoopa, "DemoKoopaBattleStairsVs3Start");
}

KoopaBattleStairsVs3::~KoopaBattleStairsVs3() {
}

void KoopaBattleStairsVs3::registerStair(KoopaBattleMapStair* pStair) {
    mStairGroup->registerActor((LiveActor*)pStair);
    calcFireAttackStep(pStair, 30.0f, 0, mTargetPos);
}

void KoopaBattleStairsVs3::exeWaitDemo() {
    if (KoopaFunction::tryStartKoopaCameraDemo(mKoopa, "階段の戦い開始デモ", "DemoKoopaBattleStairsVs3Start", "デモ中心")) {
        KoopaFunction::getKoopaDemoMeteor1(mKoopa)->appear();
        KoopaFunction::getKoopaDemoMeteor2(mKoopa)->appear();
        KoopaFunction::getKoopaDemoMeteor3(mKoopa)->appear();

        MR::requestMovementOn(KoopaFunction::getKoopaDemoPeach(mKoopa));
        MR::requestMovementOn(KoopaFunction::getKoopaDemoKoopaJr(mKoopa));
        MR::requestMovementOn(KoopaFunction::getKoopaDemoKoopaJrShip(mKoopa));
        MR::requestMovementOn(KoopaFunction::getKoopaDemoMeteor1(mKoopa));
        MR::requestMovementOn(KoopaFunction::getKoopaDemoMeteor2(mKoopa));
        MR::requestMovementOn(KoopaFunction::getKoopaDemoMeteor3(mKoopa));

        MR::startAction(KoopaFunction::getKoopaDemoPeach(mKoopa), "DemoKoopaBattleStairsVs3Start");
        MR::startAction(KoopaFunction::getKoopaDemoKoopaJr(mKoopa), "DemoKoopaBattleStairsVs3Start");
        MR::startAction(KoopaFunction::getKoopaDemoKoopaJrShip(mKoopa), "DemoKoopaBattleStairsVs3Start");
        MR::startAction(KoopaFunction::getKoopaDemoMeteor1(mKoopa), "DemoKoopaBattleStairsVs3Start01");
        MR::startAction(KoopaFunction::getKoopaDemoMeteor2(mKoopa), "DemoKoopaBattleStairsVs3Start02");
        MR::startAction(KoopaFunction::getKoopaDemoMeteor3(mKoopa), "DemoKoopaBattleStairsVs3Start03");

        KoopaFunction::endFaceCtrl(mKoopa, -1);
        setNerve(&NrvKoopaBattleStairsVs3::KoopaBattleStairsVs3NrvDemo::sInstance);
    }
}

void KoopaBattleStairsVs3::exeDemo() {
    if (KoopaFunction::tryEndKoopaCameraDemo(mKoopa, "階段の戦い開始デモ", "DemoKoopaBattleStairsVs3Start")) {
        KoopaFunction::startFaceCtrl(mKoopa);

        MR::startAction(KoopaFunction::getKoopaDemoPeach(mKoopa), "DemoKoopaVs3Wait");
        MR::startAction(KoopaFunction::getKoopaDemoKoopaJr(mKoopa), "DemoKoopaVs3Wait");
        MR::startAction(KoopaFunction::getKoopaDemoKoopaJrShip(mKoopa), "DemoKoopaVs3Wait");

        KoopaFunction::getKoopaDemoMeteor1(mKoopa)->kill();
        KoopaFunction::getKoopaDemoMeteor2(mKoopa)->kill();
        KoopaFunction::getKoopaDemoMeteor3(mKoopa)->kill();

        setNerve(&NrvKoopaBattleStairsVs3::KoopaBattleStairsVs3NrvWait::sInstance);
    }
}

void KoopaBattleStairsVs3::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction((LiveActor*)mKoopa, "Wait");
    }

    tryAttack();
}

void KoopaBattleStairsVs3::tryAttack() {
    KoopaBattleMapStair* stair;
    for (s32 i = 0; i < mStairGroup->mObjectCount; i++) {
        stair = (KoopaBattleMapStair*)mStairGroup->getActor(i);

        if (stair->isRequestAttackVs3()) {
            stair->_A6 = true;
            KoopaFunction::emitFireStairsToTarget(mKoopa, stair, mTargetPos, true);
        }
    }
}
