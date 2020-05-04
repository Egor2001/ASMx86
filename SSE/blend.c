#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define BLEND_BMP32_HEADER_SIZE 138
#define BLEND_SSE_ALIGN 16

union SBlendRGBA
{
    struct { uint8_t r, g, b, a; };
    uint8_t rgba[4];
};

struct SBlendBMP32Header
{
    uint8_t data[BLEND_BMP32_HEADER_SIZE];
};

struct SBlendBMP32Storage
{
    uint32_t size;
    uint8_t* data;
};

int blend_init_bmp32_storage(struct SBlendBMP32Storage* storage, size_t size)
{
    assert(storage);

    const size_t align = BLEND_SSE_ALIGN;
    storage->size = size - BLEND_BMP32_HEADER_SIZE;
    storage->data = 
        (uint8_t*) aligned_alloc(align, (storage->size + align - 1u) & ~align);
}

int blend_delete_bmp32_storage(struct SBlendBMP32Storage* storage)
{
    assert(storage);

    free(storage->data);
    storage->data = NULL;
}

int blend_read_bmp32(struct SBlendBMP32Header* header, 
                     struct SBlendBMP32Storage* storage, int fin)
{
    assert(header);
    assert(storage);
    assert(fin != -1);

    int header_read = read(fin, header->data, BLEND_BMP32_HEADER_SIZE);
    assert(header_read == BLEND_BMP32_HEADER_SIZE);

    int storage_read = pread(fin, storage->data, storage->size, 
                             BLEND_BMP32_HEADER_SIZE);
    assert(storage_read == storage->size);

/*
    result = storage->size;
    while (result)
        result += fread(storage->data + size - result, result, 1, fin);
*/

    return storage_read;
}

int blend_write_bmp32(const struct SBlendBMP32Header* header, 
                      const struct SBlendBMP32Storage* storage, int fout)
{
    assert(header);
    assert(storage);
    assert(fout != -1);

    int header_write = write(fout, header->data, BLEND_BMP32_HEADER_SIZE);
    assert(header_write == BLEND_BMP32_HEADER_SIZE);

    int storage_write = pwrite(fout, storage->data, storage->size, 
                               BLEND_BMP32_HEADER_SIZE);
    assert(storage_write == storage->size);

    /* ... */

    return storage_write;
}

int blend_background(union SBlendRGBA* dest, size_t pix_cnt)
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

int blend_execute(union SBlendRGBA* dest, const union SBlendRGBA* src,
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

int main(int argc, char* argv[])
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
/*
    blend_background((union SBlendRGBA*) lower_storage.data,
                     lower_storage.size/4u);
*/
    blend_execute((union SBlendRGBA*) lower_storage.data,
                  (const union SBlendRGBA*) upper_storage.data,
                  lower_storage.size/4u);

    blend_write_bmp32(&upper_header, &lower_storage, out_file);
/*
    printf("data size: %d\n", lower_storage.size);

    int fb = open("/dev/fb0", O_RDWR);
    write(fb, lower_storage.data, lower_storage.size);
    close(fb);
*/
    blend_delete_bmp32_storage(&upper_storage);
    blend_delete_bmp32_storage(&lower_storage);

    close(out_file); out_file = -1;
    close(upper_file); upper_file = -1;
    close(lower_file); lower_file = -1;

    return 0;
}
