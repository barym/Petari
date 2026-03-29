#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaStateDamageEscape.hpp"
#include "Game/Boss/KoopaStateGuard.hpp"

static MR::ActorMoveParam sFindParam = {0.0f, 1.0f, 0.98f, 3.0f};

KoopaBattleBase::KoopaBattleBase(const char* pName, Koopa* pKoopa)
    : ActorStateBase< Koopa >(pName, pKoopa), mStateDamageEscape(nullptr), mStateGuard(nullptr), mCurrentStep(-1) {
}

KoopaBattleBase::~KoopaBattleBase() {
}

void KoopaBattleBase::init() {
    mStateDamageEscape = new KoopaStateDamageEscape(getHost());
    mStateGuard = new KoopaStateGuard(getHost());
}

void KoopaBattleBase::kill() {
    mIsDead = true;
    MR::zeroVelocity(getHost());
    KoopaFunction::startRecoverKoopaArmor(getHost());
    KoopaFunction::startRecoverKoopaTailThorn(getHost());
    MR::overlayWithPreviousScreen(2);
}

void KoopaBattleBase::updateChasePlayer(const MR::ActorMoveParam& rMoveParam) {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Walk");
        MR::onBind(getHost());
        KoopaFunction::startFaceCtrl(getHost());
    }

    if (MR::isBindedWall(getHost())) {
        Koopa* koopa = getHost();
        HitSensor* pSensor = koopa->getSensor("Body");

        if (!MR::sendMsgEnemyAttackToBindedSensor(koopa, pSensor)) {
            MR::addVelocityJump(getHost(), 15.0f);
        }
    }

    MR::moveAndTurnToPlayer(getHost(), &getHost()->mFront, rMoveParam);

    Koopa* koopa = getHost();
    f32 velocityLength = MR::calcVelocityLength(koopa);
    MR::setBckRate(koopa, 0.18f * velocityLength);
}

bool KoopaBattleBase::updateWander(const MR::ActorMoveParam& rMoveParam) {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Walk");
        MR::onBind(getHost());
        KoopaFunction::endFaceCtrl(getHost(), -1);
        mCurrentStep = MR::getRandom(60L, 120L);
    }

    if (MR::isBindedWall(getHost())) {
        Koopa* koopa = getHost();
        HitSensor* pSensor = koopa->getSensor("Body");

        if (!MR::sendMsgEnemyAttackToBindedSensor(koopa, pSensor)) {
            MR::addVelocityJump(getHost(), 15.0f);
        }
    }

    TVec3f& koopaFront = KoopaFunction::getKoopaFront(getHost());
    TVec3f* koopaFrontPtr = KoopaFunction::getKoopaFrontPtr(getHost());

    MR::moveAndTurnToDirection(getHost(), koopaFrontPtr, koopaFront, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);

    Koopa* koopa = getHost();
    f32 velocityLength = MR::calcVelocityLength(koopa);
    MR::setBckRate(koopa, 0.18f * velocityLength);

    return MR::isStep(this, mCurrentStep);
}

bool KoopaBattleBase::updateSearch() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Search");
        MR::zeroVelocity(getHost());
    }

    Koopa* koopa = getHost();
    MR::rotateVecDegree(KoopaFunction::getKoopaFrontPtr(koopa), koopa->mGravity, 2.4f);

    if (MR::isStep(this, 120)) {
        return true;
    }

    return false;
}

bool KoopaBattleBase::updateFind() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Find");
        MR::setVelocityJump(getHost(), 10.0f);
    }

    Koopa* koopa = getHost();
    JGeometry::TVec3< f32 >* pFront = KoopaFunction::getKoopaFrontPtr(koopa);
    MR::moveAndTurnToPlayer(koopa, pFront, sFindParam);

    if (!MR::isFirstStep(this)) {
        if (MR::isBindedGround(getHost())) {
            MR::tryRumblePadAndCameraDistanceMiddle(getHost(), 800.0f, 1200.0f, 2000.0f);
            return true;
        }
    }

    return false;
}

bool KoopaBattleBase::updateRecover(const Nerve* pNerve) {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(getHost());
        MR::startAction(getHost(), "Recover");
        KoopaFunction::startRecoverKoopaArmor(getHost());
    }

    if (MR::isActionEnd(getHost())) {
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
