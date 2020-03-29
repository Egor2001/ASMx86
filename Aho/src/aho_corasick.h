#ifndef AHO_AHO_CORASICK_H
#define AHO_AHO_CORASICK_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

//uncomment to debug
//#define NDEBUG

#define AHO_SYMB_CNT 256

//tree, used to build dfa
struct SAhoTree
{
    uint32_t state_cnt;                 //state count
    uint32_t state_capacity;            //state array capacity

    uint8_t* symb_arr;                  //vertex symbol array
    uint32_t* dict_arr;                 //dictionary string mask

    uint32_t* suff_arr;                 //suffix link array
    uint32_t* link_arr;                 //dictionary link array

    uint32_t* edge_map[AHO_SYMB_CNT];   //next edge map
};

int testSAhoTree();

int aho_init_pref_tree(struct SAhoTree* aho_tree, 
                       const char* str_arr[], uint32_t str_cnt);

int aho_insert_new_str(struct SAhoTree* aho_tree, 
                       const char* str, uint32_t str_idx);
int aho_init_tree_link(struct SAhoTree* aho_tree);

int aho_delete_tree(struct SAhoTree* aho_tree);

int aho_dump_tree(struct SAhoTree* aho_tree, 
                  FILE* fout);

int aho_dump_tree_state(struct SAhoTree* aho_tree, uint32_t state_idx, 
                        FILE* fout);

#endif //AHO_AHO_CORASICK_H
