#include "aho_corasick.h"

int testSAhoTree()
{
    const char* str_arr[] = { "abc", "bab", "cba", "babcba" };
    const uint32_t str_cnt = sizeof(str_arr)/sizeof(str_arr[0]);

    struct SAhoTree aho_tree = {};

    aho_init_pref_tree(&aho_tree, str_arr, str_cnt);
    aho_init_tree_link(&aho_tree);
    aho_dump_tree(&aho_tree, stdout);
    aho_delete_tree(&aho_tree);

    return 0;
}

int aho_init_pref_tree(struct SAhoTree* aho_tree, 
                       const char* str_arr[], uint32_t str_cnt)
{
    //init ::state_cnt
    aho_tree->state_cnt = 1u;

    //init ::state_capacity
    aho_tree->state_capacity = 1u;
    for (uint32_t str_idx = 0u; str_idx != str_cnt; ++str_idx)
    {
        aho_tree->state_capacity += 
            strlen(str_arr[str_idx]);
    }

    //init ::symb_arr
    aho_tree->symb_arr = 
        (uint8_t*) calloc(aho_tree->state_capacity, sizeof(uint8_t));

    //init ::dict_arr
    aho_tree->dict_arr = 
        (uint32_t*) calloc(aho_tree->state_capacity, sizeof(uint32_t));

    //init ::symb_arr
    aho_tree->suff_arr = 
        (uint32_t*) calloc(aho_tree->state_capacity, sizeof(uint32_t));

    //init ::symb_arr
    aho_tree->link_arr = 
        (uint32_t*) calloc(aho_tree->state_capacity, sizeof(uint32_t));

    //init ::edge_map
    for (uint32_t symb_idx = 0u; symb_idx != AHO_SYMB_CNT; ++symb_idx)
    {
        aho_tree->edge_map[symb_idx] = 
            (uint32_t*) calloc(aho_tree->state_capacity, sizeof(uint32_t));
    }

    //insertion
    for (uint32_t str_idx = 0u; str_idx != str_cnt; ++str_idx)
    {
        aho_insert_new_str(aho_tree, str_arr[str_idx]);
    }

    return 0;
}

int aho_insert_new_str(struct SAhoTree* aho_tree, const char* str)
{
    assert(aho_tree);
    assert(str);

    //walking down the tree
    uint32_t cur_state = 0u;
    const char* it = str;
    while (*it && aho_tree->edge_map[*it][cur_state])
    {
        cur_state = aho_tree->edge_map[*it][cur_state];
        ++it;
    }

    //extending path in the tree
    while (*it)
    {
        aho_tree->edge_map[*it][cur_state] = aho_tree->state_cnt;
        cur_state = aho_tree->state_cnt;
        ++aho_tree->state_cnt;

        aho_tree->symb_arr[cur_state] = *(it);
        ++it;
    }

    //saving new state
    ++aho_tree->dict_arr[cur_state];

    return 0;
}

int aho_init_tree_link(struct SAhoTree* aho_tree)
{
    assert(aho_tree);

    //dfs used mask, dfs stack and next dfs symbol index arrays
    uint8_t* used_arr = 
        (uint8_t*) calloc(aho_tree->state_cnt, sizeof(uint8_t));

    uint32_t* dfs_arr =
        (uint32_t*) calloc(aho_tree->state_cnt, sizeof(uint32_t));

    uint32_t* idx_arr =
        (uint32_t*) calloc(aho_tree->state_cnt, sizeof(uint32_t));

    //safe dfs implementation
    uint32_t dfs_cur_size = 1u;
    dfs_arr[0u] = 0u;
    while (dfs_cur_size)
    {
        uint32_t cur_idx = dfs_arr[dfs_cur_size - 1u];

        //skip used and zero edges
        while (idx_arr[cur_idx] != AHO_SYMB_CNT && 
               (aho_tree->edge_map[idx_arr[cur_idx]][cur_idx] == 0u ||
                used_arr[aho_tree->edge_map[idx_arr[cur_idx]][cur_idx]]))
        {
            ++idx_arr[cur_idx];
        }

        //push next vertex or pop current one
        if (idx_arr[cur_idx] != AHO_SYMB_CNT)
        {
            //find next suffix link
            uint32_t symb_idx = idx_arr[cur_idx];
            uint32_t next = aho_tree->edge_map[symb_idx][cur_idx];

            uint32_t cur_prev = aho_tree->suff_arr[cur_idx];
            while (cur_prev && aho_tree->edge_map[symb_idx][cur_prev] == 0u)
                cur_prev = aho_tree->suff_arr[cur_prev];

            if (cur_idx)
            {
                aho_tree->suff_arr[next] = 
                    aho_tree->edge_map[symb_idx][cur_prev];
            }
            else
            {
                aho_tree->suff_arr[next] = 0u;
            }

            //find next dictionary link
            if (aho_tree->dict_arr[aho_tree->suff_arr[next]])
            {
                aho_tree->link_arr[next] = 
                    aho_tree->suff_arr[next];
            }
            else
            {
                aho_tree->link_arr[next] = 
                    aho_tree->link_arr[aho_tree->suff_arr[next]];
            }

            //adjust dfs stack and used values
            dfs_arr[dfs_cur_size] = next;
            ++dfs_cur_size;

            used_arr[next] = 1u;
            ++idx_arr[cur_idx];
        }
        else
        {
            --dfs_cur_size;
        }
    }

    //cleanup after dfs
    free(idx_arr); idx_arr = NULL;
    free(dfs_arr); dfs_arr = NULL;
    free(used_arr); used_arr = NULL;

    return 0;
}

int aho_delete_tree(struct SAhoTree* aho_tree)
{
    assert(aho_tree);

    //freeing arrays
    free(aho_tree->symb_arr);
    free(aho_tree->dict_arr);

    free(aho_tree->suff_arr);
    free(aho_tree->link_arr);

    for (uint32_t symb_idx = 0u; symb_idx < AHO_SYMB_CNT; ++symb_idx)
        free(aho_tree->edge_map[symb_idx]);

    //zeroing variables
    aho_tree->state_cnt = 0u;
    aho_tree->state_capacity = 0u;
    aho_tree->symb_arr = NULL;
    aho_tree->dict_arr = NULL;
    aho_tree->suff_arr = NULL;
    aho_tree->link_arr = NULL;

    for (uint32_t symb_idx = 0u; symb_idx < AHO_SYMB_CNT; ++symb_idx)
        aho_tree->edge_map[symb_idx] = NULL;

    return 0;
}

int aho_dump_tree(struct SAhoTree* aho_tree, 
                  FILE* fout)
{
    assert(aho_tree);
    assert(fout);

    //dump variables
    fprintf(fout, 
            "aho_dump_pref_tree():\n"
            "{\n"
            "    state_cnt = %u;\n"
            "    state_capacity = %u;\n"
            "    symb_arr[] = %p;\n"
            "    dict_arr[] = %p;\n"
            "    edge_map[][] = %p;\n"
            "}\n", 
            aho_tree->state_cnt, 
            aho_tree->state_capacity, 
            aho_tree->symb_arr, 
            aho_tree->dict_arr, 
            aho_tree->edge_map);

    //dump arrays
    fprintf(fout, "{\n");
    for (uint32_t state_idx = 0u; 
         state_idx != aho_tree->state_cnt; ++state_idx)
    {
        fprintf(fout, "    ");
        aho_dump_tree_state(aho_tree, state_idx, fout);
    }
    fprintf(fout, "}\n");

    return 0;
}

int aho_dump_tree_state(struct SAhoTree* aho_tree, uint32_t state_idx, 
                        FILE* fout)
{
    assert(aho_tree);
    assert(fout);

    fprintf(fout, "[%u] = { \'%c\', #%u, SUFF->%u, LINK->%u, ", 
            state_idx, 
            aho_tree->symb_arr[state_idx],
            aho_tree->dict_arr[state_idx],
            aho_tree->suff_arr[state_idx],
            aho_tree->link_arr[state_idx]);

    for (uint32_t symb_idx = 0u; symb_idx != AHO_SYMB_CNT; ++symb_idx)
    {
        if (aho_tree->edge_map[symb_idx][state_idx])
        {
            fprintf(fout, "[\'%c\'] -> %u ", 
                    (uint8_t) symb_idx,
                    aho_tree->edge_map[symb_idx][state_idx]);
        }
    }

    fprintf(fout, "}\n");

    return 0;
}
