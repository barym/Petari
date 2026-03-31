#include "Game/Boss/KoopaStateAttackShockWave.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

namespace NrvKoopaStateAttackShockWave {
    NEW_NERVE(KoopaStateAttackShockWaveNrvJumpStart, KoopaStateAttackShockWave, JumpStart);
    NEW_NERVE(KoopaStateAttackShockWaveNrvJump, KoopaStateAttackShockWave, Jump);
    NEW_NERVE(KoopaStateAttackShockWaveNrvJumpFast, KoopaStateAttackShockWave, JumpFast);
    NEW_NERVE(KoopaStateAttackShockWaveNrvLand, KoopaStateAttackShockWave, Land);
};  // namespace NrvKoopaStateAttackShockWave

KoopaStateAttackShockWave::KoopaStateAttackShockWave(Koopa* pKoopa)
    : ActorStateBase("State[衝撃波攻撃]", pKoopa), mAttackLimit(1), mAttackCount(0), mJumpVelocity(40.0f), mGravity(1.5f), mWaitStep(15) {
}

KoopaStateAttackShockWave::~KoopaStateAttackShockWave() {
}

void KoopaStateAttackShockWave::init() {
    initNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpStart::sInstance);
    kill();
}

void KoopaStateAttackShockWave::appear() {
    mIsDead = false;
    mAttackCount = 0;
    MR::validateHitSensor(getHost(), "AttackHipDrop");

    if (KoopaFunction::isKoopaVs1(getHost())) {
        if (KoopaFunction::isKoopaLv3(getHost())) {
            mAttackLimit = 3;
        } else if (KoopaFunction::isKoopaLv2(getHost())) {
            mAttackLimit = 2;
        } else {
            mAttackLimit = 1;
        }

        mJumpVelocity = 40.0f;
        mGravity = 1.5f;
        mWaitStep = 15;
    } else if (KoopaFunction::isKoopaVs2(getHost())) {
        if (KoopaFunction::isKoopaLv3(getHost())) {
            mAttackLimit = 5;
        } else {
            mAttackLimit = 3;
        }

        mJumpVelocity = 40.0f;
        mGravity = 1.5f;
        mWaitStep = 10;
    } else if (KoopaFunction::isKoopaLv1(getHost())) {
        mAttackLimit = 3;

        if (KoopaFunction::isKoopaAngry(getHost())) {
            mJumpVelocity = 40.0f;
            mGravity = 2.0f;
            mWaitStep = 5;
        } else {
            mJumpVelocity = 40.0f;
            mGravity = 1.5f;
            mWaitStep = 10;
        }
    } else {
        mJumpVelocity = 40.0f;
        mGravity = 2.0f;

        if (KoopaFunction::isKoopaAngry(getHost())) {
            mAttackLimit = 5;
            mWaitStep = 5;
        } else {
            mAttackLimit = 3;
            mWaitStep = 10;
        }
    }

    setNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpStart::sInstance);
}

void KoopaStateAttackShockWave::kill() {
    MR::invalidateHitSensor(getHost(), "AttackHipDrop");
    mIsDead = true;
}

bool KoopaStateAttackShockWave::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJump::sInstance) ||
        isNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpFast::sInstance)) {
        if (MR::isSensor(pSender, "AttackHipDrop")) {
            if (MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
                return true;
            }
        }
    }

    if (KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver)) {
        return true;
    }

    if (KoopaFunction::tryKoopaBodyAttackPlayer(pSender, pReceiver)) {
        return true;
    }

    return KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver);
}

void KoopaStateAttackShockWave::exeJumpStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "ShockWaveJumpStart");
    }

    if (MR::isBckStopped(getHost())) {
        setNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJump::sInstance);
    }
}

void KoopaStateAttackShockWave::exeJump() {
    if (MR::isFirstStep(this)) {
        if (mAttackCount == mAttackLimit - 1) {
            MR::startAction(getHost(), "ShockWaveJump3rd");
        } else if (isNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpFast::sInstance)) {
            MR::startAction(getHost(), "JumpFast");
        } else {
            MR::startAction(getHost(), "ShockWaveJump");
        }

        MR::startSound(getHost(), "SE_BM_KOOPA_JUMP", -1, -1);

        if (!mAttackCount) {
            MR::setVelocityJump(getHost(), 50.0f);
        } else {
            MR::setVelocityJump(getHost(), mJumpVelocity);
        }
    }

    MR::addVelocityToGravity(getHost(), mGravity);

    if (KoopaFunction::isKoopaVs3(getHost())) {
        if (KoopaFunction::isKoopaLv3(getHost())) {
            MR::turnDirectionToPlayerDegree(getHost(), KoopaFunction::getKoopaFrontPtr(getHost()), 3.0f);
        }
    }

    if (!MR::sendMsgEnemyAttackToBindedSensor(getHost(), getHost()->getSensor("Body"))) {
        if (MR::isOnGround(getHost())) {
            MR::tryRumblePadAndCameraDistanceMiddle(getHost(), 1500.0f, 2500.0f, 2000.0f);
            MR::shakeCameraWeak();
            MR::startSound(getHost(), "SE_BM_KOOPA_LAND", -1, -1);
            KoopaFunction::emitKoopaShockWave(getHost());

            getHost()->mVelocity.zero();

            setNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvLand::sInstance);
        }
    }
}

void KoopaStateAttackShockWave::exeJumpFast() {
    exeJump();
}

void KoopaStateAttackShockWave::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "ShockWaveLand");
    }

    if (MR::isStep(this, mWaitStep)) {
        mAttackCount++;

        if (mAttackCount < mAttackLimit) {
            setNerve(&NrvKoopaStateAttackShockWave::KoopaStateAttackShockWaveNrvJumpFast::sInstance);
            return;
        }
    }

    if (MR::isActionEnd(getHost())) {
        kill();
    }
}
