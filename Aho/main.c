#include "aho_context.h"
#include "aho_corasick.h"
#include "asm_dfa.h"

int print_context_db_nasm(const struct SAhoContext* context, FILE* fout);
int print_dfa_db_nasm(const struct SAsmDfa* asm_dfa, FILE* fout);

int test_print(int argc, char** argv);
int test_algo(int argc, char** argv);

int main(int argc, char** argv)
{
    test_print(argc, argv);

    //test_algo(argc, argv);
    //testSAhoContext();
    //testSAhoTree();
    //testSAsmDfa();

    return EXIT_SUCCESS;
}

int print_context_db_nasm(const struct SAhoContext* context, FILE* fout)
{
    assert(context);
    assert(fout);

    fprintf(fout, ";Context for string detection\n");

    //print str_cnt
    fprintf(fout, "DdAhoStrCnt:\n");
    fprintf(fout, "\t\tdd %u\n", context->str_cnt);

    for (uint32_t str_idx = 0u; str_idx != context->str_cnt; ++str_idx)
    {
        fprintf(fout, "DbAhoStr%u:\tdb '%s ',0xA,0\n", 
                str_idx, context->str_arr[str_idx]);
    }

    //print str_arr
    fprintf(fout, "DqAhoStrArr:\n");
    fprintf(fout, "\t\tdq ");
    for (uint32_t str_idx = 0u; str_idx != context->str_cnt; ++str_idx)
    {
        fprintf(fout, "DbAhoStr%u, ", str_idx);
    }
    fprintf(fout, "\n");

    //print str_len_arr
    fprintf(fout, "DdAhoLenArr:\n");
    fprintf(fout, "\t\tdd ");
    for (uint32_t str_idx = 0u; str_idx != context->str_cnt; ++str_idx)
    {
        fprintf(fout, "%lu, ", strlen(context->str_arr[str_idx]) + 1ul);
    }
    fprintf(fout, "\n");

    return 0;
}

int print_dfa_db_nasm(const struct SAsmDfa* asm_dfa, FILE* fout)
{
    assert(asm_dfa);
    assert(fout);

    fprintf(fout, ";DFA automaton for string detection\n");

    //print state_cnt
    fprintf(fout, "DdDfaSize:\n");
    fprintf(fout, "\t\tdd %u\n", asm_dfa->state_cnt);

    //print term_map
    fprintf(fout, "DdDfaTermMap:\n");
    fprintf(fout, "\t\tdd ");
    for (uint32_t state_idx = 0u; 
         state_idx != asm_dfa->state_cnt; ++state_idx)
    {
        fprintf(fout, "%u,", asm_dfa->term_map[state_idx]);
    }
    fprintf(fout, "\n");

    //print link_arr
    fprintf(fout, "DdDfaLinkArr:\n");
    fprintf(fout, "\t\tdd ");
    for (uint32_t state_idx = 0u; 
         state_idx != asm_dfa->state_cnt; ++state_idx)
    {
        fprintf(fout, "%u,", asm_dfa->link_arr[state_idx]);
    }
    fprintf(fout, "\n");

    //print edge_map
    fprintf(fout, "DdDfaEdgeMap:\n");
    for (uint32_t symb_idx = 0u; symb_idx != AHO_SYMB_CNT; ++symb_idx)
    {
        fprintf(fout, "\t\tdd ");
        for (uint32_t state_idx = 0u; 
             state_idx != asm_dfa->state_cnt; ++state_idx)
        {
            fprintf(fout, "%u,", asm_dfa->edge_map[symb_idx][state_idx]);
        }
        fprintf(fout, "\n");
    }

    return 0;
}

int test_print(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("invalid parameters number\n");
        printf("USAGE: %s FILENAME\n"
               "FILENAME - name of file will be generated\n", 
               argv[0]);

        return EXIT_FAILURE;
    }

    FILE* fout = fopen(argv[1], "w");

    if (!fout)
    {
        perror("error opening file: ");
        printf("\n");

        return EXIT_FAILURE;
    }

    const char* str_arr[] = { "abc", "bab", "cab", "babca" };
    const uint32_t str_cnt = sizeof(str_arr)/sizeof(str_arr[0]);

    struct SAhoContext context = {};
    aho_init_context(&context, str_arr, str_cnt);

    struct SAhoTree aho_tree = {};
    aho_init_pref_tree(&aho_tree, str_arr, str_cnt);
    aho_init_tree_link(&aho_tree);

    struct SAsmDfa asm_dfa = {};
    init_asm_dfa(&asm_dfa, &aho_tree);

    print_context_db_nasm(&context, fout);
    print_dfa_db_nasm(&asm_dfa, fout);

    delete_asm_dfa(&asm_dfa);
    aho_delete_tree(&aho_tree);
    aho_delete_context(&context);

    fclose(fout);
    fout = NULL;

    return EXIT_SUCCESS;
}

int test_algo(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("invalid parameters number\n");
        printf("USAGE: %s STR\n"
               "STR - string to be processed\n", 
               argv[0]);

        return EXIT_FAILURE;
    }

    const char* str_arr[] = { "abc", "bab", "cab", "babca" };
    const uint32_t str_cnt = sizeof(str_arr)/sizeof(str_arr[0]);

    struct SAhoTree aho_tree = {};
    aho_init_pref_tree(&aho_tree, str_arr, str_cnt);
    aho_init_tree_link(&aho_tree);

    struct SAsmDfa asm_dfa = {};
    init_asm_dfa(&asm_dfa, &aho_tree);

    const char* str = argv[1];

    uint32_t state_idx = 0u;
    for (const char* it = str; *it; ++it)
    {
        state_idx = asm_dfa.edge_map[(uint8_t) *it][state_idx];

        uint32_t link_idx = state_idx;
        while (link_idx)
        {
            if (asm_dfa.term_map[link_idx])
            {
                printf("detected str[%u]\n", (uint32_t) (it - str));
            }

            link_idx = asm_dfa.link_arr[link_idx];
        }

    }

    return EXIT_SUCCESS;
}
