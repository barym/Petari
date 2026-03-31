#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Koopa;

class KoopaFigureBall : public LiveActor {
public:
    KoopaFigureBall(const char*, Koopa*, f32, const MR::ActorMoveParam*);
    virtual ~KoopaFigureBall();

    /* 0x8D */ char _8D[0xA0 - 0x8D];
    /* 0xA0 */ f32 _A0;
    /* 0xA4 */ char _A4[0xD4 - 0xA4];
    /* 0xD4 */ const MR::ActorMoveParam* mMoveParam;
};
