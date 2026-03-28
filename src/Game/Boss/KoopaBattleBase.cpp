#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaStateDamageEscape.hpp"
#include "Game/Boss/KoopaStateGuard.hpp"

static MR::ActorMoveParam sFindParam = {0.0f, 1.0f, 0.98f, 3.0f};

KoopaBattleBase::KoopaBattleBase(const char* pName, Koopa* pKoopa)
    : ActorStateBase< Koopa >(pName, pKoopa), mDamageEscape(nullptr), mGuard(nullptr), mCurrentStep(-1) {
}

KoopaBattleBase::~KoopaBattleBase() {
}

void KoopaBattleBase::init() {
    mDamageEscape = new KoopaStateDamageEscape(mHost);
    mGuard = new KoopaStateGuard(mHost);
}

void KoopaBattleBase::kill() {
    mIsDead = true;
    MR::zeroVelocity(mHost);
    KoopaFunction::startRecoverKoopaArmor(mHost);
    KoopaFunction::startRecoverKoopaTailThorn(mHost);
    MR::overlayWithPreviousScreen(2);
}

void KoopaBattleBase::updateChasePlayer(const MR::ActorMoveParam& rMoveParam) {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Walk");
        MR::onBind(mHost);
        KoopaFunction::startFaceCtrl(mHost);
    }

    if (MR::isBindedWall(mHost)) {
        Koopa* koopa = mHost;
        HitSensor* pSensor = koopa->getSensor("Body");

        if (!MR::sendMsgEnemyAttackToBindedSensor(koopa, pSensor)) {
            MR::addVelocityJump(mHost, 15.0f);
        }
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->_8C, rMoveParam);

    Koopa* koopa = mHost;
    f32 velocityLength = MR::calcVelocityLength(koopa);
    MR::setBckRate(koopa, 0.18f * velocityLength);
}

bool KoopaBattleBase::updateWander(const MR::ActorMoveParam& rMoveParam) {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Walk");
        MR::onBind(mHost);
        KoopaFunction::endFaceCtrl(mHost, -1);
        mCurrentStep = MR::getRandom(60L, 120L);
    }

    if (MR::isBindedWall(mHost)) {
        Koopa* koopa = mHost;
        HitSensor* pSensor = koopa->getSensor("Body");

        if (!MR::sendMsgEnemyAttackToBindedSensor(koopa, pSensor)) {
            MR::addVelocityJump(mHost, 15.0f);
        }
    }

    TVec3f& koopaFront = KoopaFunction::getKoopaFront(mHost);
    TVec3f* koopaFrontPtr = KoopaFunction::getKoopaFrontPtr(mHost);

    MR::moveAndTurnToDirection(mHost, koopaFrontPtr, koopaFront, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);

    Koopa* koopa = mHost;
    f32 velocityLength = MR::calcVelocityLength(koopa);
    MR::setBckRate(koopa, 0.18f * velocityLength);

    return MR::isStep(this, mCurrentStep);
}

bool KoopaBattleBase::updateSearch() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Search");
        MR::zeroVelocity(mHost);
    }

    Koopa* koopa = mHost;
    MR::rotateVecDegree(KoopaFunction::getKoopaFrontPtr(koopa), koopa->mGravity, 2.4f);

    if (MR::isStep(this, 120)) {
        return true;
    }

    return false;
}

bool KoopaBattleBase::updateFind() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Find");
        MR::setVelocityJump(mHost, 10.0f);
    }

    Koopa* koopa = mHost;
    JGeometry::TVec3< f32 >* pFront = KoopaFunction::getKoopaFrontPtr(koopa);
    MR::moveAndTurnToPlayer(koopa, pFront, sFindParam);

    if (!MR::isFirstStep(this)) {
        if (MR::isBindedGround(mHost)) {
            MR::tryRumblePadAndCameraDistanceMiddle(mHost, 800.0f, 1200.0f, 2000.0f);
            return true;
        }
    }

    return false;
}

bool KoopaBattleBase::updateRecover(const Nerve* pNerve) {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(mHost);
        MR::startAction(mHost, "Recover");
        KoopaFunction::startRecoverKoopaArmor(mHost);
    }

    if (MR::isActionEnd(mHost)) {
        setNerve(pNerve);
        return true;
    }

    return false;
}

namespace MR {
    void moveAndTurnToPlayer(LiveActor* pActor, TVec3f* pVec, const MR::ActorMoveParam& rParam) {
        moveAndTurnToPlayer(pActor, pVec, rParam._0, rParam._4, rParam._8, rParam._C);
    }
};  // namespace MR
