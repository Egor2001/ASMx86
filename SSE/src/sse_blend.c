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
    const __m128i lo_mask = 
        _mm_set_epi16(0x07FF, 0x06FF, 0x05FF, 0x04FF, 
                      0x03FF, 0x02FF, 0x01FF, 0x00FF);
    const __m128i hi_mask = 
        _mm_set_epi16(0x0FFF, 0x0EFF, 0x0DFF, 0x0CFF, 
                      0x0BFF, 0x0AFF, 0x09FF, 0x08FF);

    const __m128i al_lo_mask =
        _mm_set_epi16(0x07FF, 0x07FF, 0x07FF, 0x07FF, 
                      0x03FF, 0x03FF, 0x03FF, 0x03FF);
    const __m128i al_hi_mask =
        _mm_set_epi16(0x0FFF, 0x0FFF, 0x0FFF, 0x0FFF, 
                      0x0BFF, 0x0BFF, 0x0BFF, 0x0BFF);

    const __m128i max_alpha = _mm_set1_epi16(0xFF00);

    uint16_t store_lo_buf[8u] = {};
    uint16_t store_hi_buf[8u] = {};

    uint8_t load_byte_buf[16u] = {};

    for (size_t quad = 0u; quad != pix_cnt/4u; ++quad)
    {
        __m128i lo_alpha = _mm_shuffle_epi8(src[quad].xmm_quad, al_lo_mask);
        __m128i hi_alpha = _mm_shuffle_epi8(src[quad].xmm_quad, al_hi_mask);

        __m128i lo_src_pair = _mm_shuffle_epi8(src[quad].xmm_quad, lo_mask);
        __m128i hi_src_pair = _mm_shuffle_epi8(src[quad].xmm_quad, hi_mask);

        __m128i lo_dest_pair = _mm_shuffle_epi8(dest[quad].xmm_quad, lo_mask);
        __m128i hi_dest_pair = _mm_shuffle_epi8(dest[quad].xmm_quad, hi_mask);

        lo_src_pair = _mm_mulhi_epu16(lo_src_pair, lo_alpha);
        hi_src_pair = _mm_mulhi_epu16(hi_src_pair, hi_alpha);

        lo_alpha = _mm_sub_epi16(max_alpha, lo_alpha);
        hi_alpha = _mm_sub_epi16(max_alpha, hi_alpha);

        lo_dest_pair = _mm_mulhi_epu16(lo_dest_pair, lo_alpha);
        hi_dest_pair = _mm_mulhi_epu16(hi_dest_pair, hi_alpha);

        lo_dest_pair = _mm_add_epi16(lo_dest_pair, lo_src_pair);
        hi_dest_pair = _mm_add_epi16(hi_dest_pair, hi_src_pair);

        _mm_store_si128((__m128i*) store_lo_buf, lo_dest_pair);
        _mm_store_si128((__m128i*) store_hi_buf, hi_dest_pair);

        for (uint8_t word = 0u; word != 8u; ++word)
        {
            load_byte_buf[word + 0u] = store_lo_buf[word] / 0xFF;
            load_byte_buf[word + 8u] = store_hi_buf[word] / 0xFF;
        }

        dest[quad].xmm_quad = _mm_load_si128((__m128i*) load_byte_buf);
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
