#include "mirk.h"

int mirk_gen_layout(const struct SMirkContext* context, 
                    struct SMirkLayout* layout)
{
    assert(context);
    assert(layout);

    return 0u;
}

int mirk_gen_elf_header(const struct SMirkLayout* layout, 
                        Elf64_Ehdr* elf_header)
{
    assert(layout);
    assert(elf_header);

    Elf64_Ehdr result = {
        .e_ident = {
            [EI_MAG0] = ELFMAG0,
            [EI_MAG1] = ELFMAG1,
            [EI_MAG2] = ELFMAG2,
            [EI_MAG3] = ELFMAG3,
            [EI_CLASS] = ELFCLASS64,
            [EI_DATA] = ELFDATA2LSB,
            [EI_VERSION] = EV_CURRENT,
            [EI_OSABI] = ELFOSABI_SYSV,
            [EI_ABIVERSION] = 0u,
        },
        .e_type = ET_EXEC,
        .e_machine = EM_X86_64,
        .e_version = EV_CURRENT,
        .e_phoff = sizeof(Elf64_Ehdr),
        .e_shoff = 0u,
        .e_flags = 0x0,
        .e_ehsize = sizeof(Elf64_Ehdr),
        .e_phentsize = sizeof(Elf64_Phdr),
        .e_phnum = 1u,
        .e_shentsize = 0u,
        .e_shnum = 0u,
        .e_shstrndx = SHN_UNDEF
    }; 

    *elf_header = result;

    return 0u;
}

int mirk_gen_text_header(const struct SMirkContext* context, 
                         const struct SMirkLayout* layout, 
                         Elf64_Phdr* load_header)
{
    assert(context);
    assert(layout);
    assert(load_header);

    Elf64_Phdr result = {
        .p_type = PT_LOAD,
        .p_flags = PF_R | PF_X,
        .p_offset = 0x0,
        .p_vaddr = 0x0,
        .p_paddr = 0x0,
        .p_filesz = 0x0,
        .p_memsz = 0x0,
        .p_align = 0x0
    };

    *load_header = result;

    return 0u;
}

int mirk_gen_elf(const struct SMirkContext* context, FILE* fout)
{
    assert(context);
    assert(fout);

    struct SMirkLayout layout = {};
    mirk_gen_layout(context, &layout);

    Elf64_Ehdr elf_header = {};
    mirk_gen_elf_header(&layout, &elf_header);

    Elf64_Phdr ph_text = {};
    mirk_gen_text_header(context, &layout, &ph_text);

    fwrite(&elf_header, sizeof(elf_header), 1u, fout);
    fwrite(&ph_text, sizeof(ph_text), 1u, fout);

    return 0;
}
