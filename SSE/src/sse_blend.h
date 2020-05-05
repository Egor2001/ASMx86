#ifndef SRC_SSE_BLEND_H
#define SRC_SSE_BLEND_H

#include "bitmap.h"
#include "data_struct.h"
#include <tmmintrin.h>

int sse_blend_prepare(union SBlendRGBAQuad* dest, size_t pix_cnt);

int sse_blend_execute(union SBlendRGBAQuad* dest, 
                      const union SBlendRGBAQuad* src, size_t pix_cnt);

int test_sse_blend(int argc, char* argv[]);

#endif //SRC_SSE_BLEND_H
