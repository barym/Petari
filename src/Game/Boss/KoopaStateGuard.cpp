#include "Game/Boss/KoopaStateGuard.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaStateAttackFireShort.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvKoopaStateGuard {
    NEW_NERVE(KoopaStateGuardNrvGuardFace, KoopaStateGuard, GuardFace);
    NEW_NERVE(KoopaStateGuardNrvGuardBody, KoopaStateGuard, GuardBody);
    NEW_NERVE(KoopaStateGuardNrvGuardTail, KoopaStateGuard, GuardTail);
};  // namespace NrvKoopaStateGuard

static MR::ActorMoveParam sGuardFallParam = {0.0f, 1.0f, 0.98f, 0.0f};

KoopaStateGuard::KoopaStateGuard(Koopa* pKoopa) : ActorStateBase("State[ガード攻撃]", pKoopa) {
}

KoopaStateGuard::~KoopaStateGuard() {
}

void KoopaStateGuard::init() {
    initNerve(&NrvKoopaStateGuard::KoopaStateGuardNrvGuardFace::sInstance);
    kill();
}

bool KoopaStateGuard::tryStart(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pSender) || !MR::isMsgPlayerSpinAttack(msg)) {
        return false;
    }

    if (MR::isSensor(pReceiver, "Face")) {
        MR::sendMsgEnemyAttackFlip(pSender, pReceiver);
        setNerve(&NrvKoopaStateGuard::KoopaStateGuardNrvGuardFace::sInstance);
        return true;
    }

    if (MR::isSensor(pReceiver, "GuardBody")) {
        MR::sendMsgEnemyAttackFlip(pSender, pReceiver);
        setNerve(&NrvKoopaStateGuard::KoopaStateGuardNrvGuardBody::sInstance);
        return true;
    }

    if (MR::isSensor(pReceiver, "Tail") || MR::isSensor(pReceiver, "TailTop")) {
        MR::sendMsgEnemyAttackFlipMaximum(pSender, pReceiver);
        setNerve(&NrvKoopaStateGuard::KoopaStateGuardNrvGuardTail::sInstance);
        return true;
    }

    return false;
}

void KoopaStateGuard::exeGuardFace() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "GuardFace");
        MR::startSound(getHost(), "SE_BV_KOOPA_GUARD", -1, -1);
        MR::zeroVelocity(getHost());
    }

    MR::moveAndTurnToPlayer(getHost(), KoopaFunction::getKoopaFrontPtr(getHost()), sGuardFallParam);

    if (MR::isActionEnd(getHost())) {
        kill();
    }
}

void KoopaStateGuard::exeGuardBody() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "GuardBody");
        MR::startSound(getHost(), "SE_BV_KOOPA_GUARD", -1, -1);
        MR::zeroVelocity(getHost());
    }

    MR::moveAndTurnToPlayer(getHost(), KoopaFunction::getKoopaFrontPtr(getHost()), sGuardFallParam);

    if (MR::isActionEnd(getHost())) {
        kill();
    }
}

void KoopaStateGuard::exeGuardTail() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "AttackTail");
        MR::zeroVelocity(getHost());
    }

    MR::moveAndTurnToPlayer(getHost(), KoopaFunction::getKoopaFrontPtr(getHost()), sGuardFallParam);

    if (MR::isActionEnd(getHost())) {
        kill();
    }
}
