#include "sse_blend.h"

int sse_blend_prepare(union SBlendRGBAQuad* dest, size_t pix_cnt)
{
    for (size_t pix = 0u; pix < pix_cnt; ++pix)
    {
        if (!dest[pix/4u].rgba_quad_arr[pix%4u].a)
        {
            for (size_t i = 0u; i != 4u; ++i)
                dest[pix/4u].rgba_quad_arr[pix%4u].rgba[i] = 255u;
        }
        else
        {
            dest[pix/4u].rgba_quad_arr[pix%4u].a = 255u;
        }
    }

    return 0;
}

int sse_blend_execute(union SBlendRGBAQuad* dest, 
                      const union SBlendRGBAQuad* src, size_t pix_cnt)
{
    const __m128i alpha_mask =
        _mm_set_epi16(0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 
                      0x07FF, 0x07FF, 0x07FF, 0x07FF);

    const __m128i pack_mask =
        _mm_set_epi16(0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
                      0x0F0D, 0x0B09, 0x0705, 0x0301);

    const __m128i unit = _mm_set1_epi8(0xFF);

    __m128i word_0 = {}, word_1 = {}, alpha = {};
    __m128i src_pair = {}, dest_pair = {};

    for (size_t quad = 0u; quad != pix_cnt/4u; ++quad)
    {
        src_pair = _mm_unpacklo_epi8(unit, src[quad].xmm_quad);
        dest_pair = _mm_unpacklo_epi8(unit, dest[quad].xmm_quad);

        alpha = _mm_shuffle_epi8(src_pair, alpha_mask);
        src_pair = _mm_mulhi_epu16(src_pair, alpha);

        alpha = _mm_sub_epi16(unit, alpha);
        dest_pair = _mm_mulhi_epu16(dest_pair, alpha);

        word_0 = _mm_add_epi16(src_pair, dest_pair);
        word_0 = _mm_shuffle_epi8(word_0, pack_mask);

        src_pair = _mm_unpackhi_epi8(unit, src[quad].xmm_quad);
        dest_pair = _mm_unpackhi_epi8(unit, dest[quad].xmm_quad);

        alpha = _mm_shuffle_epi8(src_pair, alpha_mask);
        src_pair = _mm_mulhi_epu16(src_pair, alpha);

        alpha = _mm_sub_epi16(unit, alpha);
        dest_pair = _mm_mulhi_epu16(dest_pair, alpha);

        word_1 = _mm_add_epi16(src_pair, dest_pair);
        word_1 = _mm_shuffle_epi8(word_1, pack_mask);

        dest[quad].xmm_quad = _mm_unpacklo_epi64(word_0, word_1);
    }

    return 0;
}

int test_sse_blend(int argc, char* argv[])
{
    assert(argc == 4);
    int lower_file = open(argv[1], O_RDONLY); assert(lower_file != -1);
    int upper_file = open(argv[2], O_RDONLY); assert(upper_file != -1);
    int out_file = open(argv[3], O_CREAT | O_RDWR, 0644); 
    assert(out_file != -1);

    struct stat lower_stat = {};
    struct stat upper_stat = {};
    fstat(lower_file, &lower_stat);
    fstat(upper_file, &upper_stat);

    size_t lower_size = lower_stat.st_size;
    size_t upper_size = upper_stat.st_size;

    struct SBlendBMP32Header lower_header = {}, upper_header = {};
    struct SBlendBMP32Storage lower_storage = {}, upper_storage = {};
    blend_init_bmp32_storage(&lower_storage, lower_size);
    blend_init_bmp32_storage(&upper_storage, upper_size);

    blend_read_bmp32(&lower_header, &lower_storage, lower_file);
    blend_read_bmp32(&upper_header, &upper_storage, upper_file);

    sse_blend_execute((union SBlendRGBAQuad*) lower_storage.data,
                      (const union SBlendRGBAQuad*) upper_storage.data,
                      lower_storage.size/4u);

    blend_write_bmp32(&upper_header, &lower_storage, out_file);

    blend_delete_bmp32_storage(&upper_storage);
    blend_delete_bmp32_storage(&lower_storage);

    close(out_file); out_file = -1;
    close(upper_file); upper_file = -1;
    close(lower_file); lower_file = -1;

    return 0;
}
