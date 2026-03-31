#include "Game/Boss/KoopaStateAttackSpin.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"

namespace NrvKoopaStateAttackSpin {
    NEW_NERVE(KoopaStateAttackSpinNrvWaitToStart, KoopaStateAttackSpin, WaitToStart);
    NEW_NERVE(KoopaStateAttackSpinNrvRun, KoopaStateAttackSpin, Run);
    NEW_NERVE(KoopaStateAttackSpinNrvStart, KoopaStateAttackSpin, Start);
    NEW_NERVE(KoopaStateAttackSpinNrvSpin, KoopaStateAttackSpin, Spin);
    NEW_NERVE(KoopaStateAttackSpinNrvEnd, KoopaStateAttackSpin, End);
    NEW_NERVE(KoopaStateAttackSpinNrvEven, KoopaStateAttackSpin, Even);
};  // namespace NrvKoopaStateAttackSpin

static MR::ActorMoveParam sRunParam = {3.0f, 1.0f, 0.9f, 3.0f};
static MR::ActorMoveParam sParamAttackSpin1st = {2.0f, 1.0f, 0.9f, 0.0f};
static MR::ActorMoveParam sParamAttackSpin2nd = {3.0f, 1.0f, 0.9f, 0.5f};
static MR::ActorMoveParam sParamAttackSpinBrake = {1.0f, 1.0f, 0.9f, 0.0f};
static MR::ActorMoveParam sEndParam = {0.0f, 1.0f, 0.90f, 0.0f};
static MR::ActorMoveParam sEvenParam = {0.0f, 1.0f, 0.95f, 0.5f};

KoopaStateAttackSpin::KoopaStateAttackSpin(Koopa* pKoopa)
    : ActorStateBase("State[スピン攻撃]", pKoopa), mSpinLimit(1), mSpinCount(0), mMoveParam(&sParamAttackSpin1st), mWaitStep(30) {
}

KoopaStateAttackSpin::~KoopaStateAttackSpin() {
}

void KoopaStateAttackSpin::init() {
    MR::addHitSensor(getHost(), "AttackSpin", ATYPE_KOOPA_ATTACK, 8, 250.0f, TVec3f(0.0f, 250.0f, 0.0f));
    initNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvStart::sInstance);
    kill();
}

void KoopaStateAttackSpin::appear() {
    mIsDead = false;
    MR::validateHitSensor(getHost(), "AttackSpin");
    mSpinCount = 0;

    if (KoopaFunction::isKoopaLv1(getHost())) {
        mSpinLimit = 1;
    } else if (KoopaFunction::isKoopaLv2(getHost())) {
        mSpinLimit = 2;
    } else {
        mSpinLimit = 3;
    }

    mMoveParam = &sParamAttackSpin1st;
    setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvWaitToStart::sInstance);
}

void KoopaStateAttackSpin::kill() {
    mIsDead = true;
    MR::invalidateHitSensor(getHost(), "AttackSpin");
}

bool KoopaStateAttackSpin::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver)) {
        return true;
    }

    if (isNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvSpin::sInstance)) {
        if (MR::isGreaterStep(this, 5)) {
            if (MR::isLessStep(this, 65)) {
                if (KoopaFunction::tryKoopaAttackPlayerMaximum(pSender, pReceiver)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool KoopaStateAttackSpin::tryDamage(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isMsgPlayerSpinAttack(msg) || !MR::isSensorPlayer(pSender)) {
        return false;
    }

    if (isNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvEven::sInstance)) {
        return false;
    }

    MR::sendMsgEnemyAttackFlipJump(pReceiver, pSender);
    MR::tryRumblePadStrong(this, 0);
    MR::startSound(getHost(), "SE_BM_KOOPA_SPIN_EVEN", -1, -1);
    MR::startSound(getHost(), "SE_BV_KOOPA_GUARD", -1, -1);
    MR::stopSceneForDefaultHit(15);

    mSpinCount++;
    if (mSpinLimit >= 2 && mSpinCount >= mSpinLimit) {
        kill();
        return true;
    }

    setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvEven::sInstance);
    return true;
}

void KoopaStateAttackSpin::exeWaitToStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Wait");
        MR::zeroVelocity(getHost());
    }

    if (MR::isStep(this, mWaitStep)) {
        setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvStart::sInstance);
    }
}

void KoopaStateAttackSpin::exeRun() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Run");
    }

    if (!KoopaFunction::moveAndTurnKoopaToPlayer(getHost(), sRunParam)) {
        if (MR::isBindedWall(getHost())) {
            MR::addVelocityJump(getHost(), 10.0f);
        }

        if (MR::isNearPlayer(getHost(), 1000.0f) || MR::isGreaterStep(this, 120)) {
            if (MR::calcGravitySpeed(getHost()) >= 0.0f) {
                setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvStart::sInstance);
            }
        }
    }
}

void KoopaStateAttackSpin::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "SpinStart");
        MR::zeroVelocity(getHost());
    }

    MR::turnDirectionToPlayerDegree(getHost(), &getHost()->mFront, 5.0f);

    if (MR::isActionEnd(getHost())) {
        setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvSpin::sInstance);
    }
}

void KoopaStateAttackSpin::exeSpin() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Spin");
        MR::zeroVelocity(getHost());
    }

    if (MR::isLessStep(this, 60)) {
        KoopaFunction::moveAndTurnKoopaToPlayer(getHost(), *mMoveParam);
    } else {
        KoopaFunction::moveAndTurnKoopaToPlayer(getHost(), sParamAttackSpinBrake);
    }

    if (MR::isBindedWall(getHost())) {
        MR::addVelocityJump(getHost(), 15.0f);
    }

    if (MR::isActionEnd(getHost())) {
        mSpinCount++;

        if (!MR::isPlayerDamaging() && mSpinCount < mSpinLimit) {
            mMoveParam = &sParamAttackSpin2nd;
            setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvStart::sInstance);
        } else {
            setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvEnd::sInstance);
        }
    }
}

void KoopaStateAttackSpin::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "SpinEnd");
    }

    KoopaFunction::moveAndTurnKoopaToPlayer(getHost(), sEndParam);

    if (MR::isActionEnd(getHost())) {
        MR::zeroVelocity(getHost());
        kill();
    }
}

void KoopaStateAttackSpin::exeEven() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "SpinEven");
        MR::calcVecFromPlayerH(&getHost()->mVelocity, getHost());
        MR::scaleVelocity(getHost(), 30.0f);
    }

    KoopaFunction::moveAndTurnKoopaToPlayer(getHost(), sEvenParam);

    if (MR::isActionEnd(getHost())) {
        if (!MR::isPlayerDamaging() && mSpinCount < mSpinLimit) {
            mMoveParam = &sParamAttackSpin2nd;
            setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvSpin::sInstance);
        } else {
            kill();
        }
    }
}
