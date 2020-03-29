#include "aho_corasick.h"
#include "asm_dfa.h"

int testSAsmDfa()
{
    const char* str_arr[] = { "abc", "bab", "cab", "babca" };
    const uint32_t str_cnt = sizeof(str_arr)/sizeof(str_arr[0]);

    struct SAhoTree aho_tree = {};
    aho_init_pref_tree(&aho_tree, str_arr, str_cnt);
    aho_init_tree_link(&aho_tree);

    struct SAsmDfa asm_dfa = {};
    init_asm_dfa(&asm_dfa, &aho_tree);

    aho_dump_tree(&aho_tree, stdout);
    dump_asm_dfa(&asm_dfa, stdout);

    delete_asm_dfa(&asm_dfa);
    aho_delete_tree(&aho_tree);

    return 0;
}

int init_asm_dfa(struct SAsmDfa* asm_dfa, const struct SAhoTree* aho_tree)
{
    assert(aho_tree);
    assert(asm_dfa);

    //init ::state_cnt
    asm_dfa->state_cnt = aho_tree->state_cnt;

    //alloc ::term_map
    asm_dfa->term_map = 
        (uint32_t*) calloc(asm_dfa->state_cnt, sizeof(uint32_t));

    //alloc ::link_arr
    asm_dfa->link_arr = 
        (uint32_t*) calloc(asm_dfa->state_cnt, sizeof(uint32_t));

    //alloc ::edge_map
    for (uint32_t symb_idx = 0u; symb_idx != AHO_SYMB_CNT; ++symb_idx)
    {
        asm_dfa->edge_map[symb_idx] = 
            (uint32_t*) calloc(asm_dfa->state_cnt, sizeof(uint32_t));
    }

    //init ::term_map
    memcpy(asm_dfa->term_map, 
           aho_tree->dict_arr, 
           asm_dfa->state_cnt*sizeof(uint32_t));

    //init ::link_arr
    memcpy(asm_dfa->link_arr, 
           aho_tree->link_arr, 
           asm_dfa->state_cnt*sizeof(uint32_t));

    //init ::edge_map
    for (uint32_t symb_idx = 0u; symb_idx != AHO_SYMB_CNT; ++symb_idx)
    {
        memcpy(asm_dfa->edge_map[symb_idx], 
               aho_tree->edge_map[symb_idx], 
               asm_dfa->state_cnt*sizeof(uint32_t));
    }

    //bfs ring buffer init
    uint32_t bfs_rb_head = 0u;
    uint32_t bfs_rb_tail = 0u;
    uint32_t* bfs_ringbuf = 
        (uint32_t*) calloc(2u*asm_dfa->state_cnt, sizeof(uint32_t));

    //bfs fill automaton edges via suffix array
    bfs_ringbuf[bfs_rb_tail++] = 0u;
    while (bfs_rb_head != bfs_rb_tail)
    {
        uint32_t cur_idx = bfs_ringbuf[bfs_rb_head++];
        for (uint32_t symb_idx = 0u; symb_idx != AHO_SYMB_CNT; ++symb_idx)
        {
            if (aho_tree->edge_map[symb_idx][cur_idx])
            {
                bfs_ringbuf[bfs_rb_tail++] = 
                    aho_tree->edge_map[symb_idx][cur_idx];
            }
            else
            {
                asm_dfa->edge_map[symb_idx][cur_idx] = 
                    asm_dfa->edge_map[symb_idx][aho_tree->suff_arr[cur_idx]];
            }
        }
    }

    return 0;
}

int delete_asm_dfa(struct SAsmDfa* asm_dfa)
{
    assert(asm_dfa);

    //freeing arrays
    free(asm_dfa->term_map);
    free(asm_dfa->link_arr);
    for (uint32_t symb_idx = 0u; symb_idx != AHO_SYMB_CNT; ++symb_idx)
        free(asm_dfa->edge_map[symb_idx]);

    //zeroing values
    asm_dfa->state_cnt = 0u;
    asm_dfa->term_map = NULL;
    asm_dfa->link_arr = NULL;
    for (uint32_t symb_idx = 0u; symb_idx != AHO_SYMB_CNT; ++symb_idx)
        asm_dfa->edge_map[symb_idx] = NULL;

    return 0;
}

int dump_asm_dfa(const struct SAsmDfa* asm_dfa, 
                 FILE* fout)
{
    assert(asm_dfa);
    assert(fout);

    fprintf(fout, 
            "dump_asm_dfa():\n"
            "{\n"
            "    state_cnt = %u;\n"
            "    link_arr[] = %p;\n"
            "    edge_map[][] = %p;\n"
            "}\n",
            asm_dfa->state_cnt, 
            asm_dfa->link_arr, 
            asm_dfa->edge_map);

    fprintf(fout, "{\n");
    for (uint32_t state_idx = 0u;
         state_idx != asm_dfa->state_cnt; ++state_idx)
    {
        fprintf(fout, "    ");
        dump_asm_dfa_state(asm_dfa, state_idx, fout);
    }
    fprintf(fout, "}\n");

    return 0;
}

int dump_asm_dfa_state(const struct SAsmDfa* asm_dfa, uint32_t state_idx, 
                       FILE* fout)
{
    assert(asm_dfa);
    assert(fout);

    fprintf(fout, "[%u] = { T(%u), L(%u), ", 
            state_idx, 
            asm_dfa->term_map[state_idx],
            asm_dfa->link_arr[state_idx]);
    for (uint32_t symb_idx = 0u; symb_idx != AHO_SYMB_CNT; ++symb_idx)
    {
        if (asm_dfa->edge_map[symb_idx][state_idx])
        {
            fprintf(fout, "[%c] -> %u ", 
                    (char) symb_idx, 
                    asm_dfa->edge_map[symb_idx][state_idx]);
        }
    }
    fprintf(fout, "}\n");

    return 0;
}
