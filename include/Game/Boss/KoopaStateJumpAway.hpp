#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaStateJumpAway : public ActorStateBase< Koopa > {
public:
    KoopaStateJumpAway(Koopa*);
};
