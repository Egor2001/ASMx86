#ifndef MIRK_H
#define MIRK_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>

#include <elf.h>

#define MIRK_ALIGN 0x1000

struct SMirkContext
{
    uint64_t text_buf_len;
    const char* text_buf;
};

struct SMirkLayout
{
    uint64_t text_off;
    uint64_t text_size;
};

int mirk_gen_layout(const struct SMirkContext* context, 
                    struct SMirkLayout* layout);

int mirk_gen_elf_header(const struct SMirkLayout* layout, 
                        Elf64_Ehdr* elf_header);

int mirk_gen_text_header(const struct SMirkContext* context, 
                         const struct SMirkLayout* layout, 
                         Elf64_Phdr* load_header);

int mirk_gen_elf(const struct SMirkContext* context, FILE* fout);

#endif //MIRK_H
