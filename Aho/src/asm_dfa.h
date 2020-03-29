#ifndef AHO_ASM_DFA_H
#define AHO_ASM_DFA_H

#include "aho_corasick.h"

struct SAsmDfa
{
    uint32_t state_cnt;
    uint32_t* term_map;
    uint32_t* link_arr;
    uint32_t* edge_map[AHO_SYMB_CNT];
};

int testSAsmDfa();

int init_asm_dfa(struct SAsmDfa* asm_dfa, const struct SAhoTree* aho_tree);
int delete_asm_dfa(struct SAsmDfa* asm_dfa);

int dump_asm_dfa(const struct SAsmDfa* asm_dfa, 
                 FILE* fout);

int dump_asm_dfa_state(const struct SAsmDfa* asm_dfa, uint32_t state_idx, 
                       FILE* fout);

#endif //AHO_ASM_DFA_H
