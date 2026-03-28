#pragma once

#include "Game/System/NerveExecutor.hpp"

class Koopa;

class KoopaStateGuard : public NerveExecutor {
public:
    KoopaStateGuard(Koopa*);

    /* 0x08 */ char _08[0x6];
};
