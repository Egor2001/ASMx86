#include "aho_corasick.h"
#include "asm_dfa.h"

int print_dfa_db_nasm(const struct SAsmDfa* asm_dfa, FILE* fout);

int main(int argc, char** argv)
{
    //testSAhoTree();
    //testSAsmDfa();

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

    struct SAhoTree aho_tree = {};
    aho_init_pref_tree(&aho_tree, str_arr, str_cnt);
    aho_init_tree_link(&aho_tree);

    struct SAsmDfa asm_dfa = {};
    init_asm_dfa(&asm_dfa, &aho_tree);

    print_dfa_db_nasm(&asm_dfa, fout);

    delete_asm_dfa(&asm_dfa);
    aho_delete_tree(&aho_tree);

    fclose(fout);

    return EXIT_SUCCESS;
}

int print_dfa_db_nasm(const struct SAsmDfa* asm_dfa, FILE* fout)
{
    fprintf(fout, "\t\t;DFA automaton for string detection\n");

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
