#ifndef RVL_FACE_LIBRARY_NAND_LOADER_H
#define RVL_FACE_LIBRARY_NAND_LOADER_H

#include <RFL_Types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Free the cached resource data.
/// @return An error code indicating the result of the operation.
RFLErrcode RFLFreeCachedResource();

/// @brief Check if the resource data is cached.
/// @return `TRUE` if the resource data is cached, `FALSE` otherwise.
BOOL RFLIsResourceCached();

#ifdef __cplusplus
}
#endif

#endif  // RVL_FACE_LIBRARY_NAND_LOADER_H
