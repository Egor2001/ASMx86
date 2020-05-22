#include "SYaggEntry.h"

void SYaggEntry::dump(FILE* fout, size_t tab) const noexcept
{
    fprintf(fout, YAGG_TAB("SYaggEntry::dump(%p)\n", tab), this);
    fprintf(fout, YAGG_TAB("{\n", tab));

    fprintf(fout, YAGG_TAB("dst = %#hhx;\n", tab + 4u), dst);
    fprintf(fout, YAGG_TAB("src = %#hhx;\n", tab + 4u), src);

    data.dump(fout, tab + 4u);
    
    fprintf(fout, YAGG_TAB("}\n", tab));
}
