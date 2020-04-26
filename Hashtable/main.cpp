#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cassert>

#include <vector>
#include <string>
#include <string_view>

#include "src/CHashTable.h"

int test_csv_file(int argc, char* argv[]);
int test_csv_mmap(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    //test_csv(argc, argv);
    test_csv_mmap(argc, argv);

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

        //printf("%zu\n", str_view.length());
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

    CHashTable hashtable;
    while (!str_view.empty())
    {
        pref_len = str_view.find_first_of(term_str);
        pref_len = std::min(pref_len, str_view.length());

        hashtable.insert(str_view.substr(0u, pref_len));
        str_view.remove_prefix(pref_len);

        pref_len = str_view.find_first_not_of(term_str);
        pref_len = std::min(pref_len, str_view.length());
        str_view.remove_prefix(pref_len);
    }

    hashtable.dump_to_csv(fout);

    fclose(fout);
    fout = nullptr;

    munmap(str, fd_in_stat.st_size); 
    str = nullptr;

    close(fd_in);
    fd_in = -1;

    return 0;
}
