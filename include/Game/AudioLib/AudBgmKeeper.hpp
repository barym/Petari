#pragma once

#include "Game/AudioLib/AudBgm.hpp"

class AudBgmKeeper {
public:
    enum BgmType {
        BGM_TYPE_SINGLE = 0,
        BGM_TYPE_MULTI = 1,
    };

    /// @brief Creates a new `AudBgmKeeper`.
    AudBgmKeeper();

    /// @brief Gets a valid `AudBgm` of the specified type.
    /// @param type The type of BGM to get.
    /// @return A valid `AudBgm` if available, otherwise `nullptr`.
    AudBgm* get(BgmType);

    /// @brief Releases the specified `AudBgm`.
    /// @param target The `AudBgm` to release.
    void release(AudBgm*);

private:
    /// @brief Gets a valid `AudSingleBgm`.
    /// @return A valid `AudSingleBgm` if available, otherwise `nullptr`.
    AudSingleBgm* getValidSingleBgm();

    /// @brief Gets a valid `AudMultiBgm`.
    /// @return A valid `AudMultiBgm` if available, otherwise `nullptr`.
    AudMultiBgm* getValidMultiBgm();

    /* 0x000 */ AudSingleBgm mSingleBgm[2];
    /* 0x3B8 */ AudMultiBgm mMultiBgm[2];
    /* 0x7B8 */ u8 singleBgmUseFlags;
    /* 0x7B9 */ u8 multiBgmUseFlags;
};
