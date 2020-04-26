#ifndef SRC_CHASHTABLE_H
#define SRC_CHASHTABLE_H

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cassert>

#include <string_view>

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

    static uint64_t HASH_USE_CNT;
    static uint64_t COMP_USE_CNT;

private:
    static uint64_t hash_func_(const data_type_& str)
    {
        ++HASH_USE_CNT;
        return hash_func_intrin(str);
    }

    static uint64_t comp_pred_(const data_type_& lhs, const data_type_& rhs)
    {
        ++COMP_USE_CNT;
        return !(lhs.compare(rhs));
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
