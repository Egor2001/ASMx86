#ifndef SRC_BINGEN_BINGEN
#define SRC_BINGEN_BINGEN

#include <vector>

#include "SInstrArg.h"
#include "SInstrData.h"
#include "../x86_spec/SMirkX86Instruction.h"
#include "../x86_spec/UMirkX86Word.h"

class CBinGen
{
public:
    CBinGen() = default;

    CBinGen             (const CBinGen&) = delete;
    CBinGen& operator = (const CBinGen&) = delete;
    CBinGen             (CBinGen&&) = delete;
    CBinGen& operator = (CBinGen&&) = delete;

    bool push_instr(const SMirkX86Instruction& instr, 
                    const UMirkX86Word* addr);

    void translate_instr(std::vector<uint8_t>& text_vec);
    void calculate_jumps();

protected:
#define MIRK_X86_COMMAND(CMD_ENUM, CMD_CODE, CMD_NAME) \
    bool push_x86_##CMD_NAME(const SInstrArg& dst, const SInstrArg& src);

    #include "../x86_spec/X86Commands.h"

#undef MIRK_X86_COMMAND

private:
    size_t size_ = 0u;

    std::vector<SInstrData> data_vec_;
    std::vector<size_t> addr_vec_;

    std::vector<size_t> jump_vec_;
};

#endif //SRC_BINGEN_CBINGEN_HPP
