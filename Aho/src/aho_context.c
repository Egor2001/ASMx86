#include "aho_context.h"

int testSAhoContext()
{
    const char* str_arr[] = { "abc", "bab", "cab", "babca" };
    uint32_t str_cnt = sizeof(str_arr)/sizeof(const char*);

    struct SAhoContext context = {};
    aho_init_context(&context, str_arr, str_cnt);
    aho_dump_context(&context, stdout);
    aho_delete_context(&context);

    return 0;
}

int aho_init_context(struct SAhoContext* context,
                     const char** str_arr, uint32_t str_cnt)
{
    assert(context);
    assert(str_arr);
    
    context->str_cnt = str_cnt;
    context->str_arr = 
        (const char**) (calloc(context->str_cnt, sizeof(const char*)));

    memcpy(context->str_arr, str_arr, 
           context->str_cnt*sizeof(const char*));

    return 0;
}

int aho_delete_context(struct SAhoContext* context)
{
    assert(context);

    free(context->str_arr);

    context->str_cnt = 0u;
    context->str_arr = NULL;

    return 0;
}

int aho_dump_context(const struct SAhoContext* context, FILE* fout)
{
    assert(context);
    assert(fout);

    fprintf(fout, 
            "aho_dump_context():\n"
            "{\n"
            "    str_cnt = %u;\n"
            "    str_arr[] = %p;\n"
            "}\n",
            context->str_cnt,
            context->str_arr);

    fprintf(fout, "{\n");
    for (uint32_t str_idx = 0u; str_idx != context->str_cnt; ++str_idx)
    {
        fprintf(fout, "    [%u] = '%s'\n", 
                str_idx, context->str_arr[str_idx]);
    }
    fprintf(fout, "}\n");

    return 0;
}
