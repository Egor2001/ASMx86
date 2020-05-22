#ifndef YAGG_DATA_H
#define YAGG_DATA_H

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cassert>

#include "YaggDefs.h"
#include "enums/EYaggTarget.h"

struct SYaggData
{
public:
    enum EType
    {
        PREF_REX = 0x01, 
        PREF_0FH = 0x02,
        BYTE_OPC = 0x04,
        SUFF_EXT = 0x08,
        DATA_DST = 0x10,
        DATA_SRC = 0x20,
        FLAG_JMP = 0x40,
    };

    void dump(FILE* fout, size_t tab = 0u) const noexcept;

    uint8_t mask;
    uint8_t rex, opc, ext;
    EYaggTarget dst, src;
};

#endif //YAGG_DATA_H
