#pragma once

class Koopa;
class JMapInfoIter;

class KoopaParts {
public:
    KoopaParts(Koopa*, const JMapInfoIter&);

    /* 0x0 */ char _0[0x78];
};
