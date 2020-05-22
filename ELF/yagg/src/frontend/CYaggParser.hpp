#ifndef YAGG_PARSER_HPP
#define YAGG_PARSER_HPP

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cassert>

#include <string_view>
#include <algorithm>

#include "../YaggDefs.h"
#include "../SYaggInstr.h"
#include "../SYaggEntry.h"
#include "../SYaggData.h"
#include "CYaggException.hpp"

class CYaggParser
{
public:
    CYaggParser() = default;

    CYaggParser             (const CYaggParser&) = delete;
    CYaggParser& operator = (const CYaggParser&) = delete;
    CYaggParser             (CYaggParser&&) = delete;
    CYaggParser& operator = (CYaggParser&&) = delete;

    const std::string_view& text() const noexcept { return text_; }

    bool parse_instr(SYaggInstr* instr, const std::string_view& text);
    bool parse_entry(SYaggEntry* entry, const std::string_view& text);
    bool parse_data(SYaggData* data, const std::string_view& text);

protected:
    bool parse_instr_name(SYaggInstr* instr);

    bool parse_entry_dst(SYaggEntry* entry);
    bool parse_entry_src(SYaggEntry* entry);

#define YAGG_OPTION(OPT_ENUM, OPT_NAME, OPT_SIZE) \
    bool parse_opt_##OPT_NAME(SYaggData* data);

    #include "../lists/YaggOptions.h"

#undef YAGG_OPTION

    bool parse_list_begin();
    bool parse_list_end();

    size_t offset_skip() const noexcept;
    size_t offset_word() const noexcept;

    void dump(FILE* fout, size_t tab = 0u) const noexcept;

private:
    std::string_view text_;
};

int test_CYaggParser(int argc, const char* argv[]);

#endif //YAGG_PARSER_HPP
