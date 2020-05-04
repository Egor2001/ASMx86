#ifndef SRC_BITMAP_H
#define SRC_BITMAP_H

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

struct SBlendBMP32Header
{
    uint8_t data[BLEND_BMP32_HEADER_SIZE];
};

struct SBlendBMP32Storage
{
    uint32_t size;
    uint8_t* data;
};

int blend_init_bmp32_storage(struct SBlendBMP32Storage* storage, size_t size);

int blend_delete_bmp32_storage(struct SBlendBMP32Storage* storage);

int blend_read_bmp32(struct SBlendBMP32Header* header, 
                     struct SBlendBMP32Storage* storage, int fin);

int blend_write_bmp32(const struct SBlendBMP32Header* header, 
                      const struct SBlendBMP32Storage* storage, int fout);

int test_bitmap(int argc, char* argv[]);

#endif //SRC_BITMAP_H
