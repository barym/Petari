#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaStateAttackFireShort : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackFireShort(Koopa*);

    /* 0x10 */ char _10[0xC];
};
