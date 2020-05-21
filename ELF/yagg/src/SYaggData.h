#ifndef YAGG_DATA_H
#define YAGG_DATA_H

#include <cstdlib>
#include <cstdio>

#include "YaggDefs.h"
#include "EYaggTarget.h"

struct SYaggData
{
public:
    enum EType
    {
        PREF_REX = 0x1, 
        PREF_0FH = 0x2,
        FLAG_JMP = 0x4,
    };

    void dump(FILE* fout, size_t tab = 0u) const noexcept;

    uint8_t mask;
    uint8_t rex, opc;
    EYaggTarget dst, src;
};

void SYaggData::dump(FILE* fout, size_t tab) const noexcept
{
    assert(fout);

    fprintf(fout, YAGG_TAB("SYaggData::dump(%p)\n", tab), this);
    fprintf(fout, YAGG_TAB("{ \n", tab));
    fprintf(fout, YAGG_TAB("mask = %#hhx; \n", tab + 4u), mask);
    fprintf(fout, YAGG_TAB("rex = %#hhx; opc = %#hhx; \n", tab + 4u), rex, opc);
    fprintf(fout, YAGG_TAB("dst = %#x; src = %#x; \n", tab + 4u), dst, src);
    fprintf(fout, YAGG_TAB("} \n", tab));
}

#endif //YAGG_DATA_H
