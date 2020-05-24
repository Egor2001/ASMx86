#ifndef DISASM_CGENELCX86
#define DISASM_CGENELCX86

#include <vector>

#include "../elc_spec/EMirkElcCommand.h"
#include "../elc_spec/SMirkElcInstruction.h"
#include "../elc_spec/UMirkElcWord.h"
#include "../x86_spec/EMirkX86Command.h"
#include "../x86_spec/SMirkX86Instruction.h"
#include "../x86_spec/UMirkX86Word.h"

class CGenElcX86
{
public:
    constexpr static EMirkX86Register MIRK_REG_MAP[MIRK_ELC_REGISTER_CNT] = {
        [MIRK_ELC_REG_AX] = MIRK_X86_REG_RAX,
        [MIRK_ELC_REG_BX] = MIRK_X86_REG_RBX,
        [MIRK_ELC_REG_CX] = MIRK_X86_REG_RCX,
        [MIRK_ELC_REG_DX] = MIRK_X86_REG_RDX,
        /*
        [MIRK_ELC_REG_R1] = MIRK_X86_REG_R8,
        [MIRK_ELC_REG_R2] = MIRK_X86_REG_R9,
        [MIRK_ELC_REG_R3] = MIRK_X86_REG_R10,
        [MIRK_ELC_REG_R4] = MIRK_X86_REG_R11,
        */
    };

    struct SX86Data
    {
        SMirkX86Instruction instr;
        size_t word_idx; 
        size_t dst_len, src_len;
    };

    CGenElcX86();

    CGenElcX86             (const CGenElcX86&) = delete;
    CGenElcX86& operator = (const CGenElcX86&) = delete;
    CGenElcX86             (CGenElcX86&&) = default;
    CGenElcX86& operator = (CGenElcX86&&) = default;

    void generate_next(std::vector<SX86Data>& cmd_vec,
                       const SMirkElcInstruction& instr, 
                       const UMirkElcWord* addr);

    void calculate_jumps();

    const std::vector<UMirkX86Word>& text_vec() const noexcept 
    { return text_vec_; }

protected:
    static SX86Data make_x86(EMirkX86Command cmd, size_t offset,
                             EMirkElcArgType dst, EMirkElcArgType src, 
                             size_t* dst_len_ptr = nullptr, 
                             size_t* src_len_ptr = nullptr);

    EMirkX86ArgType make_x86_arg(EMirkElcArgType arg_type, 
                                 const UMirkElcWord* elc_data);

#define MIRK_ELC_ARGTYPE(ARG_ENUM, ARG_CODE, ARG_NAME, ARG_SIZE) \
    EMirkX86ArgType make_x86_arg_##ARG_NAME(const UMirkElcWord* addr);

    #include "../elc_spec/ElcArgTypes.h"

#undef MIRK_ELC_ARGTYPE

    void generate_x86(std::vector<SX86Data>& cmd_vec, EMirkX86Command cmd,
                      EMirkX86ArgType dst, EMirkX86ArgType src,
                      const UMirkX86Word* addr);

#define MIRK_ELC_COMMAND(CMD_ENUM, CMD_CODE, CMD_NAME, CMD_DST, CMD_SRC) \
    void generate_cmd_##CMD_NAME(std::vector<SX86Data>& cmd_vec, \
                                 EMirkElcArgType dst, EMirkElcArgType src, \
                                 const UMirkElcWord* addr);

    #include "../elc_spec/ElcCommands.h"

#undef MIRK_ELC_COMMAND

private:
    std::vector<UMirkX86Word> text_vec_;
    std::vector<uint32_t> addr_vec_;
};

#endif //DISASM_CGENELCX86

