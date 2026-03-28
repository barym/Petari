#pragma once

#include "Game/System/NerveExecutor.hpp"

class Koopa;

class KoopaStateDamageEscape : public NerveExecutor {
public:
    KoopaStateDamageEscape(Koopa*);

    /* 0x08 */ char _08[0x2C];
};
