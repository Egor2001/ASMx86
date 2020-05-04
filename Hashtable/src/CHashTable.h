#ifndef SRC_CHASHTABLE_H
#define SRC_CHASHTABLE_H

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cassert>

#include <string_view>

extern "C" uint64_t hash_func_extrn(const char* str, uint64_t len);

uint64_t hash_func_len(std::string_view str);
uint64_t hash_func_sum(std::string_view str);
uint64_t hash_func_stdcpp(std::string_view str);
uint64_t hash_func_custom(std::string_view str);
uint64_t hash_func_crc32(std::string_view str);
uint64_t hash_func_intrin(std::string_view str);
uint64_t hash_func_asm(std::string_view str);

class CHashTable
{
public:
    constexpr static uint32_t NULL_IDX = static_cast<uint32_t>(-1);
    using data_type_ = std::string_view;

private:
    static uint64_t hash_func_(const data_type_& str)
    {
        return hash_func_intrin(str);
        //return hash_func_extrn(str.data(), str.size());
    }

    static uint64_t comp_pred_(const data_type_& lhs, const data_type_& rhs)
    {
/*
        uint64_t result = 0;
        __asm__ (
            ".intel_syntax noprefix\n\t"
            "movdqu xmm1, [%[s2]]\n\t"
            "movdqu xmm2, xmm1\n\t"
            "movdqu xmm1, [%[s1]]\n\t"
            "mov rax, %[l1]\n\t"
            "mov rdx, %[l2]\n\t"
            "pcmpestri xmm1, xmm2, 0x08\n\t"
            "mov %[res], rcx\n\t"
            ".att_syntax prefix\n\t"
            : [res] "=r" (result)
            : [s1] "r" (lhs.data()), [s2] "r" (rhs.data()),
            [l1] "rm" (lhs.size()), [l2] "rm" (rhs.size())
            : "cc"
        );

        printf("%llu ", result);
*/
        return !(lhs.compare(rhs))/* + result*/;
    }

public:
    explicit CHashTable(uint32_t capacity_set = 8u);

    CHashTable             (const CHashTable&) = delete;
    CHashTable& operator = (const CHashTable&) = delete;

    CHashTable             (CHashTable&&) = delete;
    CHashTable& operator = (CHashTable&&) = delete;

    ~CHashTable();

    uint32_t insert(const data_type_& val);
    uint32_t erase(const data_type_& val);
    uint32_t find(const data_type_& val) const;
    uint64_t find_fat(const data_type_& val) const;
    uint32_t rehash(uint32_t new_capacity);

    [[nodiscard]]
    bool ok() const noexcept;
    bool dump(FILE* fout) const noexcept;

    void dump_to_csv(FILE* fout) const;

private:
    uint32_t size_;
    uint32_t capacity_;
    uint32_t granularity_;

    uint32_t free_;
    uint32_t* head_arr_;

    data_type_* data_arr_;
    uint32_t* next_arr_;
};

#endif //SRC_CHASHTABLE_H
