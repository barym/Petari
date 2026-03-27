#include "Game/Boss/KoopaBattleStairsBase.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Map/KoopaBattleMapStair.hpp"
#include "Game/Util/JointUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>

KoopaBattleStairsBase::KoopaBattleStairsBase(Koopa* pKoopa) : NerveExecutor("クッパ戦（階段の戦い）"), mKoopa(pKoopa), mStairGroup(nullptr) {
    mStairGroup = new LiveActorGroup("階段保持", 128);
    mStairGroup->initWithoutIter();
}

KoopaBattleStairsBase::~KoopaBattleStairsBase() {
}

void KoopaBattleStairsBase::calcFireStartPos(TVec3f* pOut, const char* pJointName) const {
    if (pJointName != nullptr) {
        KoopaFunction::setKoopaPos(this->mKoopa, pJointName);
    }

    MR::copyJointPos((LiveActor*)mKoopa, "Tongue2", pOut);
}

void KoopaBattleStairsBase::update() {
    updateNerve();
}

void KoopaBattleStairsBase::end() {
    for (s32 i = 0; i < this->mStairGroup->mObjectCount; i++) {
        this->mStairGroup->getActor(i)->makeActorDead();
    }
    KoopaFunction::killKoopaFireStairsAll(this->mKoopa);
}

void KoopaBattleStairsBase::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    KoopaFunction::tryKoopaReflectStarPiece(msg, pSender, pReceiver);
}

s32 KoopaBattleStairsBase::calcFireAttackStep(KoopaBattleMapStair* pStair, f32 step, s32 attackStep, const TVec3f& rTarget) const {
    TVec3f targetPos(0.0f, 0.0f, 0.0f);
    float dist = pStair->calcAndSetTargetPos(&targetPos, rTarget);
    pStair->_A8 = (s32)(dist / step) - (s32)(90.0f / step);
    return (pStair->mFireTimer - pStair->_A8) - attackStep;
}
