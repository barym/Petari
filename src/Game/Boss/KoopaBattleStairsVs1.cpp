#include "Game/Boss/KoopaBattleStairsVs1.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/KoopaBattleMapStair.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include <revolution/types.h>

namespace NrvKoopaBattleStairsVs1 {
    NEW_NERVE(KoopaBattleStairsVs1NrvWaitDemo, KoopaBattleStairsVs1, WaitDemo);
    NEW_NERVE(KoopaBattleStairsVs1NrvDemo, KoopaBattleStairsVs1, Demo);
    NEW_NERVE(KoopaBattleStairsVs1NrvWait, KoopaBattleStairsVs1, Wait);
    NEW_NERVE(KoopaBattleStairsVs1NrvAttackFire, KoopaBattleStairsVs1, AttackFire);
    NEW_NERVE(KoopaBattleStairsVs1NrvJumpToNextPosStart, KoopaBattleStairsVs1, JumpToNextPosStart);
    NEW_NERVE(KoopaBattleStairsVs1NrvJumpToNextPosLoop, KoopaBattleStairsVs1, JumpToNextPosLoop);
    NEW_NERVE(KoopaBattleStairsVs1NrvJumpToNextPosEnd, KoopaBattleStairsVs1, JumpToNextPosEnd);
};  // namespace NrvKoopaBattleStairsVs1

static const char* sKoopaPosName0 = "階段の戦い０（クッパ）";
static const char* sKoopaPosName1 = "階段の戦い１（クッパ）";
static const char* sKoopaPosName2 = "階段の戦い２（クッパ）";
static const char* sKoopaPosNameEnd = "デモ中心";

KoopaBattleStairsVs1::KoopaBattleStairsVs1(Koopa* pKoopa)
    : KoopaBattleStairsBase(pKoopa), mDemoFarStair(nullptr), mDemoNearStair(nullptr), mFireStairs0(nullptr), mFireStairs1(nullptr),
      mTargetStair(nullptr), mHasStarted(false) {
    mFireStartPos0.set(0.0f);
    mFireStartPos1.set(0.0f);
    mFireStartPos2.set(0.0f);

    _4C = 0;
    _50 = 0;
    _54 = 0;
    _58 = 0;
    mPosProgress = 0;

    mJumpStartPos.set(0.0f);
    mJumpTargetPos.set(0.0f);
    mJumpFront.set(0.0f, 0.0f, 1.0f);

    initNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvWaitDemo::sInstance);

    calcFireStartPos(&mFireStartPos0, sKoopaPosName0);
    calcFireStartPos(&mFireStartPos1, sKoopaPosName1);
    calcFireStartPos(&mFireStartPos2, sKoopaPosName2);

    KoopaFunction::initKoopaAnimCamera(mKoopa, "DemoBattleStairsStart");
}

KoopaBattleStairsVs1::~KoopaBattleStairsVs1() {
}

void KoopaBattleStairsVs1::registerStair(KoopaBattleMapStair* pStair) {
    if (pStair->isTypeDemoFar()) {
        mDemoFarStair = pStair;
    }

    if (pStair->isTypeDemoNear()) {
        mDemoNearStair = pStair;
    }

    mStairGroup->registerActor((LiveActor*)pStair);

    TVec3f* fireStartPos = &mFireStartPos0;
    if (pStair->_94 == 1) {
        fireStartPos = &mFireStartPos1;

        if (pStair->isTypeNormal()) {
            _50++;
        }
    } else if (pStair->_94 == 2) {
        fireStartPos = &mFireStartPos2;

        if (pStair->isTypeNormal()) {
            _54++;
        }
    } else {
        if (pStair->isTypeNormal()) {
            _4C++;
        }
    }

    KoopaBattleStairsBase::calcFireAttackStep(pStair, 20.0f, 15, *fireStartPos);
}

void KoopaBattleStairsVs1::exeWaitDemo() {
    if (MR::tryStartDemo((LiveActor*)mKoopa, "階段の戦い開始デモ")) {
        KoopaFunction::startKoopaAnimCamera(mKoopa, "DemoBattleStairsStart", 60);
        KoopaFunction::endFaceCtrl(mKoopa, -1);
        MR::startAction((LiveActor*)mKoopa, "DemoBattleStairsStart");
        MR::stopStageBGM(60);
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvDemo::sInstance);
    }
}

void KoopaBattleStairsVs1::exeDemo() {
    if (MR::isStep(this, 264)) {
        Koopa* koopa;
        KoopaBattleMapStair* targetStair;
        targetStair = mDemoFarStair;
        koopa = mKoopa;

        TVec3f firePos(0.0f, 0.0f, 0.0f);
        MR::copyJointPos((LiveActor*)koopa, "Tongue2", &firePos);
        mFireStairs0 = KoopaFunction::emitFireStairsToTarget(koopa, targetStair, firePos, false);

        MR::startSound((LiveActor*)mKoopa, "SE_OJ_KOOPA_BULLET_SHOOT", -1, -1);
        MR::requestMovementOn(mFireStairs0);
        MR::requestMovementOn(mDemoFarStair);
    }

    if (MR::isStep(this, 292)) {
        Koopa* koopa;
        KoopaBattleMapStair* targetStair;
        targetStair = mDemoNearStair;
        koopa = mKoopa;

        TVec3f firePos(0.0f, 0.0f, 0.0f);
        MR::copyJointPos((LiveActor*)koopa, "Tongue2", &firePos);
        mFireStairs1 = KoopaFunction::emitFireStairsToTarget(koopa, targetStair, firePos, false);

        MR::startSound((LiveActor*)mKoopa, "SE_OJ_KOOPA_BULLET_SHOOT", -1, -1);
        MR::requestMovementOn(mFireStairs1);
        MR::requestMovementOn(mDemoNearStair);
    }

    if (MR::isBckStopped((LiveActor*)mKoopa)) {
        KoopaFunction::endKoopaAnimCamera(mKoopa, "DemoBattleStairsStart", 0);
        MR::endDemo((LiveActor*)mKoopa, "階段の戦い開始デモ");
        MR::onSwitchB((LiveActor*)KoopaFunction::getKoopaSwitchKeeper(mKoopa));
        KoopaFunction::startFaceCtrl(mKoopa);
        MR::startStageBGM("MBGM_GALAXY_15_HURRY", false);
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvWait::sInstance);
    }
}

void KoopaBattleStairsVs1::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction((LiveActor*)mKoopa, "Wait");
    }

    if (tryAttackRequest()) {
        MR::startAction((LiveActor*)mKoopa, "AttackFire");
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvAttackFire::sInstance);
    }
}

void KoopaBattleStairsVs1::exeAttackFire() {
    if (tryRequestedToMoveNextPos()) {
        mTargetStair = nullptr;
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvJumpToNextPosStart::sInstance);
        return;
    }

    KoopaBattleMapStair* targetStair;
    Koopa* koopa;

    if (MR::isStep(this, 15)) {
        targetStair = mTargetStair;
        koopa = mKoopa;

        TVec3f firePos(0.0f, 0.0f, 0.0f);
        MR::copyJointPos((LiveActor*)koopa, "Tongue2", &firePos);
        KoopaFunction::emitFireStairsToTarget(koopa, targetStair, firePos, false);
        mTargetStair = nullptr;
    }

    targetStair = mTargetStair;
    if (MR::isLessStep(this, 15)) {
        if (tryAttackRequest()) {
            koopa = mKoopa;

            TVec3f firePos(0.0f, 0.0f, 0.0f);
            MR::copyJointPos((LiveActor*)koopa, "Tongue2", &firePos);
            KoopaFunction::emitFireStairsToTarget(koopa, targetStair, firePos, false);
            setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvAttackFire::sInstance);
            return;
        }
    }

    if (MR::isGreaterEqualStep(this, 15)) {
        if (tryAttackRequest()) {
            setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvAttackFire::sInstance);
            return;
        }
    }

    if (MR::isGreaterStep(this, 33)) {
        if (mHasStarted || (mPosProgress == 0 && _58 >= _4C) || (mPosProgress == 1 && _58 >= _4C + _50) ||
            (mPosProgress == 2 && _58 >= _4C + _50 + _54)) {
            mTargetStair = nullptr;
            setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvJumpToNextPosStart::sInstance);
        } else {
            setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvWait::sInstance);
        }
    }
}

void KoopaBattleStairsVs1::exeJumpToNextPosStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mKoopa, "JumpStartFast");
        MR::startSound(mKoopa, "SE_BM_KOOPA_JUMP", -1, -1);
        mHasStarted = false;
    }

    if (MR::isBckStopped(mKoopa)) {
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvJumpToNextPosLoop::sInstance);
    }
}

// Vector math isn't correct
void KoopaBattleStairsVs1::exeJumpToNextPosLoop() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mKoopa, "JumpFast");
        mJumpStartPos.set(mKoopa->mPosition);

        mPosProgress++;
        TVec3f v(0.0f, 0.0f, 0.0f);

        if (mPosProgress == 1) {
            MR::findNamePos(sKoopaPosName1, &mJumpTargetPos, &v);
        } else if (mPosProgress == 2) {
            MR::findNamePos(sKoopaPosName2, &mJumpTargetPos, &v);
        } else {
            MR::findNamePos(sKoopaPosNameEnd, &mJumpTargetPos, &v);
        }

        MR::getRotatedAxisZ(&mJumpFront, v);
    }

    f32 t = (getNerveStep()) / 60.0f;
    f32 ease = MR::getEaseInOutValue(t, 0.0f, 1.0f, 1.0f);

    // Here's the troublemaker
    TVec3f jumpPos = (mJumpStartPos * (1.0f - ease)) + (mJumpTargetPos * ease);

    mKoopa->mPosition.set(jumpPos);
    mKoopa->mPosition.y += 1000.0f * JMath::sSinCosTable.sinLap(180.0f * t);
    MR::turnDirectionDegree(mKoopa, &mKoopa->mFront, mJumpFront, 2.0f);

    if (MR::isStep(this, 60)) {
        MR::tryRumblePadStrong(mKoopa, 0);
        MR::shakeCameraNormal();
        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvJumpToNextPosEnd::sInstance);
    }
}

void KoopaBattleStairsVs1::exeJumpToNextPosEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mKoopa, "LandFast");
        MR::startSound(mKoopa, "SE_BM_KOOPA_LAND", -1, -1);
    }

    if (tryAttackRequest()) {
        if (mPosProgress == 1) {
            KoopaFunction::setKoopaPos(mKoopa, sKoopaPosName1);
        } else {
            KoopaFunction::setKoopaPos(mKoopa, sKoopaPosName2);
        }

        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvAttackFire::sInstance);
        return;
    }

    if (MR::isBckStopped(mKoopa)) {
        if (mPosProgress == 1) {
            KoopaFunction::setKoopaPos(mKoopa, sKoopaPosName1);
        } else if (mPosProgress == 2) {
            KoopaFunction::setKoopaPos(mKoopa, sKoopaPosName2);
        } else {
            KoopaFunction::setKoopaPos(mKoopa, sKoopaPosNameEnd);
        }

        setNerve(&NrvKoopaBattleStairsVs1::KoopaBattleStairsVs1NrvWait::sInstance);
    }
}

bool KoopaBattleStairsVs1::tryAttackRequest() {
    KoopaBattleMapStair* stair = nullptr;
    bool requested = false;

    for (s32 i = 0; i < mStairGroup->mObjectCount; i++) {
        stair = (KoopaBattleMapStair*)(mStairGroup->getActor(i));

        if (stair->isRequestAttackVs1()) {
            mTargetStair = stair;
            _58++;
            requested = true;
        }
    }

    return requested;
}

bool KoopaBattleStairsVs1::tryRequestedToMoveNextPos() {
    KoopaBattleMapStair* stair = nullptr;

    for (s32 i = 0; i < mStairGroup->mObjectCount; i++) {
        stair = (KoopaBattleMapStair*)mStairGroup->getActor(i);

        if (stair->isRequestAttackVs1()) {
            if (stair->_94 > mPosProgress) {
                return true;
            }

            if (stair->_9C) {
                mHasStarted = true;
            }
        }
    }

    return false;
}
