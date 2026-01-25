#include "JSystem/JAudio2/JASSoundParams.hpp"

void JASSoundParams::combine(JASSoundParams const& a, JASSoundParams const& b) {
    mVolume = a.mVolume * b.mVolume;
    mPitch = a.mPitch * b.mPitch;
    mFxMix = a.mFxMix + b.mFxMix;
    mPan = a.mPan + b.mPan - 0.5f;
    mDolby = a.mDolby + b.mDolby;
}
