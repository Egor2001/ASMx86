#include <sys/stat.h>

#include "src/frontend/CYaggParser.hpp"
#include "src/backend/CYaggBuilder.hpp"

class CYagg
{
public:
    CYagg() = default;

    CYagg             (const CYagg&) = delete;
    CYagg& operator = (const CYagg&) = delete;
    CYagg             (CYagg&&) = delete;
    CYagg& operator = (CYagg&&) = delete;

    bool frontend(FILE* fin)
    {
        assert(fin);

        bool result = true;

        struct stat file_stat = {};
        fstat(fileno(fin), &file_stat);

        size_t file_size = file_stat.st_size;
        char* text_buf = new char[file_size + 1u]{};
        std::string_view text{ text_buf, file_size };

        size_t read_size = fread(text_buf, sizeof (char), file_size, fin);
        assert(file_size == read_size);
        
        CYaggParser parser{ text };
        while (!parser.text().empty())
        {
            try 
            {
                instr_vec_.emplace_back();
                parser.parse_instr(&instr_vec_.back());
            }
            catch (const CYaggException& exc)
            {
                instr_vec_.pop_back();
                result = false;

                const auto& where = exc.where();
                fprintf(stderr, "error: %s\n", exc.what());
                fprintf(stderr, "at %zu \"%.*s...\"\n", 
                        text.size() - where.size(), 
                        static_cast<int>(std::min(where.size(), 16ul)), 
                        where.data());

                break;
            }
        }

        delete [] text_buf;
        text_buf = nullptr;

        return result;
    }

    bool backend(FILE* fout)
    {
        assert(fout);

        bool result = true;

        CYaggBuilder builder{ fout };

        for (const auto& instr : instr_vec_)
        {
            if (!builder.build_instr(instr, 0u))
            {
                result = false;
                break;
            }
        }

        return result;
    }

private:
    std::vector<SYaggInstr> instr_vec_;
};

int main(int argc, const char* argv[])
{
    CYagg yagg;
    yagg.frontend(stdin);
    yagg.backend(stdout);
/*
    test_CYaggParser(argc, argv);
    test_CYaggBuilder(argc, argv);
*/
    return 0;
}
