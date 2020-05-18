#include <cstdlib>
#include <cstring>
#include <cassert>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "src/elfgen/context.hpp"
#include "src/disasm/CElcDisasm.hpp"
#include "src/exegen/CGenElcX86.hpp"

class CMirkTest
{
public:
    explicit CMirkTest(const char filename[]):
        word_cnt_(0u),
        text_buf_(nullptr)
    {
        assert(filename);

        int fd = open(filename, O_RDONLY);
        assert(fd != -1);

        struct stat statbuf = {};
        int err = fstat(fd, &statbuf);
        assert(!err);

        word_cnt_ = statbuf.st_size / sizeof(UMirkElcWord);
        text_buf_ = (UMirkElcWord*) (calloc(word_cnt_, sizeof(UMirkElcWord)));
        assert(text_buf_);

        ssize_t cnt = read(fd, text_buf_, word_cnt_*sizeof(UMirkElcWord));
        assert(cnt == word_cnt_*sizeof(UMirkElcWord));

        close(fd);
        fd = -1;
    }

    CMirkTest             (const CMirkTest&) = delete;
    CMirkTest& operator = (const CMirkTest&) = delete;
    CMirkTest             (CMirkTest&&) = delete;
    CMirkTest& operator = (CMirkTest&&) = delete;

    ~CMirkTest()
    {
        free(text_buf_);
        text_buf_ = nullptr;
        word_cnt_ = 0u;
    }

    std::vector<CElcDisasm::SElcData> test_disasm() const
    {
        CElcDisasm disasm(word_cnt_, text_buf_);
        std::vector<CElcDisasm::SElcData> result;

        size_t len = 1u;
        while (len)
        {
            auto data = disasm.decode_next();
            len = data.len;

            result.push_back(data);
        }

        printf("Elc cnt: %zu\n", result.size());
        for (const auto& data : result)
            printf("Elc data: [%x %p %zu]\n", 
                   data.instr, data.addr, data.len);

        return std::move(result);
    }

    std::vector<CGenElcX86::SX86Data> test_exegen() const
    {
        auto elc_data_vec = test_disasm();

        CGenElcX86 exegen;
        std::vector<CGenElcX86::SX86Data> result;

        for (const auto& elc_data : elc_data_vec)
            exegen.generate_next(result, elc_data.instr, elc_data.addr);

        printf("X86 cnt: %zu\n", result.size());
        for (const auto& data : result)
            printf("X86 data: [%x %zu %zu %zu]\n", 
                   data.instr, data.word_idx, data.dst_len, data.src_len);

        return std::move(result);
    }

private:
    size_t word_cnt_;
    UMirkElcWord* text_buf_;
};

int main(int argc, char* argv[])
{
    //test_CMirkContext(argc, argv);
    assert(argc == 2);
    CMirkTest test(argv[1u]);

    //test.test_disasm();
    test.test_exegen();

    return 0u;
}
