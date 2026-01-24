#include "Game/AudioLib/CSSoundNameConverter.hpp"

#include "Game/Speaker/SpkSystem.hpp"
#include "Game/Speaker/SpkTable.hpp"
#include <JSystem/JAudio2/JASGlobal.hpp>
#include <JSystem/JGadget/hashcode.hpp>
#include <cstring>
#include <revolution/types.h>

CSSoundNameConverter::CSSoundNameConverter() : mNumEntries(0), mEntries(nullptr) {
    init();
}

s32 CSSoundNameConverter::getSoundID(const char* pName) const {
    u32 hash = JGadget::getHashCode(pName);

    for (s32 i = 0; i < mNumEntries; i++) {
        CSSoundNameConverterEntry* entry = &mEntries[i];
        if (hash == entry->mHash) {
            if (strcmp(pName, entry->mName) == 0) {
                return entry->mSoundID;
            }
        }
    }
    return -1;
}

void CSSoundNameConverter::init() {
    SpkTable* table = &JASGlobalInstance< SpkSystem >::sInstance->mData->mTable;
    initDataTable(table);
    makeDataTable(table);
}

void CSSoundNameConverter::initDataTable(const SpkTable* pTable) {
    mNumEntries = pTable->mResourceCount;
    mEntries = new CSSoundNameConverterEntry[mNumEntries];

    for (s32 i = 0; i < mNumEntries; i++) {
        mEntries[i].mName = nullptr;
        mEntries[i].mSoundID = 0;
        mEntries[i].mHash = 0;
    }
}

void CSSoundNameConverter::makeDataTable(const SpkTable* pTable) {
    for (s32 i = 0; i < pTable->mResourceCount; i++) {
        CSSoundNameConverterEntry* entry = &mEntries[i];
        entry->mSoundID = i;
        entry->mName = (const char*)pTable->mDataOffsets[i];
        entry->mHash = JGadget::getHashCode(entry->mName);
    }
}
