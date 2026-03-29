#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;
class HitSensor;

class KoopaStateGuard : public ActorStateBase< Koopa > {
public:
    KoopaStateGuard(Koopa*);
    bool tryStart(u32, HitSensor*, HitSensor*);
};
