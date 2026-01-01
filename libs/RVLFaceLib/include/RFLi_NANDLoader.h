#ifndef RVL_FACE_LIBRARY_INTERNAL_NAND_LOADER_H
#define RVL_FACE_LIBRARY_INTERNAL_NAND_LOADER_H
#include <RFL_NANDLoader.h>
#include <RFLi_Types.h>
#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RFLiArcID_Beard,
    RFLiArcID_Eye,
    RFLiArcID_Eyebrow,
    RFLiArcID_Faceline,
    RFLiArcID_FaceTex,
    RFLiArcID_ForeHead,
    RFLiArcID_Glass,
    RFLiArcID_GlassTex,
    RFLiArcID_Hair,
    RFLiArcID_Mask,
    RFLiArcID_Mole,
    RFLiArcID_Mouth,
    RFLiArcID_Mustache,
    RFLiArcID_Nose,
    RFLiArcID_Nline,
    RFLiArcID_NlineTex,
    RFLiArcID_Cap,
    RFLiArcID_CapTex,
    RFLiArcID_Max
} RFLiArcID;

typedef struct RFLiArchive {
    /* 0x0 */ u16 numFiles;
    /* 0x2 */ u16 maxSize;
    /* 0x4 */ u32 offset;
} RFLiArchive;

typedef struct RFLiLoader {
    /* 0x00 */ RFLiArchive archives[RFLiArcID_Max];
    /* 0x90 */ u32 cacheSize;
    /* 0x94 */ void* cache;
    /* 0x98 */ BOOL cached;
    /* 0x9C */ u16 version;
    /* 0xA0 */ u8* headerBuf1;
    /* 0xA4 */ u8* headerBuf2;
    /* 0xA8 */ u8 numResources;
} RFLiLoader;

/// @brief Initializes the NAND loader.
void RFLiInitLoader();

/// @brief Asynchronously loads the resource header.
/// @return An error code indicating the result of the operation.
RFLErrcode RFLiLoadResourceHeaderAsync();

/// @brief Gets the size of a texture part.
/// @param part The texture part type.
/// @param fileIndex The index of the file.
/// @return The size of the texture part.
u32 RFLiGetTexSize(RFLiPartsTex, u16);

/// @brief Loads a texture part.
/// @param part The texture part type.
/// @param fileIndex The index of the file.
/// @param dst The destination buffer.
/// @return A pointer to the loaded texture.
struct RFLiTexture* RFLiLoadTexture(RFLiPartsTex, u16, void*);

/// @brief Gets the size of a shape texture part.
/// @param part The shape texture part type.
/// @param fileIndex The index of the file.
/// @return The size of the shape texture part.
u32 RFLiGetShpTexSize(RFLiPartsShpTex, u16);

/// @brief Loads a shape texture part.
/// @param part The shape texture part type.
/// @param fileIndex The index of the file.
/// @param dst The destination buffer.
/// @return A pointer to the loaded shape texture.
struct RFLiTexture* RFLiLoadShpTexture(RFLiPartsShpTex, u16, void*);

/// @brief Gets the size of a shape part.
/// @param part The shape part type.
/// @param fileIndex The index of the file.
/// @return The size of the shape part.
u32 RFLiGetShapeSize(RFLiPartsShp, u16);

/// @brief Loads a shape part.
/// @param part The shape part type.
/// @param fileIndex The index of the file.
/// @param dst The destination buffer.
/// @return A pointer to the loaded shape.
void* RFLiLoadShape(RFLiPartsShp, u16, void*);

#ifdef __cplusplus
}
#endif

#endif  // RVL_FACE_LIBRARY_INTERNAL_NAND_LOADER_H
