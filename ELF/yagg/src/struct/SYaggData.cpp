#include "SYaggData.h"

void SYaggData::dump(FILE* fout, size_t tab) const noexcept
{
    assert(fout);

    fprintf(fout, YAGG_TAB("SYaggData::dump(%p)\n", tab), this);
    fprintf(fout, YAGG_TAB("{ \n", tab));

    fprintf(fout, YAGG_TAB("mask = %#hhx; \n", tab + 4u), mask);
    fprintf(fout, YAGG_TAB("rex = %#hhx; opc = %#hhx; ext = %#hhx; \n", 
                            tab + 4u), rex, opc, ext);
    fprintf(fout, YAGG_TAB("dst = %#x; src = %#x; \n", tab + 4u), dst, src);

    fprintf(fout, YAGG_TAB("} \n", tab));
}
