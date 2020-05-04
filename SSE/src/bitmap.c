#include "bitmap.h"

int blend_init_bmp32_storage(struct SBlendBMP32Storage* storage, size_t size)
{
    assert(storage);

    storage->size = size - BLEND_BMP32_HEADER_SIZE;

    const size_t align = BLEND_SSE_ALIGN;
    size_t alloc = (storage->size + align - 1u) & ~align;
    storage->data = (uint8_t*) aligned_alloc(align, alloc);

    return alloc;
}

int blend_delete_bmp32_storage(struct SBlendBMP32Storage* storage)
{
    assert(storage);

    free(storage->data);
    storage->data = NULL;
    storage->size = 0u;

    return 0;
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

    return storage_write;
}

int test_bitmap(int argc, char* argv[])
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

    blend_write_bmp32(&upper_header, &lower_storage, out_file);

    blend_delete_bmp32_storage(&upper_storage);
    blend_delete_bmp32_storage(&lower_storage);

    close(out_file); out_file = -1;
    close(upper_file); upper_file = -1;
    close(lower_file); lower_file = -1;

    return 0;
}
