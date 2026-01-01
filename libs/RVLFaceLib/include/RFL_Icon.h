#ifndef RVL_FACE_LIBRARY_ICON_H
#define RVL_FACE_LIBRARY_ICON_H

#include <RFL_MiddleDatabase.h>
#include <RFL_Types.h>
#include <RFLi_Types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { RFLIconBG_Favorite = 0, RFLIconBG_Direct } RFLIconBGType;

typedef struct {
    /* 0x0 */ u16 width;
    /* 0x2 */ u16 height;
    /* 0x4 */ RFLIconBGType bgType;
    /* 0x8 */ GXColor bgColor;
    /* 0xC */ BOOL drawXluOnly;
} RFLIconSetting;

/// @brief Creates a Mii icon from a data source.
/// @param buf The destination buffer for the icon.
/// @param source The data source to pick the Mii from.
/// @param middleDB The middle database to use if source is `RFLDataSource_Middle`.
/// @param index The index of the Mii in the source.
/// @param expression The expression to use for the icon.
/// @param setting The icon settings.
/// @returns The result of the operation.
RFLErrcode RFLMakeIcon(void*, RFLDataSource, RFLMiddleDB*, u16, RFLExpression, const RFLIconSetting*);

/// @brief Internal function to create a Mii icon.
/// @param buf The destination buffer for the icon.
/// @param info The character info.
/// @param expression The expression to use for the icon.
/// @param setting The icon settings.
void RFLiMakeIcon(void*, RFLiCharInfo*, RFLExpression, const RFLIconSetting*);

/// @brief Sets a callback to be called after the icon drawing is finished.
/// @param callback The callback function.
void RFLSetIconDrawDoneCallback(RFLCallback);

#ifdef __cplusplus
}
#endif

#endif  // RVL_FACE_LIBRARY_ICON_H
