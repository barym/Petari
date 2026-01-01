#ifndef RVL_FACE_LIBRARY_INTERNAL_NAND_ACCESS_H
#define RVL_FACE_LIBRARY_INTERNAL_NAND_ACCESS_H

#include <RFL_Types.h>
#include <RFLi_Types.h>

#include <revolution/mem.h>
#include <revolution/nand.h>
#include <revolution/os/OSAlarm.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RFLiAsyncTag_WriteFile,
    RFLiAsyncTag_WriteCloseFile,
    RFLiAsyncTag_ReadFile,
    RFLiAsyncTag_ReadCloseFile,
    RFLiAsyncTag_ReadCache,
    RFLiAsyncTag_ReadCacheClose,
    RFLiAsyncTag_WriteHDBFile,
    RFLiAsyncTag_WriteHDBFileClose,
    RFLiAsyncTag_ReadHDBFile,
    RFLiAsyncTag_ReadHDBFileClose,
    RFLiAsyncTag_CreateAsync,
    RFLiAsyncTag_OpenAsync,
    RFLiAsyncTag_SeekAsync,
    RFLiAsyncTag_ReadAsync,
    RFLiAsyncTag_WriteAsync,
    RFLiAsyncTag_CloseAsync,
    RFLiAsyncTag_GetLengthAsync,
    RFLiAsyncTag_DeleteAsync,
    RFLiAsyncTag_CreateDirAsync,

    RFLiAsyncTag_Max
} RFLiAsyncTag;

typedef enum {
    RFLiFileType_Database,
    RFLiFileType_Resource,

    RFLiFileType_Max
} RFLiFileType;

typedef struct RFLiCallbackTag {
    /* 0x0 */ RFLiAsyncTag tag;
    /* 0x4 */ RFLiFileType type;
} RFLiCallbackTag;

typedef struct RFLiAccessInfo {
    /* 0x0 */ RFLiCallback callback;
    /* 0x04 */
    union {
        struct {
            /* 0x00 */ char path[NAND_MAX_PATH + 1];
            /* 0x41 */ u8 openMode;
            /* 0x42 */ u8 perm;
            /* 0x43 */ u8 attr;
        } openInfo;

        struct {
            /* 0x0 */ void* dst;
            /* 0x4 */ u32 size;
            /* 0x8 */ s32 offset;
        } readInfo;

        struct {
            /* 0x0 */ const void* src;
            /* 0x4 */ u32 size;
            /* 0x8 */ s32 offset;
        } writeInfo;

        struct {
            /* 0x0 */ u8 count;
        } closeInfo;

        struct {
            /* 0x0 */ u32* dst;
        } getLengthInfo;
    };
    /* 0x48 */ NANDFileInfo file;
    /* 0xD4 */ NANDCommandBlock block;
    /* 0x190 */ RFLiCallbackTag tag;
    /* 0x198 */ OSAlarm alarm;
    /* 0x1C8 */ RFLiFileType alarmData;
    /* 0x1CC */ RFLiExCallback retryCallback;
    /* 0x1D0 */ u8 retryCount;
    /* 0x1D4 */ void* safeBuffer;
    /* 0x1D8 */ u8 opened;
} RFLiAccessInfo;

/// @brief Initializes the NAND access info system.
/// @param heap The heap to use for allocations.
void RFLiInitAccessInfo(MEMiHeapHead*);

/// @brief Exits the NAND access info system.
void RFLiExitAccessInfo();

/// @brief Checks if the NAND access system is currently working.
/// @returns `TRUE` if working, `FALSE` otherwise.
BOOL RFLiIsWorking();

/// @brief Starts a NAND access operation.
void RFLiStartWorking();

/// @brief Ends a NAND access operation with a specific error and reason.
/// @param err The error code.
/// @param reason The NAND result reason.
void RFLiEndWorkingReason(RFLErrcode, s32);

/// @brief Ends a NAND access operation with a specific error.
/// @param err The error code.
void RFLiEndWorking(RFLErrcode);

/// @brief Sets up a command block for a NAND operation.
/// @param type The file type.
/// @param tag The async tag.
/// @returns The configured command block.
NANDCommandBlock* RFLiSetCommandBlock(RFLiFileType, RFLiAsyncTag);

/// @brief Gets the file type from a command block.
/// @param block The command block.
/// @returns The file type.
RFLiFileType RFLiGetType(NANDCommandBlock*);

/// @brief Gets the working file info for a file type.
/// @param type The file type.
/// @returns The file info, or `NULL` if not available.
NANDFileInfo* RFLiGetWorkingFile(RFLiFileType);

/// @brief Asynchronously opens a file.
/// @param type The file type.
/// @param openMode The open mode.
/// @param callback The completion callback.
/// @returns The async status.
RFLErrcode RFLiOpenAsync(RFLiFileType, u8, RFLiCallback);

/// @brief Asynchronously reads from a file.
/// @param type The file type.
/// @param dst The destination buffer.
/// @param size The number of bytes to read.
/// @param callback The completion callback.
/// @param offset The file offset.
/// @returns The async status.
RFLErrcode RFLiReadAsync(RFLiFileType, void*, u32, RFLiCallback, s32);

/// @brief Asynchronously writes to a file.
/// @param type The file type.
/// @param src The source buffer.
/// @param size The number of bytes to write.
/// @param callback The completion callback.
/// @param offset The file offset.
/// @returns The async status.
RFLErrcode RFLiWriteAsync(RFLiFileType, const void*, u32, RFLiCallback, s32);

/// @brief Asynchronously closes a file.
/// @param type The file type.
/// @param callback The completion callback.
/// @returns The async status.
RFLErrcode RFLiCloseAsync(RFLiFileType, RFLiCallback);

/// @brief Asynchronously gets the length of a file.
/// @param type The file type.
/// @param out The pointer to store the length.
/// @param callback The completion callback.
/// @returns The async status.
RFLErrcode RFLiGetLengthAsync(RFLiFileType, u32*, RFLiCallback);

/// @brief Asynchronously deletes a file.
/// @param type The file type.
/// @param callback The completion callback.
/// @returns The async status.
RFLErrcode RFLiDeleteAsync(RFLiFileType, RFLiCallback);

/// @brief Asynchronously creates the save directory.
/// @param callback The completion callback.
/// @returns The async status.
RFLErrcode RFLiCreateSaveDirAsync(RFLiCallback);

#ifdef __cplusplus
}
#endif

#endif  // RVL_FACE_LIBRARY_INTERNAL_NAND_ACCESS_H
