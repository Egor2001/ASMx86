#include "src/naive.h"
#include "src/sse_blend.h"

int test_blend(int argc, char* argv[], bool sse = false, size_t iter = 1u);

int main(int argc, char* argv[])
{
    test_blend(argc, argv, true, 1);

    return 0;
}

int test_blend(int argc, char* argv[], bool sse, size_t iter)
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

    if (sse)
    {
        for (size_t i = 0u; i < iter; ++i)
        {
            sse_blend_execute((union SBlendRGBAQuad*) lower_storage.data,
                              (const union SBlendRGBAQuad*) upper_storage.data,
                              lower_storage.size/4u);
        }
    }
    else
    {
        for (size_t i = 0u; i < iter; ++i)
        {
            naive_blend_execute((union SBlendRGBA*) lower_storage.data,
                                (const union SBlendRGBA*) upper_storage.data,
                                lower_storage.size/4u);
        }
    }

    blend_write_bmp32(&upper_header, &lower_storage, out_file);

    blend_delete_bmp32_storage(&upper_storage);
    blend_delete_bmp32_storage(&lower_storage);

    close(out_file); out_file = -1;
    close(upper_file); upper_file = -1;
    close(lower_file); lower_file = -1;

    return 0;
}
