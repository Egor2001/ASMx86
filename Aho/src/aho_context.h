#ifndef SRC_AHO_CONTEXT_H
#define SRC_AHO_CONTEXT_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

struct SAhoContext
{
    uint32_t str_cnt;
    const char** str_arr;
};

int testSAhoContext();

int aho_init_context(struct SAhoContext* context,
                     const char* str_arr[], uint32_t str_cnt);

int aho_delete_context(struct SAhoContext* context);

int aho_dump_context(const struct SAhoContext* context, FILE* fout);

#endif //SRC_AHO_CONTEXT_H
