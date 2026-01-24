#pragma once

#include "revolution/types.h"

class SpkTable;

class CSSoundNameConverterEntry {
public:
    /* 0x0 */ const char* mName;
    /* 0x4 */ u32 mSoundID;
    /* 0x8 */ u32 mHash;
};

class CSSoundNameConverter {
public:
    CSSoundNameConverter();
    void init();
    s32 getSoundID(const char*) const;
    void initDataTable(const SpkTable*);
    void makeDataTable(const SpkTable*);

private:
    /* 0x0 */ s32 mNumEntries;
    /* 0x4 */ CSSoundNameConverterEntry* mEntries;
};
