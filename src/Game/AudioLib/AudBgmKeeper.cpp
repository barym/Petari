#include "Game/AudioLib/AudBgmKeeper.hpp"
#include "Game/AudioLib/AudBgm.hpp"
#include <revolution/types.h>

AudBgmKeeper::AudBgmKeeper() : singleBgmUseFlags(0), multiBgmUseFlags(0) {}

AudBgm* AudBgmKeeper::get(BgmType type) {
    AudBgm* bgm;
    switch (type) {
    case BGM_TYPE_SINGLE:
        bgm = AudBgmKeeper::getValidSingleBgm();
        if (bgm != nullptr) {
            return bgm;
        }
        break;
    case BGM_TYPE_MULTI:
        bgm = AudBgmKeeper::getValidMultiBgm();
        if (bgm != nullptr) {
            return bgm;
        }
        break;
    }

    return nullptr;
}

void AudBgmKeeper::release(AudBgm* target) {
    u8 mask = 1;
    for (int i = 0; i < 2; i++) {
        AudSingleBgm* current = mSingleBgm + i;
        if (current == target) {
            current->init();
            singleBgmUseFlags &= ~mask;
            return;
        }
        mask <<= 1;
    }

    mask = 1;
    for (int i = 0; i < 2; i++) {
        AudMultiBgm* current = mMultiBgm + i;
        if (current == target) {
            current->init();
            multiBgmUseFlags &= ~mask;
            return;
        }
        mask <<= 1;
    }
}

AudSingleBgm* AudBgmKeeper::getValidSingleBgm() {
    u8 mask = 1;
    for (int i = 0; i < 2; i++) {
        if ((singleBgmUseFlags & mask) == 0) {
            singleBgmUseFlags |= mask;
            return &mSingleBgm[i];
        }
        mask <<= 1;
    }

    return nullptr;
}

AudMultiBgm* AudBgmKeeper::getValidMultiBgm() {
    u8 mask = 1;
    for (int i = 0; i < 2; i++) {
        if ((multiBgmUseFlags & mask) == 0) {
            multiBgmUseFlags |= mask;
            return &mMultiBgm[i];
        }
        mask <<= 1;
    }

    return nullptr;
}
