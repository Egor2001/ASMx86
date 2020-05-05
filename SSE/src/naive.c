#include "naive.h"

int naive_blend_prepare(union SBlendRGBA* dest, size_t pix_cnt)
{
    for (size_t pix = 0u; pix != pix_cnt; ++pix)
    {
        if (!dest[pix].a)
        {
            for (size_t i = 0u; i != 4u; ++i)
                dest[pix].rgba[i] = 255u;
        }
        else
        {
            dest[pix].a = 255u;
        }
    }

    return 0;
}

int naive_blend_execute(union SBlendRGBA* dest, const union SBlendRGBA* src,
                        size_t pix_cnt)
{
    for (size_t pix = 0u; pix != pix_cnt; ++pix)
    {
        dest[pix].r = 
            (dest[pix].r*(255u - src[pix].a) + src[pix].r*src[pix].a)/255u;
        dest[pix].g = 
            (dest[pix].g*(255u - src[pix].a) + src[pix].g*src[pix].a)/255u;
        dest[pix].b = 
            (dest[pix].b*(255u - src[pix].a) + src[pix].b*src[pix].a)/255u;
    }

    return 0;
}

int test_naive_blend(int argc, char* argv[])
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

    naive_blend_execute((union SBlendRGBA*) lower_storage.data,
                        (const union SBlendRGBA*) upper_storage.data,
                        lower_storage.size/4u);

    blend_write_bmp32(&upper_header, &lower_storage, out_file);

    blend_delete_bmp32_storage(&upper_storage);
    blend_delete_bmp32_storage(&lower_storage);

    close(out_file); out_file = -1;
    close(upper_file); upper_file = -1;
    close(lower_file); lower_file = -1;

    return 0;
}
