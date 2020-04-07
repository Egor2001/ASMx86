#include <stdio.h>

#include "aho_dfa.h"

void find_pattern(const char* str)
{
    unsigned int state_idx = 0u;
    for (const char* it = str; *it; ++it)
    {
        state_idx = DdDfaEdgeMap[(unsigned char) *it][state_idx];

        unsigned int link_idx = state_idx;
        while (link_idx)
        {
            if (DdDfaTermMap[link_idx])
            {
/*
                printf("detected term:\n%s %#x\n", 
                       DqAhoStrArr[DdDfaTermMap[link_idx] - 1u],
                       (unsigned int) (it - str));
*/
            }

            link_idx = DdDfaLinkArr[link_idx];
        }

    }
}
