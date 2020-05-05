#ifndef SRC_NAIVE_H
#define SRC_NAIVE_H

#include "bitmap.h"
#include "data_struct.h"

int naive_blend_prepare(union SBlendRGBA* dest, size_t pix_cnt);

int naive_blend_execute(union SBlendRGBA* dest, const union SBlendRGBA* src,
                        size_t pix_cnt);

int test_naive_blend(int argc, char* argv[]);

#endif //SRC_NAIVE_H
