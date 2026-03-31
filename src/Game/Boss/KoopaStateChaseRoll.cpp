#include "Game/Boss/KoopaStateChaseRoll.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/KoopaFigureBall.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaRockBreak.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"

namespace NrvKoopaStateChaseRoll {
    NEW_NERVE(KoopaStateChaseRollNrvWaitToStart, KoopaStateChaseRoll, WaitToStart);
    NEW_NERVE(KoopaStateChaseRollNrvStart, KoopaStateChaseRoll, Start);
    NEW_NERVE(KoopaStateChaseRollNrvRollAir, KoopaStateChaseRoll, RollAir);
    NEW_NERVE(KoopaStateChaseRollNrvRollGround, KoopaStateChaseRoll, RollGround);
    NEW_NERVE(KoopaStateChaseRollNrvEndAir, KoopaStateChaseRoll, EndAir);
    NEW_NERVE(KoopaStateChaseRollNrvEndLand, KoopaStateChaseRoll, EndLand);
};  // namespace NrvKoopaStateChaseRoll

static MR::ActorMoveParam sStartParam = {0.0f, 0.5f, 1.0f, 3.0f};
static MR::ActorMoveParam sChaseRollParam = {1.1f, 2.0f, 0.95f, 1.5f};
static MR::ActorMoveParam sChaseRollParamFast = {1.5f, 2.0f, 0.95f, 1.5f};
static MR::ActorMoveParam sEndParam = {0.0f, 1.0f, 0.98f, 1.0f};

KoopaStateChaseRoll::KoopaStateChaseRoll(Koopa* pKoopa) : ActorStateBase("State[転がり追跡攻撃]", pKoopa), mFigureBall(nullptr), mWaitStep(60) {
}

KoopaStateChaseRoll::~KoopaStateChaseRoll() {
}

void KoopaStateChaseRoll::init() {
    initNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvStart::sInstance);
    KoopaFunction::initKoopaCamera(getHost(), "ロール追跡開始");
    KoopaFunction::createKoopaRock(getHost());

    mFigureBall = new KoopaFigureBall("追跡ボール", getHost(), 260.0f, &sChaseRollParam);
    mFigureBall->initWithoutIter();
    kill();
}

void KoopaStateChaseRoll::appear() {
    MR::zeroVelocity(getHost());

    mIsDead = false;
    MR::validateHitSensor(getHost(), "ChaseRollFace");
    MR::validateHitSensor(getHost(), "ChaseRollAttack");
    MR::validateHitSensor(getHost(), "ChaseRollStarPiece");

    if (KoopaFunction::isKoopaAngry(getHost())) {
        mFigureBall->mMoveParam = &sChaseRollParamFast;
        mWaitStep = 60;
    } else {
        mFigureBall->mMoveParam = &sChaseRollParam;
        mWaitStep = 60;
    }

    if (mWaitStep >= 0) {
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvWaitToStart::sInstance);
    } else {
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvStart::sInstance);
    }
}

void KoopaStateChaseRoll::kill() {
    mIsDead = true;

    MR::invalidateHitSensor(getHost(), "ChaseRollFace");
    MR::invalidateHitSensor(getHost(), "ChaseRollAttack");
    MR::invalidateHitSensor(getHost(), "ChaseRollStarPiece");

    MR::tryDeleteEffect(KoopaFunction::getKoopaRock(getHost()), "RollingSmoke");
    KoopaFunction::getKoopaRock(getHost())->kill();

    KoopaFunction::endKoopaCamera(getHost(), "ロール追跡開始", false, -1);
    KoopaFunction::startFaceCtrl(getHost());
}

bool KoopaStateChaseRoll::tryCalcAndSetBaseMtx() {
    if (isNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvRollAir::sInstance) ||
        isNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvRollGround::sInstance)) {
        MR::setBaseTRMtx(getHost(), mFigureBall->getBaseMtx());
        return true;
    }

    return false;
}

bool KoopaStateChaseRoll::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::isSensor(pSender, "ChaseRollAttack")) {
            if (MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
                return true;
            }
        }
    }

    if (MR::isSensor(pSender, "ChaseRollAttack")) {
        if (KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver)) {
            return true;
        }
    }

    return false;
}

bool KoopaStateChaseRoll::tryDamage(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isMsgPlayerSpinAttack(msg) || !MR::isSensor(pReceiver, "ChaseRollFace")) {
        return false;
    }

    if (isNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvRollGround::sInstance)) {
        KoopaFunction::getKoopaRockBreak(getHost())->setCalcOwnMtx(true);
        KoopaFunction::getKoopaRockBreak(getHost())->appear();
        MR::calcAnimDirect(KoopaFunction::getKoopaRockBreak(getHost()));
        MR::startAction(KoopaFunction::getKoopaRockBreak(getHost()), "Break");
        MR::startSound(getHost(), "SE_BM_KOOPA_ROCK_BREAK", -1, -1);
        KoopaFunction::getKoopaRockBreak(getHost())->setCalcOwnMtx(false);
        return true;
    }

    return false;
}

bool KoopaStateChaseRoll::isEnableGuard() const {
    if (isNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvWaitToStart::sInstance) ||
        isNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvEndAir::sInstance) ||
        isNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvEndLand::sInstance)) {
        return true;
    }

    return false;
}

void KoopaStateChaseRoll::exeWaitToStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Wait");
    }

    if (MR::isStep(this, mWaitStep)) {
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvStart::sInstance);
    }
}

void KoopaStateChaseRoll::exeStart() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::getKoopaRock(getHost())->appear();
        KoopaFunction::getKoopaRockBreak(getHost())->appear();
        KoopaFunction::getKoopaRockBreak(getHost())->setCalcOwnMtx(true);

        MR::startAction(getHost(), "ChaseRollStart");
        MR::startAction(KoopaFunction::getKoopaRock(getHost()), "AttackRollStart");
        MR::startAction(KoopaFunction::getKoopaRockBreak(getHost()), "AttackRollStart");
        KoopaFunction::startKoopaCamera(getHost(), "ロール追跡開始");
        KoopaFunction::endFaceCtrl(getHost(), -1);
    }

    MR::startLevelSound(getHost(), "SE_BM_LV_KOOPA_ROCK_GATHER", -1, -1, -1);

    if (MR::isStep(this, 24)) {
        MR::setVelocityJump(getHost(), 30.0f);
    }

    if (MR::isGreaterStep(this, 24)) {
        MR::moveAndTurnToPlayer(getHost(), &getHost()->mFront, sStartParam);
    }

    if (MR::isActionEnd(getHost())) {
        mFigureBall->appear();
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvRollAir::sInstance);
    } else if (MR::isPlayerDamaging()) {
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvEndAir::sInstance);
    }
}

void KoopaStateChaseRoll::exeRollAir() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "ChaseRoll");
        MR::startAction(KoopaFunction::getKoopaRock(getHost()), "AttackRoll");
    }

    mFigureBall->_A0 += 5.0f;
    mFigureBall->movement();

    getHost()->mPosition.set(mFigureBall->mPosition);

    if (MR::isBindedGround(mFigureBall)) {
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvRollGround::sInstance);
    } else if (MR::isPlayerDamaging()) {
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvEndAir::sInstance);
    }
}

void KoopaStateChaseRoll::exeRollGround() {
    if (MR::isFirstStep(this)) {
        KoopaFunction::endKoopaCamera(getHost(), "ロール追跡開始", false, -1);
        MR::emitEffect(KoopaFunction::getKoopaRock(getHost()), "RollingSmoke");
    }

    mFigureBall->movement();
    getHost()->mPosition.set(mFigureBall->mPosition);

    MR::sendMsgEnemyAttackToBindedSensor(mFigureBall, getHost()->getSensor("ChaseRollFace"));
    MR::startLevelSound(getHost(), "SE_BM_LV_KOOPA_CHACE_ROLL", -1, -1, -1);

    if (MR::isPlayerDamaging() || MR::isGreaterStep(this, 900)) {
        MR::deleteEffect(KoopaFunction::getKoopaRock(getHost()), "RollingSmoke");
        mFigureBall->kill();
        setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvEndAir::sInstance);
    }
}

void KoopaStateChaseRoll::exeEndAir() {
    if (MR::isFirstStep(this)) {
        MR::setVelocityJump(getHost(), 30.0f);
        MR::startAction(getHost(), "ChaseRollEnd");
        MR::startAction(KoopaFunction::getKoopaRock(getHost()), "AttackRollEnd");

        KoopaFunction::getKoopaRockBreak(getHost())->setCalcOwnMtx(true);
        KoopaFunction::getKoopaRockBreak(getHost())->appear();
        MR::calcAnimDirect(KoopaFunction::getKoopaRockBreak(getHost()));
        MR::startAction(KoopaFunction::getKoopaRockBreak(getHost()), "AttackRollEnd");
        KoopaFunction::getKoopaRockBreak(getHost())->setCalcOwnMtx(false);

        MR::startSound(getHost(), "SE_BM_KOOPA_ROCK_BREAK", -1, -1);
    }

    MR::moveAndTurnToPlayer(getHost(), &getHost()->mFront, sEndParam);

    if (!MR::isFirstStep(this)) {
        if (MR::isBindedGround(getHost())) {
            MR::zeroVelocity(getHost());
            setNerve(&NrvKoopaStateChaseRoll::KoopaStateChaseRollNrvEndLand::sInstance);
        }
    }
}

void KoopaStateChaseRoll::exeEndLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "ChaseRollLand");
    }

    if (MR::isBckStopped(getHost())) {
        kill();
    }
}
