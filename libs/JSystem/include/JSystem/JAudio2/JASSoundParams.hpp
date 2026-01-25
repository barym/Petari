#pragma once

#include <revolution/types.h>

struct JASSoundParams {
    /// @brief Combines two JASSoundParams into this one.
    /// @param a First sound params.
    /// @param b Second sound params.
    void combine(JASSoundParams const&, JASSoundParams const&);

    /* 0x00 */ f32 mVolume;
    /* 0x04 */ f32 mFxMix;
    /* 0x08 */ f32 mPitch;
    /* 0x0C */ f32 mPan;
    /* 0x10 */ f32 mDolby;
};
