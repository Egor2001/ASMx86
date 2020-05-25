#include <cstdlib>
#include <cstring>
#include <cassert>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "mirk/CMirkContext.hpp"

#include "src/disasm/CElcDisasm.hpp"
#include "src/exegen/CGenElcX86.hpp"
#include "src/bingen/CBinGen.hpp"

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

    std::vector<CElcDisasm::SElcData> 
    test_disasm() const
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
        {
            printf("Elc data: ");
            dump_elc_data(data);
            printf("[LEN %zu]\n", data.len);
        }

        return std::move(result);
    }

    std::vector<CGenElcX86::SX86Data> 
    test_exegen(std::vector<UMirkX86Word>& text_vec) const
    {
        auto elc_data_vec = test_disasm();

        CGenElcX86 exegen;
        std::vector<CGenElcX86::SX86Data> result;

        for (const auto& elc_data : elc_data_vec)
            exegen.generate_next(result, elc_data.instr, elc_data.addr);

        exegen.calculate_jumps();

        printf("X86 cnt: %zu\n", result.size());
        for (const auto& data : result)
        {
            printf("X86 data: ");
            dump_x86_data(data);

            size_t word_idx = data.word_idx + 1u;
            for (size_t idx = 0u; idx != data.dst_len; ++idx)
                printf("%#x ", exegen.text_vec()[word_idx + idx]);

            word_idx += data.dst_len;
            for (size_t idx = 0u; idx != data.src_len; ++idx)
                printf("%#x ", exegen.text_vec()[word_idx + idx]);

            printf("[IDX %#x] [LEN %zu]\n", 
                    data.word_idx, data.dst_len + data.src_len);
        }

        text_vec = exegen.text_vec();
        return std::move(result);
    }

    std::vector<uint8_t> 
    test_bingen() const
    {
        std::vector<UMirkX86Word> text_vec;
        auto x86_data_vec = test_exegen(text_vec);

        CBinGen bingen;
        for (const auto& data : x86_data_vec)
            bingen.push_instr(data.instr, text_vec.data() + data.word_idx);

        std::vector<uint8_t> result;
        bingen.calculate_jumps();
        bingen.translate_instr(result);

        printf("BIN cnt: %zu\n", result.size());
        printf("BIN data: \n");
        for (uint8_t data : result)
            printf("%#hhx ", data);

        return std::move(result);
    }

    void
    test_elfgen() const
    {
        constexpr static size_t DEFAULT_DATA_SIZE = 0x100;
        constexpr static char DEFAULT_FILE_NAME[] = "elc.out";
        constexpr uint8_t x86_data_buf[DEFAULT_DATA_SIZE] = {};

        auto x86_text_vec = test_bingen();
        CMirkContext context(x86_text_vec.size(), x86_text_vec.data(), 
                             DEFAULT_DATA_SIZE, x86_data_buf);

        int fdout = open(DEFAULT_FILE_NAME, O_RDWR | O_CREAT, 0766);
        assert(fdout != -1);

        context.create(fdout);

        close(fdout);
        fdout = -1;
    }

    static void dump_elc_data(const CElcDisasm::SElcData& data)
    {
    #define MIRK_ELC_COMMAND(CMD_ENUM, CMD_CODE, CMD_NAME, CMD_DST, CMD_SRC) \
        case MIRK_ELC_CMD_##CMD_ENUM: \
            printf("[CMD %s] ", #CMD_NAME); \
        break;

        switch (data.instr.cmd)
        {
            #include "src/elc_spec/ElcCommands.h"
            default: printf("[CMD ?] ");
        }

    #undef MIRK_ELC_COMMAND

    #define MIRK_ELC_ARGTYPE(ARG_ENUM, ARG_CODE, ARG_NAME, ARG_SIZE) \
        case MIRK_ELC_ARG_##ARG_ENUM: \
            printf("[ARG %s] ", #ARG_NAME); \
        break;

        switch (data.instr.dst)
        {
            #include "src/elc_spec/ElcArgTypes.h"
            default: printf("[ARG ?] ");
        }

        switch (data.instr.src)
        {
            #include "src/elc_spec/ElcArgTypes.h"
            default: printf("[ARG ?] ");
        }

    #undef MIRK_ELC_ARGTYPE
    }

    static void dump_x86_data(const CGenElcX86::SX86Data& data)
    {
    #define MIRK_X86_COMMAND(CMD_ENUM, CMD_CODE, CMD_NAME) \
        case MIRK_X86_CMD_##CMD_ENUM: \
            printf("[CMD %s] ", #CMD_NAME); \
        break;

        switch (data.instr.cmd)
        {
            #include "src/x86_spec/X86Commands.h"
            default: printf("[CMD ?] ");
        }

    #undef MIRK_X86_COMMAND

    #define MIRK_X86_ARGTYPE(ARG_ENUM, ARG_CODE, ARG_NAME, ARG_SIZE) \
        case MIRK_X86_ARG_##ARG_ENUM: \
            printf("[ARG %s] ", #ARG_NAME); \
        break;

        switch (data.instr.dst)
        {
            #include "src/x86_spec/X86ArgTypes.h"
            default: printf("[ARG ?] ");
        }

        switch (data.instr.src)
        {
            #include "src/x86_spec/X86ArgTypes.h"
            default: printf("[ARG ?] ");
        }

    #undef MIRK_X86_ARGTYPE
    }

private:
    size_t word_cnt_;
    UMirkElcWord* text_buf_;
};

int main(int argc, const char* argv[])
{
    https://habr.com/ru/company/huawei/blog/503194/

    //test_CMirkContext(argc, argv);

    assert(argc == 2);
    CMirkTest test(argv[1u]);

    //test.test_disasm();
    //test.test_exegen();
    //test.test_bingen();
    test.test_elfgen();


    return 0u;
}
