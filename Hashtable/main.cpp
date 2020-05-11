#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <vector>
#include <string>
#include <string_view>

#include "src/CHashTable.h"

int test_csv_file(int argc, char* argv[]);
int test_csv_mmap(int argc, char* argv[]);

//copy-pasted just to prove that hashtable and asm compare is correct)
int test_csv_mmap_aligned(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    test_csv_mmap(argc, argv);
    //test_csv_mmap_aligned(argc, argv);

    return 0;
}

int test_csv_file(int argc, char* argv[])
{
    int fd_in = open(argv[1], O_RDWR);
    assert(fd_in != -1);

    FILE* fout = fopen(argv[2], "w");
    assert(fout);

    struct stat fd_in_stat = {};
    fstat(fd_in, &fd_in_stat);

    //OK, OK, I know that sizeof(char) == 1!
    char* str = static_cast<char*>(calloc(sizeof(char), fd_in_stat.st_size));
    assert(str);

    //TODO: to wrap with cycle
    read(fd_in, str, fd_in_stat.st_size*sizeof(char));

    //magic value beause of its use in scanf fmtstring
    auto str_view = std::string_view(str, fd_in_stat.st_size);
    std::string_view term_str = ",.-!?'\" \t\n\r";

    std::size_t pref_len = str_view.find_first_not_of(term_str);
    pref_len = std::min(pref_len, str_view.length());
    str_view.remove_prefix(pref_len);

    CHashTable hashtable;
    while (!str_view.empty())
    {
        pref_len = str_view.find_first_of(term_str);
        pref_len = std::min(pref_len, str_view.length());

        hashtable.insert(str_view.substr(pref_len));
        str_view.remove_prefix(pref_len);

        pref_len = str_view.find_first_not_of(term_str);
        pref_len = std::min(pref_len, str_view.length());
        str_view.remove_prefix(pref_len);
    }

    hashtable.dump_to_csv(fout);

    free(str);
    str = nullptr;

    fclose(fout);
    fout = nullptr;

    close(fd_in);
    fd_in = -1;

    return 0;
}

int test_csv_mmap(int argc, char* argv[])
{
    int fd_in = open(argv[1], O_RDWR);
    assert(fd_in != -1);

    FILE* fout = fopen(argv[2], "w");
    assert(fout);

    struct stat fd_in_stat = {};
    fstat(fd_in, &fd_in_stat);
    char* str = static_cast<char*>(mmap(nullptr, fd_in_stat.st_size, 
                                        PROT_READ, MAP_PRIVATE, fd_in, 0));
    assert(str);

    auto str_view = std::string_view(str, fd_in_stat.st_size);
    std::string_view term_str = ",.-!?'\" \t\n\r";

    std::size_t pref_len = str_view.find_first_not_of(term_str);
    pref_len = std::min(pref_len, str_view.length());
    str_view.remove_prefix(pref_len);

    std::vector<std::string_view> search_vec;
    search_vec.reserve(128*1024*1024);

    CHashTable hashtable;
    while (!str_view.empty())
    {
        pref_len = str_view.find_first_of(term_str);
        pref_len = std::min(pref_len, str_view.length());

        std::string_view cur_view = str_view.substr(0u, pref_len);
        search_vec.push_back(cur_view);

        hashtable.insert(cur_view);
        str_view.remove_prefix(pref_len);

        pref_len = str_view.find_first_not_of(term_str);
        pref_len = std::min(pref_len, str_view.length());
        str_view.remove_prefix(pref_len);
    }

    srand(time(0));
    size_t mod = search_vec.size();
    size_t res = 0u;
    for (size_t i = 0u; i < 32*1024*1024; ++i)
    {
        size_t idx = rand()%mod;
        res += hashtable.find(search_vec[idx]);
    }
    hashtable.insert(search_vec[res%mod]);

    hashtable.dump_to_csv(fout);

    fclose(fout);
    fout = nullptr;

    munmap(str, fd_in_stat.st_size); 
    str = nullptr;

    close(fd_in);
    fd_in = -1;

    return 0;
}

int test_csv_mmap_aligned(int argc, char* argv[])
{
    int fd_in = open(argv[1], O_RDWR);
    assert(fd_in != -1);

    FILE* fout = fopen(argv[2], "w");
    assert(fout);

    struct stat fd_in_stat = {};
    fstat(fd_in, &fd_in_stat);
    char* str = static_cast<char*>(mmap(nullptr, fd_in_stat.st_size, 
                                        PROT_READ, MAP_PRIVATE, fd_in, 0));
    assert(str);

    auto str_view = std::string_view(str, fd_in_stat.st_size);
    std::string_view term_str = ",.-!?'\" \t\n\r";

    std::size_t pref_len = str_view.find_first_not_of(term_str);
    pref_len = std::min(pref_len, str_view.length());
    str_view.remove_prefix(pref_len);

    constexpr size_t STR_BUF_SZ = 64u;
    char str_buf[STR_BUF_SZ] = "";
    std::vector<std::string> str_vec;

    std::vector<std::string_view> search_vec;
    search_vec.reserve(1024*1024);

    CHashTable hashtable;
    while (!str_view.empty())
    {
        pref_len = str_view.find_first_of(term_str);
        pref_len = std::min(pref_len, str_view.length());

        std::string_view cur_view = str_view.substr(0u, pref_len);

        size_t aligned_size = (pref_len + 15u)%16u;
        memset(str_buf, 0xFF, STR_BUF_SZ);
        strncpy(str_buf, cur_view.data(), 
                std::min(aligned_size, STR_BUF_SZ - 1u));
        str_vec.emplace_back(str_buf, STR_BUF_SZ);

        if (hashtable.find(str_vec.back()) == CHashTable::NULL_IDX)
        {
            search_vec.push_back(str_vec.back());
            hashtable.insert(str_vec.back());
            printf("inserted %.*s => %llu\n", 
                   cur_view.size(), cur_view.data(), 
                   hashtable.find(str_vec.back()));
        }
        else
        {
            printf("found before %.*s => %llu\n", 
                   cur_view.size(), cur_view.data());
        }
        str_view.remove_prefix(pref_len);

        pref_len = str_view.find_first_not_of(term_str);
        pref_len = std::min(pref_len, str_view.length());
        str_view.remove_prefix(pref_len);
    }

    srand(time(0));
    size_t mod = search_vec.size();
    size_t res = 0u;
    for (size_t i = 0u; i < 32*1024*1024; ++i)
    {
        size_t idx = rand()%mod;
        res += hashtable.find(search_vec[idx]);
    }
    hashtable.insert(search_vec[res%mod]);

    hashtable.dump_to_csv(fout);

    fclose(fout);
    fout = nullptr;

    munmap(str, fd_in_stat.st_size); 
    str = nullptr;

    close(fd_in);
    fd_in = -1;

    return 0;
}
