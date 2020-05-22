#include "SYaggInstr.h"

void SYaggInstr::dump(FILE* fout, size_t tab) const noexcept
{
    fprintf(fout, YAGG_TAB("SYaggInstr::dump(%p)\n", tab), this);
    fprintf(fout, YAGG_TAB("{\n", tab));

    fprintf(fout, YAGG_TAB("name_buf = \"%.*s\";\n", tab + 4u), 
            static_cast<int>(NAME_BUF_LEN), name_buf);

    fprintf(fout, YAGG_TAB("entry_vec = {\n", tab + 4u));
    for (const auto& entry : entry_vec)
        entry.dump(fout, tab + 8u);
    fprintf(fout, YAGG_TAB("}// entry_vec\n", tab + 4u));

    fprintf(fout, YAGG_TAB("}\n", tab));
}
