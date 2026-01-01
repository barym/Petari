#ifndef RVL_FACE_LIBRARY_INTERNAL_HIDDEN_DATABASE_H
#define RVL_FACE_LIBRARY_INTERNAL_HIDDEN_DATABASE_H

#include <RFL_Database.h>
#include <RFL_Types.h>
#include <RFLi_Controller.h>
#include <RFLi_Format.h>
#include <RFLi_Types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RFLi_HDB_DATA_MAX (10000)

typedef struct RFLiHiddenDB {
    /* 0x00000 */ u32 identifier;
    /* 0x00004 */ s16 head;
    /* 0x00006 */ s16 tail;
    /* 0x00008 */ RFLiTableData data[RFLi_HDB_DATA_MAX];
    /* 0x1D4C8 */ u8 padding[22];
    /* 0x1D4DE */ u16 crc;
} RFLiHiddenDB;

typedef struct RFLiHDBList {
    /* 0x0000 */ RFLiHiddenCharData data[RFL_DB_CHAR_MAX];
    /* 0x1900 */ u8 num;
    /* 0x1901 */ u8 current;
} RFLiHDBList;

typedef struct RFLiHDBManager {
    /* 0x00 */ BOOL loaded;
    /* 0x04 */ BOOL readError;
    /* 0x08 */ void* writeTmp;
    /* 0x0C */ s16 writeIndex;
    /* 0x10 */ RFLiCallback writeCb;
    /* 0x14 */ void* formatTmp;
    /* 0x18 */ RFLiCallback formatCb;
    /* 0x1C */ s16 formatIndex;
    /* 0x20 */ RFLiHiddenCharData* loadDst;
    /* 0x24 */ void* loadTmp;
    /* 0x28 */ u32 loadArg;
    /* 0x2C */ RFLiExCallback loadCb;
    /* 0x30 */ u16 loadIndex;
    /* 0x34 */ RFLiHiddenCharData* cachedDB;
    /* 0x38 */ BOOL cached;
    /* 0x3C */ RFLiHDBList list;
} RFLiHDBManager;

/// @brief Initializes the hidden database manager.
void RFLiInitHiddenDatabase(void);

/// @brief Asynchronously loads hidden data from the database.
/// @param data The destination buffer for the data.
/// @param index The index of the data to load.
/// @param cb The completion callback.
/// @param arg The user argument for the callback.
/// @returns The result of the operation.
RFLErrcode RFLiLoadHiddenDataAsync(RFLiHiddenCharData*, u16, RFLiExCallback, u32);

/// @brief Loads hidden data from the cache.
/// @param data The destination buffer for the data.
/// @param index The index of the data to load.
/// @returns The result of the operation.
RFLErrcode RFLiLoadCachedHiddenData(RFLiHiddenCharData*, u16);

/// @brief Writes data from a controller buffer to the hidden database.
/// @param buf The controller buffer.
/// @param ch Whether to filter by hidden type.
/// @returns The result of the operation.
RFLErrcode RFLiWriteCtrlToHiddenDB(const RFLiCtrlBuf*, BOOL);

/// @brief Searches for data with a specific ID in the hidden database.
/// @param id The ID to search for.
/// @returns The index of the data, or `-1` if not found.
s32 RFLiSearchHiddenData(const RFLCreateID*);

/// @brief Counts the number of valid hidden data entries.
/// @param sex The sex to filter by.
/// @returns The number of valid entries.
u16 RFLiCountupHiddenDataNum(RFLSex);

/// @brief Gets the index of the next entry in the hidden database linked list.
/// @param index The current index.
/// @returns The next index, or `-1` if not available.
s16 RFLiGetHiddenNext(u16);

/// @brief Gets the index of the previous entry in the hidden database linked list.
/// @param index The current index.
/// @returns The previous index, or `-1` if not available.
s16 RFLiGetHiddenPrev(u16);

/// @brief Checks if the hidden data at the specified index is valid.
/// @param index The index to check.
/// @param sex The sex to filter by.
/// @returns `TRUE` if valid, `FALSE` otherwise.
BOOL RFLiIsValidHiddenData(u16, RFLSex) NO_INLINE;

/// @brief Clears the hidden database cache.
/// @param hdb The cache buffer to clear.
void RFLiClearCacheHDB(RFLiHiddenCharData*);

/// @brief Checks if the hidden database is cached.
/// @returns `TRUE` if cached, `FALSE` otherwise.
BOOL RFLiIsCachedHDB(void);

#ifdef __cplusplus
}
#endif

#endif  // RVL_FACE_LIBRARY_INTERNAL_HIDDEN_DATABASE_H
