#include "CHashTable.h"

uint64_t hash_func_len(std::string_view str)
{
    return str.size();
}

uint64_t hash_func_sum(std::string_view str)
{
    uint64_t hash = 0u;
    for (size_t idx = 0u; idx != str.size(); ++idx)
        hash += static_cast<uint64_t>(str[idx]);

    return hash;
}

uint64_t hash_func_custom(std::string_view str)
{
    constexpr static uint64_t SALT = 0xb1bab0bafeedbabe;
    //TODO: to shuffle better!
    constexpr static uint64_t SHIFT_ARR[16u] = { 
        0u, 8u, 16u, 24u, 4u, 12u, 20u, 28u, 
        56u, 48u, 40u, 32u, 60u, 52u, 44u, 36u
    };

    uint64_t hash = 0u;
    for (size_t idx = 0u; idx != str.size(); ++idx)
        hash ^= static_cast<uint64_t>(str[idx]) << SHIFT_ARR[idx & 0x7f];

    return ((hash >> 32u) ^ hash) ^ SALT;
}

uint64_t hash_func_crc32(std::string_view str)
{
    //IEEE standard reverted (LSB, poly = 0xEDB88320)
    constexpr static uint64_t byte_hash_arr[256] = { 
        #include "CRC32_IEEE_REV.h"
    };

    uint64_t hash = ~0LLU;
    for (char symb : str)
    {
        uint8_t idx = hash ^ static_cast<uint64_t>(symb);
        hash = byte_hash_arr[idx] ^ (hash >> 8u);
    }

    return hash;
}

CHashTable::CHashTable(uint32_t capacity_set):
    size_(0u),
    capacity_(capacity_set),
    granularity_((capacity_ + 7u)/8u),
    free_(0u),
    head_arr_(nullptr),
    data_arr_(nullptr),
    next_arr_(nullptr)
{
    head_arr_ = new uint32_t[granularity_];
    data_arr_ = new data_type_[capacity_];
    next_arr_ = new uint32_t[capacity_];

    for (uint32_t head_idx = 0u; head_idx < granularity_; ++head_idx)
        head_arr_[head_idx] = NULL_IDX;

    for (uint32_t idx = 0u; idx < capacity_; ++idx)
        next_arr_[idx] = idx + 1u;

    next_arr_[capacity_ - 1u] = NULL_IDX;
}

CHashTable::~CHashTable()
{
    delete [] head_arr_;
    delete [] data_arr_;
    delete [] next_arr_;

    head_arr_ = nullptr;
    data_arr_ = nullptr;
    next_arr_ = nullptr;

    size_ = capacity_ = granularity_ = 0u;
    free_ = NULL_IDX;
}

uint32_t CHashTable::insert(const data_type_& val)
{
    if (size_ == capacity_)
        rehash(capacity_*2u);

    uint64_t fat_idx = find_fat(val);

    uint32_t prev = fat_idx;
    uint32_t hash = fat_idx >> 32u;

    uint32_t idx = (prev == NULL_IDX ? head_arr_[hash] : next_arr_[prev]);

    uint64_t next_free = next_arr_[free_];
    data_arr_[free_] = val;
    if (idx == NULL_IDX)
    {
        next_arr_[free_] = head_arr_[hash];
        head_arr_[hash] = free_;
    }
    else
    {
        next_arr_[free_] = next_arr_[idx];
        next_arr_[idx] = free_;
    }

    idx = free_;
    free_ = next_free;
    ++size_;

    return idx;
}

uint32_t CHashTable::erase(const data_type_& val)
{
    uint64_t fat_idx = find_fat(val);

    uint32_t prev = fat_idx;
    uint32_t hash = fat_idx >> 32u;

    uint32_t idx = (prev == NULL_IDX ? head_arr_[hash] : next_arr_[prev]);
    if (idx != NULL_IDX)
    {
        if (prev != NULL_IDX)
            next_arr_[prev] = next_arr_[idx];
        else
            head_arr_[hash] = next_arr_[idx];

        next_arr_[idx] = free_;
        free_ = idx;

        --size_;
    }

    return idx;
}

uint32_t CHashTable::find(const data_type_& val) const
{
    uint64_t fat_idx = find_fat(val);

    uint32_t prev = fat_idx;
    uint32_t hash = fat_idx >> 32u;

    return (prev == NULL_IDX ? head_arr_[hash] : next_arr_[prev]);
}

uint64_t CHashTable::find_fat(const data_type_& val) const
{
    uint64_t hash = hash_func_(val);
    hash %= granularity_;

    uint64_t idx = head_arr_[hash];
    uint64_t prev = NULL_IDX;
    while (idx != NULL_IDX && !comp_pred_(data_arr_[idx], val))
    {
        prev = idx;
        idx = next_arr_[idx];
    }

    return (static_cast<uint64_t>(hash) << 32u) | 
            static_cast<uint64_t>(prev);
}

uint32_t CHashTable::rehash(uint32_t new_capacity)
{
    uint32_t new_granularity = (new_capacity + 7u)/8u;

    uint32_t new_free = 0u;
    uint32_t* new_head_arr = new uint32_t[new_granularity];

    data_type_* new_data_arr = new data_type_[new_capacity];
    uint32_t* new_next_arr = new uint32_t[new_capacity];

    for (uint32_t head_idx = 0u; head_idx < new_granularity; ++head_idx)
        new_head_arr[head_idx] = NULL_IDX;

    for (uint32_t idx = 0u; idx < new_capacity; ++idx)
        new_next_arr[idx] = idx + 1u;

    new_next_arr[new_capacity - 1u] = NULL_IDX;

    uint32_t old_granularity = granularity_;

    uint32_t* old_head_arr = head_arr_;
    data_type_* old_data_arr = data_arr_;
    uint32_t* old_next_arr = next_arr_;

    size_ = 0u;
    capacity_ = new_capacity;
    granularity_ = new_granularity;

    free_ = new_free;
    head_arr_ = new_head_arr;

    data_arr_ = new_data_arr;
    next_arr_ = new_next_arr;

    for (uint32_t head_idx = 0u; head_idx < old_granularity; ++head_idx)
    {
        for (uint32_t idx = old_head_arr[head_idx]; idx != NULL_IDX;
             idx = old_next_arr[idx])
        {
            insert(old_data_arr[idx]);
        }
    }

    delete [] old_head_arr;
    delete [] old_data_arr;
    delete [] old_next_arr;

    old_head_arr = nullptr;
    old_data_arr = nullptr;
    old_next_arr = nullptr;

    return capacity_;
}

bool CHashTable::ok() const noexcept
{
    return (size_ < capacity_) && 
           (granularity_ == (capacity_ + 7u)/8u) &&
           (free_ < size_) && head_arr_ && data_arr_ && next_arr_;
}

bool CHashTable::dump(FILE* fout) const noexcept
{
    if (!fout) 
        return false;

    fprintf(fout, "CHashTable::dump():\n"
                  "{\n"
                  "    size = %lu;\n"
                  "    capacity = %lu;\n"
                  "    granularity = %lu;\n"
                  "    \n"
                  "    free = %lu\n"
                  "    head_arr = %p\n"
                  "    \n"
                  "    data_arr = %p\n"
                  "    next_arr = %p\n"
                  "}\n",
                  size_, capacity_, granularity_,
                  free_, head_arr_,
                  data_arr_, next_arr_);

    fprintf(fout, "head_arr = {\n\t");
    for (uint32_t idx = 0u; idx < granularity_; ++idx)
    {
        fprintf(fout, "%lu, ", head_arr_[idx]);
    }
    fprintf(fout, "\n} //head_arr\n");

    fprintf(fout, "next_arr = {\n\t");
    for (uint32_t idx = 0u; idx < capacity_; ++idx)
    {
        fprintf(fout, "%lu, ", next_arr_[idx]);
    }
    fprintf(fout, "\n} //next_arr\n");

    return ok();
}

void CHashTable::dump_to_csv(FILE* fout) const
{
    for (uint32_t head_idx = 0u; head_idx < granularity_; ++head_idx)
    {
        uint32_t cur_val = 0u;
        for (uint32_t idx = head_arr_[head_idx]; idx != NULL_IDX;
             idx = next_arr_[idx])
        {
            ++cur_val;
        }

        fprintf(fout, "%lu, ", cur_val);
    }
}

int test_CHashTable(int argc, char* argv[])
{
    CHashTable hashtable(64u);
    constexpr std::string_view str_arr[8u] = {
        "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"
    };

    for (uint32_t i = 1u; i < 6u; ++i)
        hashtable.insert(str_arr[i]);

    for (uint32_t i = 3u; i < 5u; ++i)
        hashtable.erase(str_arr[i]);

    hashtable.dump(stdout);

    for (uint32_t i = 0u; i < 8u; ++i)
        printf("[%s] = %lu\n", str_arr[i].data(), hashtable.find(str_arr[i]));

    return 0;
}

