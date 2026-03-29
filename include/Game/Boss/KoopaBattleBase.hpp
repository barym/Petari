#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/Util/ActorMovementUtil.hpp"

class Nerve;
class Koopa;
class KoopaStateDamageEscape;
class KoopaStateGuard;

class KoopaBattleBase : public ActorStateBase< Koopa > {
public:
    KoopaBattleBase(const char*, Koopa*);

    virtual ~KoopaBattleBase();

    void init();
    void kill();
    void updateChasePlayer(const MR::ActorMoveParam&);
    bool updateWander(const MR::ActorMoveParam&);
    bool updateSearch();
    bool updateFind();
    bool updateRecover(const Nerve*);

    /* 0x10 */ KoopaStateDamageEscape* mStateDamageEscape;
    /* 0x14 */ KoopaStateGuard* mStateGuard;
    /* 0x18 */ s32 mCurrentStep;
};

namespace MR {
    void moveAndTurnToPlayer(LiveActor*, TVec3f*, const MR::ActorMoveParam&) NO_INLINE;
};
