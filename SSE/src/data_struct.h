#ifndef SRC_DATA_STRUCT_H
#define SRC_DATA_STRUCT_H

#include <emmintrin.h>

union SBlendRGBA
{
    struct { uint8_t r, g, b, a; };
    uint8_t rgba[4];
};

union SBlendRGBAQuad
{
    union SBlendRGBA rgba_quad_arr[4];
    __m128i xmm_quad;
};

#endif //SRC_DATA_STRUCT_H
