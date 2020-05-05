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
        //return !(lhs.compare(rhs));

        uint64_t result = 0;
        __asm__ (
            ".intel_syntax noprefix\n\t"    //syntax = intel
            "mov rax, %[l1]\n\t"            //rax <- len1
            "mov rdx, %[l2]\n\t"            //rdx <- len2
            "loop_start:\n\t"               //strcmp loop start
            "movdqu xmm1, [%[s2]]\n\t"          //xmm1 <- str2
            "movdqu xmm2, xmm1\n\t"             //xmm2 <- xmm1
            "movdqu xmm1, [%[s1]]\n\t"          //xmm2 <- str1
            "pcmpestri xmm1, xmm2, 0x0C\n\t"    //res = strcmp 16 bytes
            "jnz loop_end\n\t"                      //res != 0 => break
            "cmp %[l1], 16\n\t"                 //len1 <= 16 =>
            "jle loop_end\n\t"                      //=> break
            "cmp %[l2], 16\n\t"                 //len2 <= 16 =>
            "jle loop_end\n\t"                      //=> break
            "sub %[l1], 16\n\t"                 //len1 <- len1 - 16
            "sub %[l2], 16\n\t"                 //len2 <- len2 - 16
            "add %[s1], 16\n\t"                 //str1 <- str1 + 16
            "add %[s2], 16\n\t"                 //str2 <- str2 + 16
            "loop loop_start\n\t"               //continue
            "loop_end:\n\t"                 //strcmp loop end
            "mov %[res], rcx\n\t"           //result <- res
            ".att_syntax prefix\n\t"        //syntax = AT&T
            : [res] "=r" (result)
            : [s1] "r" (lhs.data()), [s2] "r" (rhs.data()),
            [l1] "rm" (lhs.size()), [l2] "rm" (rhs.size())
            : "cc", "rax", "rdx", "rcx", "xmm1", "xmm2"
        );

        return result;
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
