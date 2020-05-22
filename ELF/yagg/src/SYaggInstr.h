#ifndef YAGG_INSTR_H
#define YAGG_INSTR_H

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cassert>

#include <vector>

#include "YaggDefs.h"
#include "SYaggEntry.h"

struct SYaggInstr
{
public:
    constexpr static size_t NAME_BUF_LEN = 16u;

    void dump(FILE* fout, size_t tab = 0u) const noexcept;

    char name_buf[NAME_BUF_LEN];
    std::vector<SYaggEntry> entry_vec;
};

#endif //YAGG_INSTR_H
