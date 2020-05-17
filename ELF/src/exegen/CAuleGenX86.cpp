#ifndef DISASM_CGENELCX86
#define DISASM_CGENELCX86

#include "../elc_spec/SMirkElcInstruction.h"
#include "../x86_spec/SMirkX86Instruction.h"

class CGenElcX86
{
public:
    struct SX86Data
    {
        SMirkX86Instruction instr;
        const uint8_t* addr; 
        size_t len;
    };

    CGenElcX86();

    CGenElcX86             (const CGenElcX86&) = delete;
    CGenElcX86& operator = (const CGenElcX86&) = delete;
    CGenElcX86             (CGenElcX86&&) = default;
    CGenElcX86& operator = (CGenElcX86&&) = default;

    std::vector<SX86Data> generate_next(const SMirkElcInstruction& instr,
                                        const UWord* addr, size_t len);

protected:
#define MIRK_ELC_COMMAND(CMD_ENUM, CMD_CODE, CMD_NAME, CMD_DST, CMD_SRC) \
    int generate_cmd_##CMD_NAME(const SMirkElcInstruction& elc_instr, \
                                const UWord* addr);

    #include "../elc_spec/ElcCommands.h"

#undef MIRK_ELC_COMMAND

private:
    std::multimap<const UWord*, size_t> elc_label_map_;

    std::vector<uint8_t> x86_text_vec_;
};

#endif //DISASM_CGENELCX86
