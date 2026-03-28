#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaDemoJumpToPlanet : public ActorStateBase< Koopa > {
public:
    KoopaDemoJumpToPlanet(Koopa*);
};
