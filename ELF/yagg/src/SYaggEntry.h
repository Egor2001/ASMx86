#ifndef YAGG_ENTRY_H
#define YAGG_ENTRY_H

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cassert>

#include "YaggDefs.h"
#include "enums/EYaggArgType.h"
#include "SYaggData.h"

struct SYaggEntry
{
public:
    void dump(FILE* fout, size_t tab = 0u) const noexcept;

    EYaggArgType dst, src;
    SYaggData data;
};

#endif //YAGG_ENTRY_H
