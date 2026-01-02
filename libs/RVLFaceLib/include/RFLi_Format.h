#ifndef RVL_FACE_LIBRARY_INTERNAL_FORMAT_H
#define RVL_FACE_LIBRARY_INTERNAL_FORMAT_H
#include <RFLi_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RFLiTableData {
    /* 0x0 */ RFLCreateID createID;
    /* 0x8 */ u16 sex : 1;
    /* 0x8 */ s16 next : 15;
    /* 0xA */ s16 padding2 : 1;
    /* 0xA */ s16 prev : 15;
} RFLiTableData;

void RFLiClearTableData(RFLiTableData* data);
void RFLiClearDBBuffer(void);
RFLErrcode RFLiFormatAsync(RFLiCallback cb);

#ifdef __cplusplus
}
#endif
#endif
