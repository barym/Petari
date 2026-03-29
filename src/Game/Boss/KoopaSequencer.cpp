#include "Game/Boss/KoopaSequencer.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaDemoJumpToPlanet.hpp"
#include "Game/Boss/KoopaDemoPowerUp.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"

KoopaSequencer::KoopaSequencer(const char* pName, s32 a2)
    : NerveExecutor(pName), mKoopa(nullptr), mDemoPowerUp(nullptr), _10(nullptr), mDemoJumpToPlanet(nullptr), mLevel(1), _1C(a2), _20(false) {
}

KoopaSequencer::~KoopaSequencer() {
}

void KoopaSequencer::init(Koopa* pKoopa, const JMapInfoIter& rIter) {
    mKoopa = pKoopa;

    mDemoPowerUp = new KoopaDemoPowerUp(mKoopa);
    mDemoPowerUp->init();

    mDemoJumpToPlanet = new KoopaDemoJumpToPlanet(mKoopa);
    mDemoJumpToPlanet->init();
}

void KoopaSequencer::update() {
    updateNerve();
}

void KoopaSequencer::calcAndSetBaseMtx() {
    MR::calcAndSetBaseMtxFromGravityAndZAxis((LiveActor*)mKoopa, mKoopa->mFront);
}

void KoopaSequencer::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
}

bool KoopaSequencer::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool KoopaSequencer::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool KoopaSequencer::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

void KoopaSequencer::startBattleLevel2() {
    mLevel = 2;
    _20 = false;
}

void KoopaSequencer::startBattleLevel3() {
    mLevel = 3;
    _20 = false;
}
