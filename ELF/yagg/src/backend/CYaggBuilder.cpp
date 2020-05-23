#include "CYaggBuilder.hpp"

CYaggBuilder::CYaggBuilder(FILE* file):
    yagg_file_{ file }
{}

bool CYaggBuilder::build_instr(const SYaggInstr& instr, size_t tab)
{
    fprintf(yagg_file_, YAGG_TAB("bool YB_INSTR_FNC(%s)"
            "(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)\n", tab), 
            instr.name_buf);
    fprintf(yagg_file_, YAGG_TAB("{\n", tab));

    fprintf(yagg_file_, YAGG_TAB("YB_DATA_TYPE data = {};\n", tab + 4u));

    fprintf(yagg_file_, YAGG_TAB("EYaggArgType dst_type = YB_TYPE_FNC(dst);\n", 
                                 tab + 4u));
    fprintf(yagg_file_, YAGG_TAB("EYaggArgType src_type = YB_TYPE_FNC(src);\n", 
                                 tab + 4u));

    fprintf(yagg_file_, YAGG_TAB("switch ((dst_type << 4u) | src_type)\n", 
                                 tab + 4u));
    fprintf(yagg_file_, YAGG_TAB("{\n", tab + 4u));

    for (const auto& entry : instr.entry_vec)
        build_entry(entry, "data", tab + 8u);

    fprintf(yagg_file_, YAGG_TAB("default: return false;\n", tab + 8u));
    fprintf(yagg_file_, YAGG_TAB("}// switch\n", tab + 4u));

    fprintf(yagg_file_, YAGG_TAB("YB_NEW_DATA(data);\n", tab + 4u));

    fprintf(yagg_file_, YAGG_TAB("return true;\n", tab + 4u));
    fprintf(yagg_file_, YAGG_TAB("}\n", tab));

    return true;
}

bool CYaggBuilder::build_entry(const SYaggEntry& entry, 
                               const std::string_view& name, size_t tab)
{
    fprintf(yagg_file_, YAGG_TAB("case %#hx:\n", tab), 
            (entry.dst << 4u) | entry.src);
    fprintf(yagg_file_, YAGG_TAB("{\n", tab));

    build_data(entry.data, name, tab + 4u);

    fprintf(yagg_file_, YAGG_TAB("}\n", tab));
    fprintf(yagg_file_, YAGG_TAB("break;// case\n", tab));

    return true;
}

bool CYaggBuilder::build_data(const SYaggData& data, 
                              const std::string_view& name, size_t tab)
{
    if (data.mask & SYaggData::PREF_REX)
        fprintf(yagg_file_, YAGG_TAB("YB_DATA_SET_REX(%*s, %#hhx);\n", tab),
                static_cast<int>(name.size()), name.data(), data.rex);

    if (data.mask & SYaggData::PREF_0FH)
        fprintf(yagg_file_, YAGG_TAB("YB_DATA_SET_0FH(%*s);\n", tab),
                static_cast<int>(name.size()), name.data());

    if (data.mask & SYaggData::BYTE_OPC)
        fprintf(yagg_file_, YAGG_TAB("YB_DATA_SET_OPC(%*s, %#hhx);\n", tab),
                static_cast<int>(name.size()), name.data(), data.opc);

    if (data.mask & SYaggData::SUFF_EXT)
        fprintf(yagg_file_, YAGG_TAB("YB_DATA_SET_EXT(%*s, %#hhx);\n", tab),
                static_cast<int>(name.size()), name.data(), data.ext);

    if (data.mask & SYaggData::DATA_DST)
    {
        switch (data.dst)
        {
        #define YAGG_TARGET(TGT_ENUM, TGT_NAME) \
            case YAGG_TGT_##TGT_ENUM: \
                fprintf(yagg_file_, \
                        YAGG_TAB("YB_DATA_SET_ARG(%*s, %s, dst);\n", tab), \
                        static_cast<int>(name.size()), name.data(), \
                        #TGT_NAME); \
            break;

            #include "../lists/YaggTargets.h"
            default: return false;

        #undef YAGG_TARGET
        }
    }

    if (data.mask & SYaggData::DATA_SRC)
    {
        switch (data.src)
        {
        #define YAGG_TARGET(TGT_ENUM, TGT_NAME) \
            case YAGG_TGT_##TGT_ENUM: \
                fprintf(yagg_file_, \
                        YAGG_TAB("YB_DATA_SET_ARG(%*s, %s, src);\n", tab), \
                        static_cast<int>(name.size()), name.data(), \
                        #TGT_NAME); \
            break;

            #include "../lists/YaggTargets.h"
            default: return false;

        #undef YAGG_TARGET
        }
    }

    if (data.mask & SYaggData::FLAG_JMP)
        fprintf(yagg_file_, YAGG_TAB("YB_NEW_JUMP(%*s);\n", tab), 
                static_cast<int>(name.size()), name.data());

    return true;
}

void CYaggBuilder::dump(FILE* fout, size_t tab) const noexcept
{
    fprintf(fout, YAGG_TAB("CYaggBuilder::dump(%p)\n", tab), this);
    fprintf(fout, YAGG_TAB("{\n", tab));

    fprintf(fout, YAGG_TAB("yagg_file_ = %p;\n", tab + 4u), yagg_file_);

    fprintf(fout, YAGG_TAB("}\n", tab));
}

int test_CYaggBuilder(int argc, const char* argv[])
{
    CYaggBuilder builder(stdout);
    builder.build_instr(SYaggInstr{});

    return 0;
}
