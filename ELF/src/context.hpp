#ifndef SRC_MIRK_CONTEXT_HPP
#define SRC_MIRK_CONTEXT_HPP

#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <cassert>
#include <cstdio>

#include <string_view>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>

#include <elf.h>

class CMirkContext
{
public:
    static constexpr Elf64_Off MIRK_APAGE = 0x1000;
    static constexpr Elf64_Off MIRK_APARA = 0x10;
    static constexpr Elf64_Off MIRK_ANULL = 0x1;

    static constexpr uint32_t MIRK_PROG_NUM = 2u;
    static constexpr uint32_t MIRK_SECT_NUM = 4u;
    static constexpr uint32_t MIRK_SHSTRNDX = 3u;

    static constexpr std::string_view MIRK_SHSTRTAB = 
        std::string_view("\0.text\0.data\0.shstrtab\0", 0x17);

    CMirkContext(uint64_t prog_text_size_other, const uint8_t* text_buf_other,
                 uint64_t prog_data_size_other, const uint8_t* data_buf_other);

    CMirkContext             (const CMirkContext&) = delete;
    CMirkContext& operator = (const CMirkContext&) = delete;

    CMirkContext             (CMirkContext&&) = default;
    CMirkContext& operator = (CMirkContext&&) = default;

    bool create(int fdout) const;

protected:
    //[[nodiscard]] //TODO:to uncomment after testing
    int generate_elf(int fdout) const;
    int generate_elf_segm(int fdout) const;

    int fill_elf_hdr(Elf64_Ehdr* elf_hdr) const;

    int fill_info_progseg_hdr(Elf64_Phdr* info_phdr) const;
    int fill_code_progseg_hdr(Elf64_Phdr* code_phdr) const;

    int fill_init_sec_hdr(Elf64_Shdr* init_sec_hdr) const;
    int fill_text_sec_hdr(Elf64_Shdr* text_sec_hdr) const;
    int fill_data_sec_hdr(Elf64_Shdr* data_sec_hdr) const;
    int fill_shstrtab_hdr(Elf64_Shdr* shstrtab_hdr) const;

private:
    uint64_t prog_text_size_ = MIRK_APARA;
    const uint8_t* text_buf_ = nullptr;

    uint64_t prog_data_size_ = 0x0;
    const uint8_t* data_buf_ = nullptr;

    Elf64_Off prog_entry_off_ = MIRK_APAGE;
    Elf64_Addr prog_load_addr_ = 0x40000;
};

int test_CMirkContext(int argc, char* argv[]);

#endif //SRC_MIRK_CONTEXT_HPP
