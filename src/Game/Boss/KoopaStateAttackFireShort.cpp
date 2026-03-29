#include "Game/Boss/KoopaStateAttackFireShort.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace NrvKoopaStateAttackFireShort {
    NEW_NERVE(KoopaStateAttackFireShortNrvStart, KoopaStateAttackFireShort, Start);
    NEW_NERVE(KoopaStateAttackFireShortNrvEmit, KoopaStateAttackFireShort, Emit);
};  // namespace NrvKoopaStateAttackFireShort

static MR::ActorMoveParam sFallParam = {0.0f, 1.0f, 1.0f, 0.0f};

KoopaStateAttackFireShort::KoopaStateAttackFireShort(Koopa* pKoopa)
    : ActorStateBase("State[ショート炎攻撃]", pKoopa), mShotCount(-1), mMaxShots(3), mShotInterval(0x2D) {
}

KoopaStateAttackFireShort::~KoopaStateAttackFireShort() {
}

void KoopaStateAttackFireShort::init() {
    initNerve(&NrvKoopaStateAttackFireShort::KoopaStateAttackFireShortNrvStart::sInstance);
}

void KoopaStateAttackFireShort::appear() {
    mIsDead = false;
    KoopaFunction::startFaceCtrl(getHost());
    mShotCount = 0;

    if (KoopaFunction::isKoopaVs1(getHost())) {
        if (KoopaFunction::isKoopaLv2(getHost())) {
            mMaxShots = 3;
        } else {
            mMaxShots = 5;
        }

        mShotInterval = 45;
    } else if (KoopaFunction::isKoopaVs2(getHost())) {
        mMaxShots = 5;
        mShotInterval = 30;
    } else if (KoopaFunction::isKoopaLv2(getHost())) {
        if (KoopaFunction::isKoopaAngry(getHost())) {
            mMaxShots = 5;
            mShotInterval = 15;
        } else {
            mMaxShots = 3;
            mShotInterval = 30;
        }
    } else if (KoopaFunction::isKoopaLv3(getHost())) {
        if (KoopaFunction::isKoopaAngry(getHost())) {
            mMaxShots = 10;
        } else {
            mMaxShots = 5;
        }

        mShotInterval = 15;
    } else {
        mMaxShots = 3;
        mShotInterval = 30;
    }

    setNerve(&NrvKoopaStateAttackFireShort::KoopaStateAttackFireShortNrvStart::sInstance);
}

void KoopaStateAttackFireShort::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(getHost());
        MR::startAction(getHost(), "AttackFireStart");
    }

    MR::moveAndTurnToPlayer(getHost(), &getHost()->mFront, sFallParam);

    if (MR::isActionEnd(getHost())) {
        MR::startAction(getHost(), "AttackFire");
        setNerve(&NrvKoopaStateAttackFireShort::KoopaStateAttackFireShortNrvEmit::sInstance);
    }
}

void KoopaStateAttackFireShort::exeEmit() {
    if (MR::isFirstStep(this)) {
        mShotCount++;
    }

    MR::turnDirectionToPlayerDegree(getHost(), KoopaFunction::getKoopaFrontPtr(getHost()), 2.0f);

    MR::moveAndTurnToPlayer(getHost(), &getHost()->mFront, sFallParam);

    if (MR::isStep(this, 1)) {
        if (KoopaFunction::isKoopaVs1(getHost()) && !KoopaFunction::isKoopaLv3(getHost())) {
            KoopaFunction::emitKoopaFireShortSlow(getHost());
        } else if (KoopaFunction::isKoopaVs2(getHost())) {
            KoopaFunction::emitKoopaFireShortCurve(getHost());
        } else {
            KoopaFunction::emitKoopaFireShortFast(getHost());
        }
    }

    if (mShotCount < mMaxShots) {
        if (MR::isStep(this, mShotInterval)) {
            setNerve(&NrvKoopaStateAttackFireShort::KoopaStateAttackFireShortNrvEmit::sInstance);
        }
    } else if (MR::isGreaterStep(this, 30)) {
        kill();
    }
}
