#ifndef YAGG_BUILDER_HPP
#define YAGG_BUILDER_HPP

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cassert>

#include <string_view>

#include "../YaggDefs.h"
#include "../SYaggInstr.h"
#include "../SYaggEntry.h"
#include "../SYaggData.h"

class CYaggBuilder
{
public:
    explicit CYaggBuilder(FILE* file);

    CYaggBuilder             (const CYaggBuilder&) = delete;
    CYaggBuilder& operator = (const CYaggBuilder&) = delete;
    CYaggBuilder             (CYaggBuilder&&) = delete;
    CYaggBuilder& operator = (CYaggBuilder&&) = delete;

    bool build_instr(const SYaggInstr& instr, size_t tab = 0u);
    bool build_entry(const SYaggEntry& entry, 
                     const std::string_view& name, size_t tab = 0u);
    bool build_data(const SYaggData& data, 
                     const std::string_view& name, size_t tab = 0u);

    void dump(FILE* fout, size_t tab = 0u) const noexcept;

private:
    FILE* yagg_file_;
};

int test_CYaggBuilder(int argc, const char* argv[]);

#endif //YAGG_BUILDER_HPP
