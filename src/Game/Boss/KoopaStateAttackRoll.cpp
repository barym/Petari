#include "Game/Boss/KoopaStateAttackRoll.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/KoopaFigureBall.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"

namespace NrvKoopaStateAttackRoll {
    NEW_NERVE(KoopaStateAttackRollNrvWaitToStart, KoopaStateAttackRoll, WaitToStart);
    NEW_NERVE(KoopaStateAttackRollNrvStart, KoopaStateAttackRoll, Start);
    NEW_NERVE(KoopaStateAttackRollNrvRollAir, KoopaStateAttackRoll, RollAir);
    NEW_NERVE(KoopaStateAttackRollNrvRollGround, KoopaStateAttackRoll, RollGround);
    NEW_NERVE(KoopaStateAttackRollNrvEndAir, KoopaStateAttackRoll, EndAir);
    NEW_NERVE(KoopaStateAttackRollNrvEndLand, KoopaStateAttackRoll, EndLand);
};  // namespace NrvKoopaStateAttackRoll

static MR::ActorMoveParam sStartParam = {0.0f, 0.5f, 1.0f, 3.0f};
static MR::ActorMoveParam sAttackRollParamLv2Slow = {3.0f, 2.0f, 0.9f, 1.0f};
static MR::ActorMoveParam sAttackRollParamLv2Fast = {4.5f, 2.0f, 0.9f, 1.0f};
static MR::ActorMoveParam sAttackRollParamLv3Slow = {8.0f, 2.0f, 0.8f, 0.5f};
static MR::ActorMoveParam sAttackRollParamLv3Fast = {10.0f, 2.0f, 0.8f, 1.0f};
static MR::ActorMoveParam sEndParam = {0.0f, 1.0f, 0.95f, 1.0f};

KoopaStateAttackRoll::KoopaStateAttackRoll(Koopa* pKoopa) : ActorStateBase("State[ローリング攻撃]", pKoopa) {
    mFigureBall = nullptr;
    mWaitStep = 90;
    mMaxRollStep = 600;
    mIsHitPlayer = false;
    mIsSkipWait = false;
}

KoopaStateAttackRoll::~KoopaStateAttackRoll() {
}

void KoopaStateAttackRoll::init() {
    initNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvStart::sInstance);

    KoopaFunction::initKoopaCamera(getHost(), "ローリング攻撃開始");
    KoopaFunction::initKoopaCamera(getHost(), "ローリング攻撃");

    KoopaFunction::createKoopaRollBall(getHost());

    mFigureBall = new KoopaFigureBall("追跡ボール", getHost(), 230.0f, &sAttackRollParamLv3Slow);
    mFigureBall->initWithoutIter();

    kill();
}

void KoopaStateAttackRoll::appear() {
    MR::zeroVelocity(getHost());

    mIsDead = false;
    MR::validateHitSensor(getHost(), "AttackRoll");
    MR::validateHitSensor(getHost(), "AttackRollReceiver");
    MR::validateHitSensor(getHost(), "AttackRollStarPiece");

    mIsHitPlayer = false;

    if (KoopaFunction::isKoopaVs3(getHost())) {
        if (KoopaFunction::isKoopaLv2(getHost())) {
            mWaitStep = 30;

            if (KoopaFunction::isKoopaAngry(getHost())) {
                mFigureBall->mMoveParam = &sAttackRollParamLv2Fast;
                mMaxRollStep = 300;
            } else {
                mFigureBall->mMoveParam = &sAttackRollParamLv2Slow;
                mMaxRollStep = 600;
            }
        } else {
            if (KoopaFunction::isKoopaAngry(getHost())) {
                mFigureBall->mMoveParam = &sAttackRollParamLv3Fast;
                mMaxRollStep = 320;
                mWaitStep = 75;
            } else {
                mFigureBall->mMoveParam = &sAttackRollParamLv3Slow;
                mMaxRollStep = 280;
                mWaitStep = 90;
            }

            if (mIsSkipWait) {
                mIsSkipWait = false;
                mWaitStep = 0;
            }
        }
    } else {
        mFigureBall->mMoveParam = &sAttackRollParamLv3Slow;
        mMaxRollStep = 280;
        mWaitStep = 90;
    }

    if (mWaitStep >= 0) {
        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvWaitToStart::sInstance);
    } else {
        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvStart::sInstance);
    }
}

void KoopaStateAttackRoll::kill() {
    mIsDead = true;

    KoopaFunction::getKoopaRollBall(getHost())->kill();

    MR::invalidateHitSensor(getHost(), "AttackRoll");
    MR::invalidateHitSensor(getHost(), "AttackRollReceiver");
    MR::invalidateHitSensor(getHost(), "AttackRollStarPiece");

    KoopaFunction::endKoopaCamera(getHost(), "ローリング攻撃開始", false, -1);
    KoopaFunction::endKoopaCamera(getHost(), "ローリング攻撃", false, -1);
}

bool KoopaStateAttackRoll::tryCalcAndSetBaseMtx() {
    if (isNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvRollAir::sInstance) ||
        isNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvRollGround::sInstance)) {
        MR::setBaseTRMtx(getHost(), mFigureBall->getBaseMtx());
        return true;
    }

    return false;
}

bool KoopaStateAttackRoll::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::isSensor(pSender, "AttackRoll")) {
            if (MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
                mIsHitPlayer = true;
                return true;
            }
        }
    }

    if (MR::isSensor(pSender, "AttackRoll")) {
        if (KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver)) {
            return true;
        }
    }

    return false;
}

bool KoopaStateAttackRoll::isDamage(u32 msg, HitSensor* pSender, HitSensor* pReceiver) const {
    if (!MR::isMsgToEnemyAttackBlow(msg) || !MR::isSensor(pReceiver, "AttackRollReceiver")) {
        return false;
    }
    return !mIsDead;
}

bool KoopaStateAttackRoll::isEnableGuard() const {
    if (isNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvWaitToStart::sInstance) ||
        isNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvEndAir::sInstance) ||
        isNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvEndLand::sInstance)) {
        return true;
    }

    return false;
}

void KoopaStateAttackRoll::exeWaitToStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Wait");
    }

    if (MR::isStep(this, mWaitStep)) {
        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvStart::sInstance);
    }
}

void KoopaStateAttackRoll::exeStart() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::getKoopaRollBall(getHost())->appear();
        MR::startAction(getHost(), "AttackRollStart");
        MR::startAction(KoopaFunction::getKoopaRollBall(getHost()), "AttackRollStart");
        KoopaFunction::startKoopaCamera(getHost(), "ローリング攻撃開始");
    }

    if (MR::isStep(this, 24)) {
        MR::setVelocityJump(getHost(), 20.0f);
    }

    if (MR::isGreaterStep(this, 24)) {
        MR::moveAndTurnToPlayer(getHost(), &getHost()->mFront, sStartParam);
    }

    if (MR::isActionEnd(getHost())) {
        mFigureBall->appear();
        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvRollAir::sInstance);
    }
}

void KoopaStateAttackRoll::exeRollAir() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "AttackRoll");
        MR::startAction(KoopaFunction::getKoopaRollBall(getHost()), "AttackRoll");
    }

    mFigureBall->_A0 += 5.0f;
    mFigureBall->movement();

    getHost()->mPosition.set(mFigureBall->mPosition);

    MR::startLevelSound(getHost(), "SE_BM_LV_KOOPA_ATTACK_ROLL", -1, -1, -1);

    if (MR::isBindedGround(mFigureBall)) {
        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvRollGround::sInstance);
    } else if (MR::isPlayerDamaging()) {
        setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvEndAir::sInstance);
    }
}

void KoopaStateAttackRoll::exeRollGround() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::startKoopaCamera(getHost(), "ローリング攻撃");
    }

    mFigureBall->movement();
    getHost()->mPosition.set(mFigureBall->mPosition);

    MR::sendMsgEnemyAttackToBindedSensor(mFigureBall, getHost()->getSensor("AttackRoll"));
    MR::startLevelSound(getHost(), "SE_BM_LV_KOOPA_ATTACK_ROLL", -1, -1, -1);
    MR::startLevelSound(getHost(), "SE_BM_LV_KOOPA_CHACE_ROLL", -1, -1, -1);

    if (MR::isGreaterStep(this, mMaxRollStep) || MR::isPlayerDamaging()) {
        mFigureBall->kill();

        if (!MR::isPlayerDamaging() && KoopaFunction::isKoopaVs3(getHost()) && KoopaFunction::isKoopaLv3(getHost())) {
            kill();
        } else {
            setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvEndAir::sInstance);
        }
    }
}

void KoopaStateAttackRoll::exeEndAir() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "AttackRollEnd");
        MR::startAction(KoopaFunction::getKoopaRollBall(getHost()), "AttackRollEnd");
        KoopaFunction::endKoopaCamera(getHost(), "ローリング攻撃", false, -1);
        MR::setVelocityJump(getHost(), 30.0f);
    }

    MR::moveAndTurnToPlayer(getHost(), &getHost()->mFront, sEndParam);

    if (!MR::isFirstStep(this)) {
        if (MR::isBindedGround(getHost())) {
            MR::zeroVelocity(getHost());
            setNerve(&NrvKoopaStateAttackRoll::KoopaStateAttackRollNrvEndLand::sInstance);
        }
    }
}

void KoopaStateAttackRoll::exeEndLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "AttackRollLand");
    }

    if (MR::isActionEnd(getHost())) {
        kill();
    }
}
