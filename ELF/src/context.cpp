#include "context.hpp"

CMirkContext::
CMirkContext(uint64_t prog_text_size_other, const uint8_t* text_buf_other,
             uint64_t prog_data_size_other, const uint8_t* data_buf_other):
    prog_text_size_{ prog_text_size_other }, text_buf_{ text_buf_other },
    prog_data_size_{ prog_data_size_other }, data_buf_{ data_buf_other },
    prog_entry_off_{ MIRK_APAGE }, prog_load_addr_{ 0x400000 }
{}

bool CMirkContext::create(int fdout) const
{
    return generate_elf(fdout) == 0;
}

int CMirkContext::generate_elf(int fdout) const
{
    assert(fdout != -1);

    int cursize = 0u;

    Elf64_Ehdr elf_header = {};
    fill_elf_hdr(&elf_header);

    Elf64_Phdr elf_phtab[2u] = {};
    fill_info_progseg_hdr(elf_phtab + 0u);
    fill_code_progseg_hdr(elf_phtab + 1u);

    Elf64_Shdr elf_shtab[4u] = {};
    fill_init_sec_hdr(elf_shtab + 0u);
    fill_text_sec_hdr(elf_shtab + 1u);
    fill_data_sec_hdr(elf_shtab + 2u);
    fill_shstrtab_hdr(elf_shtab + 3u);

    cursize += pwrite(fdout, &elf_header, sizeof(elf_header), 0x0);
    cursize += pwrite(fdout, elf_phtab, sizeof(elf_phtab), elf_header.e_phoff);

    cursize += generate_elf_segm(fdout);

    cursize += pwrite(fdout, elf_shtab, sizeof(elf_shtab), elf_header.e_shoff);

    return cursize;
}

int CMirkContext::generate_elf_segm(int fdout) const
{
    assert(fdout != -1);

    off_t cur_off = prog_entry_off_;
    cur_off += pwrite(fdout, text_buf_, prog_text_size_, cur_off);
    cur_off += pwrite(fdout, data_buf_, prog_data_size_, cur_off);
    cur_off += pwrite(fdout, MIRK_SHSTRTAB.data(), 
                             MIRK_SHSTRTAB.size(), cur_off);

    return cur_off - prog_entry_off_;
}

int CMirkContext::fill_elf_hdr(Elf64_Ehdr* elf_hdr) const
{
    assert(elf_hdr);

    Elf64_Off ph_tab_offset = sizeof(Elf64_Ehdr);
    Elf64_Off sh_tab_offset = MIRK_APAGE + prog_text_size_ + prog_data_size_ +
                              MIRK_SHSTRTAB.size();

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
        .e_entry = prog_load_addr_ + prog_entry_off_,
        .e_phoff = ph_tab_offset,
        .e_shoff = sh_tab_offset,
        .e_flags = 0x0,
        .e_ehsize = sizeof(Elf64_Ehdr),
        .e_phentsize = sizeof(Elf64_Phdr),
        .e_phnum = MIRK_PROG_NUM,
        .e_shentsize = sizeof(Elf64_Shdr),
        .e_shnum = MIRK_SECT_NUM,
        .e_shstrndx = MIRK_SHSTRNDX
    }; 

    *elf_hdr = result;

    return 0u;
}

int CMirkContext::fill_info_progseg_hdr(Elf64_Phdr* info_phdr) const
{
    assert(info_phdr);

    //size of ELF header and prog headers
    uint64_t info_size = sizeof(Elf64_Ehdr) + MIRK_PROG_NUM*sizeof(Elf64_Phdr);

    //load info header
    Elf64_Phdr result = {
        .p_type = PT_LOAD,
        .p_flags = PF_R,
        .p_offset = 0x0,
        .p_vaddr = prog_load_addr_,
        .p_paddr = prog_load_addr_,
        .p_filesz = info_size,
        .p_memsz = info_size,
        .p_align = MIRK_APAGE
    };

    *info_phdr = result;

    return 0;
}

int CMirkContext::fill_code_progseg_hdr(Elf64_Phdr* code_phdr) const
{
    assert(code_phdr);

    //size of ELF text and data segments
    uint64_t code_size = prog_text_size_ + prog_data_size_;

    //load code header
    Elf64_Phdr result = {
        .p_type = PT_LOAD,
        .p_flags = PF_R | PF_X,
        .p_offset = prog_entry_off_,
        .p_vaddr = prog_load_addr_ + prog_entry_off_,
        .p_paddr = prog_load_addr_ + prog_entry_off_,
        .p_filesz = code_size,
        .p_memsz = code_size,
        .p_align = MIRK_APAGE
    };

    *code_phdr = result;

    return 0;
}

int CMirkContext::fill_init_sec_hdr(Elf64_Shdr* init_sec_hdr) const
{
    assert(init_sec_hdr);

    Elf64_Shdr result = {
        .sh_name = 0u,
        .sh_type = SHT_NULL,
    };

    *init_sec_hdr = result;

    return 0;
}

int CMirkContext::fill_text_sec_hdr(Elf64_Shdr* text_sec_hdr) const
{
    assert(text_sec_hdr);

    Elf64_Shdr result = {
        .sh_name = static_cast<uint32_t>(MIRK_SHSTRTAB.find(".text")),
        .sh_type = SHT_PROGBITS,
        .sh_flags = SHF_ALLOC | SHF_EXECINSTR,
        .sh_addr = prog_load_addr_ + prog_entry_off_,
        .sh_offset = prog_entry_off_,
        .sh_size = prog_text_size_,
        .sh_link = 0u,
        .sh_info = 0u,
        .sh_addralign = MIRK_APARA,
        .sh_entsize = 0u
    };

    *text_sec_hdr = result;

    return 0;
}

int CMirkContext::fill_data_sec_hdr(Elf64_Shdr* data_sec_hdr) const
{
    assert(data_sec_hdr);

    Elf64_Shdr result = {
        .sh_name = static_cast<uint32_t>(MIRK_SHSTRTAB.find(".data")),
        .sh_type = SHT_PROGBITS,
        .sh_flags = SHF_ALLOC | SHF_WRITE,
        .sh_addr = prog_load_addr_ + prog_entry_off_,
        .sh_offset = prog_entry_off_ + prog_text_size_,
        .sh_size = prog_data_size_,
        .sh_link = 0u,
        .sh_info = 0u,
        .sh_addralign = MIRK_APARA,
        .sh_entsize = 0u
    };

    *data_sec_hdr = result;

    return 0;
}

int CMirkContext::fill_shstrtab_hdr(Elf64_Shdr* shstrtab_hdr) const
{
    assert(shstrtab_hdr);

    Elf64_Shdr result = {
        .sh_name = static_cast<uint32_t>(MIRK_SHSTRTAB.find(".shstrtab")),
        .sh_type = SHT_STRTAB,
        .sh_flags = 0x0,
        .sh_addr = 0x0,
        .sh_offset = prog_entry_off_ + prog_text_size_ + prog_data_size_,
        .sh_size = MIRK_SHSTRTAB.size(),
        .sh_link = 0u,
        .sh_info = 0u,
        .sh_addralign = MIRK_ANULL,
        .sh_entsize = 0u
    };

    *shstrtab_hdr = result;

    return 0;
}

int test_CMirkContext(int argc, char* argv[])
{
    assert(argc == 2);
    int fdout = open(argv[1u], O_RDWR | O_CREAT, 0766);

    constexpr uint64_t TEXT_SIZE = 0x30;
    constexpr uint64_t DATA_SIZE = 0x30;

    const uint8_t text[TEXT_SIZE] = { 0xb8, 0x3c, 0x00, 0x00, 0x00, 
                                      0x48, 0x31, 0xff, 
                                      0x0f, 0x05, 0xc3 };
    const uint8_t data[DATA_SIZE] = {};
    CMirkContext context(TEXT_SIZE, text, DATA_SIZE, data);
    context.create(fdout);

    close(fdout);
    fdout = -1;

    return 0u;
}
