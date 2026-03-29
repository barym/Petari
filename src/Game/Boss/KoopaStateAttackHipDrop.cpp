#include "Game/Boss/KoopaStateAttackHipDrop.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace NrvKoopaStateAttackHipDrop {
    NEW_NERVE(KoopaStateAttackHipDropNrvRun, KoopaStateAttackHipDrop, Run);
    NEW_NERVE(KoopaStateAttackHipDropNrvJumpStart, KoopaStateAttackHipDrop, JumpStart);
    NEW_NERVE(KoopaStateAttackHipDropNrvJumpUp, KoopaStateAttackHipDrop, JumpUp);
    NEW_NERVE(KoopaStateAttackHipDropNrvJumpWaitAir, KoopaStateAttackHipDrop, JumpWaitAir);
    NEW_NERVE(KoopaStateAttackHipDropNrvJumpAttackDown, KoopaStateAttackHipDrop, JumpAttackDown);
    NEW_NERVE(KoopaStateAttackHipDropNrvLand, KoopaStateAttackHipDrop, Land);
    NEW_NERVE(KoopaStateAttackHipDropNrvDamage, KoopaStateAttackHipDrop, Damage)
};  // namespace NrvKoopaStateAttackHipDrop

static MR::ActorMoveParam sParamRunStart = {3.0f, 1.0f, 0.9f, 3.0f};
static MR::ActorMoveParam sParamJumpStart = {0.0f, 1.0f, 0.95f, 0.0f};
static MR::ActorMoveParam sFallParam = {0.0f, 1.0f, 1.0f, 0.0f};

KoopaStateAttackHipDrop::KoopaStateAttackHipDrop(Koopa* pKoopa)
    : ActorStateBase("State[ヒップドロップ攻撃]", pKoopa), mTargetPos(0.0f, 0.0f, 0.0f), mAttackLimit(1), mAttackCount(0), mWaitStep(30),
      mMaxFallStep(0) {
}

KoopaStateAttackHipDrop::~KoopaStateAttackHipDrop() {
}

void KoopaStateAttackHipDrop::init() {
    KoopaFunction::initKoopaCamera(getHost(), "ヒップドロップ攻撃");
    KoopaFunction::initKoopaCamera(getHost(), "ヒップドロップ着地");
    KoopaFunction::initKoopaCamera(getHost(), "ヒップドロップ着地（マリオ痺れ）");
    initNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvRun::sInstance);
    kill();
}

void KoopaStateAttackHipDrop::appear() {
    MR::onCalcGravity(getHost());
    MR::onBind(getHost());

    mIsDead = false;
    MR::validateHitSensor(getHost(), "AttackHipDrop");
    mAttackCount = 0;

    if (KoopaFunction::isKoopaVs1(getHost())) {
        mAttackLimit = 3;
        mWaitStep = 30;
        setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvRun::sInstance);
    } else if (KoopaFunction::isKoopaVs2(getHost())) {
        mAttackLimit = 2;
        mWaitStep = 15;
        setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpUp::sInstance);
    } else {
        mWaitStep = 15;

        if (KoopaFunction::isKoopaAngry(getHost())) {
            mAttackLimit = 2;
        } else {
            mAttackLimit = 3;
        }

        if (MR::isNearPlayer(getHost(), 2000.0f)) {
            setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpUp::sInstance);
        } else {
            setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvRun::sInstance);
        }
    }
}

void KoopaStateAttackHipDrop::kill() {
    KoopaFunction::endKoopaCamera(getHost(), "ヒップドロップ攻撃", false, -1);
    KoopaFunction::endKoopaCamera(getHost(), "ヒップドロップ着地", false, -1);
    KoopaFunction::endKoopaCamera(getHost(), "ヒップドロップ着地（マリオ痺れ）", false, -1);
    MR::invalidateHitSensor(getHost(), "AttackHipDrop");
    mIsDead = true;
}

bool KoopaStateAttackHipDrop::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpAttackDown::sInstance)) {
        if (MR::isSensor(pSender, "AttackHipDrop")) {
            if (MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
                return true;
            }

            if (MR::sendArbitraryMsg(192, pReceiver, pSender)) {
                return true;
            }
        }
    }

    if (KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver)) {
        return false;
    }

    return KoopaFunction::tryKoopaBodyAttackPlayerMaximum(pSender, pReceiver);
}

bool KoopaStateAttackHipDrop::isDamage() const {
    return isNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvDamage::sInstance);
}

bool KoopaStateAttackHipDrop::isEnableGuard() const {
    if (isNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvRun::sInstance) ||
        isNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpStart::sInstance) ||
        isNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpUp::sInstance) ||
        isNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvLand::sInstance)) {
        return true;
    }

    return false;
}

void KoopaStateAttackHipDrop::exeRun() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Run");
        KoopaFunction::startKoopaCamera(getHost(), "ヒップドロップ攻撃");
    }

    MR::moveAndTurnToPlayer(getHost(), &getHost()->mFront, sParamRunStart);

    if (!MR::sendMsgEnemyAttackToBindedSensor(getHost(), getHost()->getSensor("Body"))) {
        if (MR::isBindedWall(getHost())) {
            MR::addVelocityJump(getHost(), 10.0f);
        }

        if (MR::isNearPlayer(getHost(), 1000.0f) || MR::isGreaterStep(this, 180)) {
            if (MR::calcGravitySpeed(getHost()) >= 0.0f) {
                setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpStart::sInstance);
            }
        }
    }
}

void KoopaStateAttackHipDrop::exeJumpStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "HipDropJumpStart");
    }

    MR::moveAndTurnToPlayer(getHost(), &getHost()->mFront, sParamJumpStart);

    if (MR::isBckStopped(getHost())) {
        setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpUp::sInstance);
    }
}

void KoopaStateAttackHipDrop::exeJumpUp() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "HipDropJump");
        MR::startSound(getHost(), "SE_BM_KOOPA_JUMP", -1, -1);

        MR::vecKillElement(getHost()->mVelocity, getHost()->mGravity, &getHost()->mVelocity);
        MR::restrictVelocity(getHost(), 20.0f);
        MR::addVelocityJump(getHost(), 50.0f);
        KoopaFunction::startKoopaCamera(getHost(), "ヒップドロップ攻撃");
    }

    MR::addVelocityToGravity(getHost(), 1.0f);
    MR::turnDirectionToPlayerDegree(getHost(), KoopaFunction::getKoopaFrontPtr(getHost()), 3.0f);

    if (getHost()->mVelocity.dot(getHost()->mGravity) >= 0.0f) {
        setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpWaitAir::sInstance);
    }
}

void KoopaStateAttackHipDrop::exeJumpWaitAir() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(getHost());
    }

    MR::turnDirectionToPlayerDegree(getHost(), &getHost()->mFront, 2.0f);

    if (MR::isStep(this, 0)) {
        MR::getPlayerGroundPos(&mTargetPos);
    }

    if (MR::isStep(this, mWaitStep)) {
        setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvJumpAttackDown::sInstance);
    }
}

void KoopaStateAttackHipDrop::exeJumpAttackDown() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "HipDropAttackDown");

        TVec3f v(mTargetPos);
        v.subInline(getHost()->mPosition);

        mMaxFallStep = (s32)(PSVECMag(&v) / 35.0f) + 10;

        MR::normalizeOrZero(&v);
        v.x *= 35.0f;
        v.y *= 35.0f;
        v.z *= 35.0f;

        MR::setVelocity(getHost(), v);
        MR::startSound(getHost(), "SE_BV_KOOPA_HIPDROP", -1, -1);
    }

    if (MR::isBindedGround(getHost()) || MR::isGreaterStep(this, mMaxFallStep)) {
        if (!MR::sendMsgEnemyAttackToBindedSensor(getHost(), getHost()->getSensor("Body"))) {
            if (MR::isBindedGround(getHost()) && MR::sendMsgToBindedSensor(192, getHost(), getHost()->getSensor("Body"))) {
                MR::startSound(getHost(), "SE_BM_KOOPA_LAND", -1, -1);
                MR::stopScene(15);

                if (MR::sendMsgToBindedSensor(194, KoopaFunction::getKoopaMessageSensor(getHost()))) {
                    MR::zeroVelocity(getHost());
                    MR::tryRumblePadAndCameraDistanceVeryStrong(getHost(), 1500.0f, 3000.0f, 2000.0f);
                    MR::startSound(getHost(), "SE_OJ_KOOPA_PLATE_LAVA_ST", -1, -1);
                    setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvDamage::sInstance);
                } else {
                    MR::tryRumblePadAndCameraDistanceMiddle(getHost(), 1500.0f, 3000.0f, 2000.0f);
                }
            } else {
                MR::zeroVelocity(getHost());
                MR::tryRumblePadAndCameraDistanceStrong(getHost(), 1500.0f, 3000.0f, 2000.0f);
                KoopaFunction::emitKoopaShockWave(getHost());
                KoopaFunction::startKoopaCamera(getHost(), "ヒップドロップ着地");
                MR::startSound(getHost(), "SE_BM_KOOPA_LAND", -1, -1);
                setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvLand::sInstance);
            }
        }
    }
}

void KoopaStateAttackHipDrop::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "HipDropLand");
    }

    MR::moveAndTurnToPlayer(getHost(), &getHost()->mFront, sFallParam);

    if (MR::isBckStopped(getHost())) {
        if (++mAttackCount >= mAttackLimit || MR::isPlayerDamaging()) {
            kill();
        } else {
            setNerve(&NrvKoopaStateAttackHipDrop::KoopaStateAttackHipDropNrvRun::sInstance);
        }
    }
}

void KoopaStateAttackHipDrop::exeDamage() {
}
