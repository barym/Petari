#include "Game/Boss/KoopaStateAttackFireLong.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace NrvKoopaStateAttackFireLong {
    NEW_NERVE(KoopaStateAttackFireLongNrvStart, KoopaStateAttackFireLong, Start);
    NEW_NERVE(KoopaStateAttackFireLongNrvEmit, KoopaStateAttackFireLong, Emit);
};  // namespace NrvKoopaStateAttackFireLong

static MR::ActorMoveParam sFallParam = {0.0f, 1.0f, 1.0f, 0.0f};

KoopaStateAttackFireLong::KoopaStateAttackFireLong(Koopa* pKoopa) : ActorStateBase("State[炎連弾攻撃]", pKoopa), mMaxShots(16), mShotCount(-1) {
}

KoopaStateAttackFireLong::~KoopaStateAttackFireLong() {
}

void KoopaStateAttackFireLong::init() {
    initNerve(&NrvKoopaStateAttackFireLong::KoopaStateAttackFireLongNrvStart::sInstance);
}

void KoopaStateAttackFireLong::appear() {
    mIsDead = false;
    KoopaFunction::startFaceCtrl(getHost());
    mShotCount = 0;
    setNerve(&NrvKoopaStateAttackFireLong::KoopaStateAttackFireLongNrvStart::sInstance);
}

void KoopaStateAttackFireLong::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(getHost());
        MR::startAction(getHost(), "AttackFireStart");
    }

    MR::moveAndTurnToPlayer(getHost(), &getHost()->mFront, sFallParam);

    if (MR::isActionEnd(getHost())) {
        MR::startAction(getHost(), "AttackFire");
        setNerve(&NrvKoopaStateAttackFireLong::KoopaStateAttackFireLongNrvEmit::sInstance);
    }
}

void KoopaStateAttackFireLong::exeEmit() {
    if (MR::isFirstStep(this)) {
        mShotCount++;
    }

    MR::rotateVecDegree(KoopaFunction::getKoopaFrontPtr(getHost()), getHost()->mGravity, 3.0f);

    MR::moveAndTurnToPlayer(getHost(), &getHost()->mFront, sFallParam);

    if (MR::isStep(this, 0)) {
        KoopaFunction::emitKoopaFireLongTime(getHost());
    }

    if (MR::isStep(this, 15)) {
        if (mShotCount >= mMaxShots) {
            kill();
        } else {
            setNerve(&NrvKoopaStateAttackFireLong::KoopaStateAttackFireLongNrvEmit::sInstance);
        }
    }
}
