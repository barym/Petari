#include "RFL_Types.h"
#include "RFLi_NANDLoader.h"
#include "RFLi_System.h"
#include <RVLFaceLibInternal.h>
#include <revolution/nand.h>

#define LOADER_HEADER_BUF_1_SIZE 0x100
#define LOADER_HEADER_BUF_2_SIZE 0x20
#define TEMP_BUF_SIZE 0x400

static const char* scResFileFullPathName = "/shared2/FaceLib/RFL_Res.dat";

void RFLiInitLoader() {
    int i;
    RFLiLoader* loader = RFLiGetLoader();

    for (i = 0; i < RFLiArcID_Max; i++) {
        loader->archives[i].numFiles = 0;
        loader->archives[i].maxSize = 0;
        loader->archives[i].offset = 0;
    }

    loader->cacheSize = 0;
    loader->cache = NULL;
    loader->cached = FALSE;
    loader->version = 0;
    loader->headerBuf1 = NULL;
    loader->headerBuf2 = NULL;
}

static void parseOnmemoryRes_() NO_INLINE {
    int i;
    RFLiLoader* loader = RFLiGetLoader();

    loader->version = *(u16*)((u8*)loader->cache + 2);

    for (i = 0; i < RFLiArcID_Max; i++) {
        s32* headerSectionOffset;
        u16* sectionOffset;

        headerSectionOffset = loader->cache;
        headerSectionOffset += (i + 1);
        sectionOffset = (u16*)((u8*)loader->cache + *headerSectionOffset);

        loader->archives[i].numFiles = sectionOffset[0];
        loader->archives[i].maxSize = sectionOffset[1];
        loader->archives[i].offset = *headerSectionOffset + 4;
    }
}

static void loadResRead2ndcallback_() {
    RFLiLoader* loader = RFLiGetLoader();
    BOOL shouldClose = FALSE;
    RFLiCallback cb = NULL;

    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        u16 count = loader->numResources;
        s32* headerBuf1 = (s32*)loader->headerBuf1;

        if (count < RFLiArcID_Max) {
            u16* headerBuf2 = (u16*)loader->headerBuf2;
            headerBuf1 = &headerBuf1[count + 1];

            loader->archives[count].numFiles = headerBuf2[0];
            loader->archives[count].maxSize = headerBuf2[1];
            loader->archives[count].offset = *headerBuf1 + 4;
            loader->numResources++;

            headerBuf1++;
            switch (RFLiReadAsync(RFLiFileType_Resource, loader->headerBuf2, LOADER_HEADER_BUF_2_SIZE, loadResRead2ndcallback_, *headerBuf1)) {
            case RFLErrcode_Busy:
            case RFLErrcode_Success:
                break;
            default:
                shouldClose = TRUE;
            }
        } else {
            shouldClose = TRUE;
            cb = NULL;
        }
    } else {
        shouldClose = TRUE;
        RFLiSetFileBroken(RFLiFileBrokenType_ResBroken);
    }

    if (shouldClose) {
        RFLiFree(loader->headerBuf1);
        loader->headerBuf1 = NULL;

        RFLiFree(loader->headerBuf2);
        loader->headerBuf2 = NULL;

        RFLiCloseAsync(RFLiFileType_Resource, cb);
    }
}

static void errclosecallback_() {
    RFLExit();
}

static void loadResRead1stcallback_() {
    RFLiLoader* loader = RFLiGetLoader();
    BOOL shouldClose = FALSE;

    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        u32* headerBuf1 = (u32*)loader->headerBuf1;
        loader->version = *(u16*)((u8*)headerBuf1 + 2);

        loader->headerBuf2 = RFLiAlloc32(LOADER_HEADER_BUF_2_SIZE);
        loader->numResources = 0;

        headerBuf1++;
        switch (RFLiReadAsync(RFLiFileType_Resource, loader->headerBuf2, LOADER_HEADER_BUF_2_SIZE, loadResRead2ndcallback_, *headerBuf1)) {
        case RFLErrcode_Busy:
        case RFLErrcode_Success:
            break;
        default:
            shouldClose = TRUE;
        }
    } else {
        shouldClose = TRUE;
        RFLiSetFileBroken(RFLiFileBrokenType_ResBroken);
    }

    if (shouldClose) {
        RFLiFree(loader->headerBuf1);
        loader->headerBuf1 = NULL;

        if (loader->headerBuf2 != NULL) {
            RFLiFree(loader->headerBuf2);
            loader->headerBuf2 = NULL;
        }

        RFLiCloseAsync(RFLiFileType_Resource, errclosecallback_);
    }
}

static void loadResGetlengthcallback_() {
    RFLiLoader* loader = RFLiGetLoader();

    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        loader->headerBuf1 = RFLiAlloc32(LOADER_HEADER_BUF_1_SIZE);

        switch (RFLiReadAsync(RFLiFileType_Resource, loader->headerBuf1, LOADER_HEADER_BUF_1_SIZE, loadResRead1stcallback_, 0)) {
        case RFLErrcode_Busy:
        case RFLErrcode_Success:
            break;
        default:
            RFLiFree(loader->headerBuf1);
            loader->headerBuf1 = NULL;
            RFLiCloseAsync(RFLiFileType_Resource, NULL);
        }
    } else {
        RFLiCloseAsync(RFLiFileType_Resource, errclosecallback_);
    }
}

static void loadResOpencallback_() {
    if (RFLGetAsyncStatus() == RFLErrcode_Success) {
        RFLiLoader* loader = RFLiGetLoader();
        loader->cacheSize = 0;
        switch (RFLiGetLengthAsync(RFLiFileType_Resource, &loader->cacheSize, loadResGetlengthcallback_)) {
        case RFLErrcode_Busy:
        case RFLErrcode_Success:
            break;
        default:
            RFLiCloseAsync(RFLiFileType_Resource, NULL);
        }
    } else {
        RFLExit();
    }
}

RFLErrcode RFLiLoadResourceHeaderAsync() {
    RFLiLoader* loader = RFLiGetLoader();

    if (loader == NULL) {
        RFLiEndWorking(RFLErrcode_Fatal);
        return RFLErrcode_Fatal;
    }

    if (RFLIsResourceCached()) {
        parseOnmemoryRes_();
        RFLiEndWorking(RFLErrcode_Success);
        return RFLErrcode_Busy;
    }

    return RFLiOpenAsync(RFLiFileType_Resource, 1, loadResOpencallback_);
}

static u32 getCachedLength_(RFLiLoader* loader, u32 arcID, u16 fileIndex) {
    RFLiArchive* arc = &loader->archives[arcID];
    u32* pos = (u32*)((u8*)loader->cache + arc->offset);
    return pos[fileIndex + 1] - pos[fileIndex];
}

static u32 getNANDLength_(RFLiLoader* loader, u32 arcID, u16 fileIndex) {
    NANDFileInfo file;
    RFLiArchive* arc;
    void* tmpBuf;
    u32 length = 0;

    tmpBuf = RFLiAlloc32(TEMP_BUF_SIZE);
    arc = &loader->archives[arcID];

    if (NANDPrivateOpen(scResFileFullPathName, &file, 1) == NAND_RESULT_OK) {
        u32 readSize = ROUND_UP(arc->numFiles * 4 + 4, 32);
        NANDSeek(&file, arc->offset, 0);

        if (NANDRead(&file, tmpBuf, readSize) == readSize) {
            u32* pos = tmpBuf;
            length = pos[fileIndex + 1] - pos[fileIndex];
        }

        NANDClose(&file);
    }

    RFLiFree(tmpBuf);
    return length;
}

static u32 getLength_(u32 arcID, u16 fileIndex) {
    RFLiLoader* loader = RFLiGetLoader();

    if (loader == NULL) {
        return 0;
    }

    if (fileIndex >= loader->archives[arcID].numFiles) {
        return 0;
    }

    if (RFLIsResourceCached()) {
        return getCachedLength_(loader, arcID, fileIndex);
    }

    return getNANDLength_(loader, arcID, fileIndex);
}

static void* getCachedFile_(void* dst, RFLiLoader* loader, u32 arcID, u16 fileIndex) {
    u8* cache = (u8*)loader->cache;
    RFLiArchive* arc = &loader->archives[arcID];
    u32* pos = (u32*)(cache + arc->offset);
    s32 globalOffset = (arc->offset + (arc->numFiles * sizeof(u32)) + 4) + pos[fileIndex];
    u32 fileSize = pos[fileIndex + 1] - pos[fileIndex];

    memcpy(dst, cache + globalOffset, fileSize);
    return dst;
}

static void* getNANDFile_(void* dst, RFLiLoader* loader, u32 arcID, u16 fileIdx) {
    RFLiArchive* arc;
    NANDFileInfo file;

    u8* tmpBuf;
    u8* arcBuf;
    void* ret = NULL;

    arc = &loader->archives[arcID];
    tmpBuf = RFLiAlloc32(TEMP_BUF_SIZE);

    if (NANDPrivateOpen(scResFileFullPathName, &file, 1) == NAND_RESULT_OK) {
        u32 readSize, offsetSize;
        s32 globalOffset;
        u32 arcSize;
        u32 arcOffset;

        readSize = ROUND_UP(arc->numFiles * 4 + 4, 32);
        arcSize = 0;
        arcOffset = 0;

        NANDSeek(&file, arc->offset, 0);

        if (NANDRead(&file, tmpBuf, readSize) == readSize) {
            u32* pos = (u32*)tmpBuf;
            arcSize = pos[fileIdx + 1] - pos[fileIdx];
            arcOffset = pos[fileIdx];
        } else {
            RFLiSetFileBroken(RFLiFileBrokenType_ResBroken);
            RFLiFree(tmpBuf);
            return NULL;
        }

        globalOffset = (arc->offset + (arc->numFiles * 4)) + 4 + arcOffset;
        offsetSize = ROUND_UP(arcSize, 32);

        arcBuf = RFLiAlloc32(offsetSize);
        NANDSeek(&file, globalOffset, 0);

        if (NANDRead(&file, arcBuf, offsetSize) == offsetSize) {
            memcpy(dst, arcBuf, arcSize);
            ret = dst;
        } else {
            RFLiSetFileBroken(RFLiFileBrokenType_ResBroken);
            RFLiFree(tmpBuf);
            RFLiFree(arcBuf);
            return NULL;
        }

        RFLiFree(arcBuf);
        NANDClose(&file);
    } else {
        RFLiSetFileBroken(RFLiFileBrokenType_ResBroken);
    }

    RFLiFree(tmpBuf);
    return ret;
}

static void* getFile_(void* dst, u32 arcID, u16 fileIndex) {
    RFLiLoader* loader;

    if (!RFLAvailable()) {
        return NULL;
    }

    loader = RFLiGetLoader();
    if (loader == NULL) {
        return NULL;
    }

    if (fileIndex >= loader->archives[arcID].numFiles) {
        return NULL;
    }

    if (RFLIsResourceCached()) {
        return getCachedFile_(dst, loader, arcID, fileIndex);
    } else {
        return getNANDFile_(dst, loader, arcID, fileIndex);
    }
}

u32 RFLiGetTexSize(RFLiPartsTex part, u16 fileIndex) {
    static const u32 scParts2Arc[] = {RFLiArcID_Eye, RFLiArcID_Eyebrow, RFLiArcID_Mouth, RFLiArcID_Mustache, RFLiArcID_Mole};
    return getLength_(scParts2Arc[part], fileIndex);
}

RFLiTexture* RFLiLoadTexture(RFLiPartsTex part, u16 fileIndex, void* dst) {
    static const u32 scParts2Arc[] = {RFLiArcID_Eye, RFLiArcID_Eyebrow, RFLiArcID_Mouth, RFLiArcID_Mustache, RFLiArcID_Mole};
    return (RFLiTexture*)getFile_(dst, scParts2Arc[part], fileIndex);
}

u32 RFLiGetShpTexSize(RFLiPartsShpTex part, u16 fileIndex) {
    static const u32 scParts2Arc[] = {RFLiArcID_FaceTex, RFLiArcID_CapTex, RFLiArcID_NlineTex, RFLiArcID_GlassTex};
    return getLength_(scParts2Arc[part], fileIndex);
}

RFLiTexture* RFLiLoadShpTexture(RFLiPartsShpTex part, u16 fileIndex, void* dst) {
    static const u32 scParts2Arc[] = {RFLiArcID_FaceTex, RFLiArcID_CapTex, RFLiArcID_NlineTex, RFLiArcID_GlassTex};
    return (RFLiTexture*)getFile_(dst, scParts2Arc[part], fileIndex);
}

u32 RFLiGetShapeSize(RFLiPartsShp part, u16 fileIndex) {
    static const u32 scParts2Arc[] = {RFLiArcID_Nose,  RFLiArcID_ForeHead, RFLiArcID_Faceline, RFLiArcID_Hair, RFLiArcID_Cap,
                                      RFLiArcID_Beard, RFLiArcID_Nline,    RFLiArcID_Mask,     RFLiArcID_Glass};
    return getLength_(scParts2Arc[part], fileIndex);
}

void* RFLiLoadShape(RFLiPartsShp part, u16 fileIndex, void* dst) {
    static const u32 scParts2Arc[] = {RFLiArcID_Nose,  RFLiArcID_ForeHead, RFLiArcID_Faceline, RFLiArcID_Hair, RFLiArcID_Cap,
                                      RFLiArcID_Beard, RFLiArcID_Nline,    RFLiArcID_Mask,     RFLiArcID_Glass};
    return getFile_(dst, scParts2Arc[part], fileIndex);
}

RFLErrcode RFLFreeCachedResource() {
    RFLiLoader* loader;

    if (!RFLAvailable()) {
        return RFLErrcode_NotAvailable;
    }

    if (RFLiIsWorking()) {
        return RFLErrcode_NotAvailable;
    }

    loader = RFLiGetLoader();
    if (loader == NULL) {
        return RFLErrcode_NotAvailable;
    }

    loader->cached = FALSE;
    loader->cache = NULL;
    loader->cacheSize = 0;
    return RFLErrcode_Success;
}

BOOL RFLIsResourceCached() {
    RFLiLoader* loader;

    if (!RFLAvailable()) {
        return FALSE;
    }

    loader = RFLiGetLoader();
    if (loader == NULL) {
        return FALSE;
    }

    return loader->cached;
}
