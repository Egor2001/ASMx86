#ifndef SRC_NAIVE_H
#define SRC_NAIVE_H

#include "bitmap.h"

union SBlendRGBA
{
    struct { uint8_t r, g, b, a; };
    uint8_t rgba[4];
};

int blend_prepare(union SBlendRGBA* dest, size_t pix_cnt);

int blend_execute(union SBlendRGBA* dest, const union SBlendRGBA* src,
                  size_t pix_cnt);

int test_naive(int argc, char* argv[]);

#endif //SRC_NAIVE_H
