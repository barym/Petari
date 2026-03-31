#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class KoopaRockBreak : public PartsModel {
public:
    KoopaRockBreak(const char*);

    virtual ~KoopaRockBreak();
    virtual void init(const JMapInfoIter&);
    virtual void control();
};
