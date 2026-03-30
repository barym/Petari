#include "Game/Boss/KoopaStateJumpAway.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

namespace NrvKoopaStateJumpAway {
    NEW_NERVE(KoopaStateJumpAwayNrvJumpStart, KoopaStateJumpAway, JumpStart);
    NEW_NERVE(KoopaStateJumpAwayNrvJump, KoopaStateJumpAway, Jump);
    NEW_NERVE(KoopaStateJumpAwayNrvLand, KoopaStateJumpAway, Land);
};  // namespace NrvKoopaStateJumpAway

KoopaStateJumpAway::KoopaStateJumpAway(Koopa* pKoopa) : ActorStateBase("State[ジャンプで離れる]", pKoopa) {
}

KoopaStateJumpAway::~KoopaStateJumpAway() {
}

void KoopaStateJumpAway::init() {
    initNerve(&NrvKoopaStateJumpAway::KoopaStateJumpAwayNrvJumpStart::sInstance);
    kill();
}

void KoopaStateJumpAway::appear() {
    mIsDead = false;
    setNerve(&NrvKoopaStateJumpAway::KoopaStateJumpAwayNrvJumpStart::sInstance);
}

void KoopaStateJumpAway::exeJumpStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "JumpAwayStart");
        MR::zeroVelocity(getHost());
    }

    if (MR::isActionEnd(getHost())) {
        setNerve(&NrvKoopaStateJumpAway::KoopaStateJumpAwayNrvJump::sInstance);
    }
}

void KoopaStateJumpAway::exeJump() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "JumpAway");
        MR::setVelocity(getHost(), KoopaFunction::getKoopaFront(getHost()).multInLine(-20.0f));
        MR::addVelocityJump(getHost(), 35.0f);
        MR::startSound(getHost(), "SE_BM_KOOPA_JUMP", -1, -1);
        MR::startSound(getHost(), "SE_BV_KOOPA_JUMP", -1, -1);
    }

    MR::addVelocityToGravity(getHost(), 1.0f);

    TVec3f vel(getHost()->mVelocity);
    MR::vecKillElement(vel, getHost()->mGravity, &vel);

    if (!MR::isNearZero(vel, 0.001f)) {
        MR::normalize(&vel);
        KoopaFunction::getKoopaFrontPtr(getHost())->set(vel.negateInline());
    }

    if (!MR::sendMsgEnemyAttackToBindedSensor(getHost(), getHost()->getSensor("Body"))) {
        if (!MR::isFirstStep(this)) {
            if (getHost()->mVelocity.dot(getHost()->mGravity) >= 0.0f) {
                if (MR::isBindedGround(getHost())) {
                    MR::tryRumblePadAndCameraDistanceStrong(getHost(), 1500.0f, 2000.0f, 2000.0f);
                    MR::startSound(getHost(), "SE_BM_KOOPA_LAND", -1, -1);
                    MR::zeroVelocity(getHost());
                    setNerve(&NrvKoopaStateJumpAway::KoopaStateJumpAwayNrvLand::sInstance);
                }
            }
        }
    }
}

void KoopaStateJumpAway::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "JumpAwayLand");
    }

    if (KoopaFunction::isKoopaVs3(getHost()) && KoopaFunction::isKoopaLv3(getHost()) && MR::isGreaterStep(this, 30)) {
        kill();
    } else if (MR::isActionEnd(getHost())) {
        kill();
    }
}
