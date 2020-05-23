#include "CGenElcX86.hpp"

CGenElcX86::CGenElcX86():
    label_map_{},
    text_vec_{}
{}

// public
void CGenElcX86::generate_next(std::vector<SX86Data>& cmd_vec,
                               const SMirkElcInstruction& instr,
                               const UMirkElcWord* addr)
{
#define MIRK_ELC_COMMAND(CMD_ENUM, CMD_CODE, CMD_NAME, CMD_DST, CMD_SRC) \
    case MIRK_ELC_CMD_##CMD_ENUM: \
        generate_cmd_##CMD_NAME(cmd_vec, instr.dst, instr.src, addr); \
    break;

    switch (instr.cmd)
    {
        #include "../elc_spec/ElcCommands.h"
        default: return;
    }

#undef MIRK_ELC_COMMAND
}

// protected static
CGenElcX86::SX86Data 
CGenElcX86::
make_x86(EMirkX86Command cmd, size_t offset,
         EMirkElcArgType dst, EMirkElcArgType src, 
         size_t* dst_len_ptr, size_t* src_len_ptr)
{ 
    SX86Data result = {
        .instr = SMirkX86Instruction { 
            .cmd = cmd, 
            .dst = MIRK_X86_ARG_NUL, .src = MIRK_X86_ARG_NUL
        },
        .word_idx = offset, 
        .dst_len = 0u, .src_len = 0u
    };

    size_t len = 0u;
    EMirkX86ArgType type = MIRK_X86_ARG_NUL;

#define MIRK_ELC_ARGTYPE(ARG_ENUM, ARG_CODE, ARG_NAME, ARG_SIZE) \
    case MIRK_ELC_ARG_##ARG_ENUM: \
        len = ARG_SIZE; \
        type = MIRK_X86_ARG_##ARG_ENUM; \
    break;

    switch (dst)
    {
        #include "../elc_spec/ElcArgTypes.h"
        default: return {};
    }

    result.instr.dst = type;
    result.dst_len = len;
    if (dst_len_ptr) 
        *dst_len_ptr = len;

    switch (src)
    {
        #include "../elc_spec/ElcArgTypes.h"
        default: return {};
    }

    result.instr.src = type;
    result.src_len = len;
    if (src_len_ptr) 
        *src_len_ptr = len;

#undef MIRK_ELC_ARGTYPE

    return result;
}

EMirkX86ArgType
CGenElcX86::
make_x86_arg(EMirkElcArgType arg_type, const UMirkElcWord* elc_data)
{ 
    EMirkX86ArgType result = MIRK_X86_ARG_NUL;

#define MIRK_ELC_ARGTYPE(ARG_ENUM, ARG_CODE, ARG_NAME, ARG_SIZE) \
    case MIRK_ELC_ARG_##ARG_ENUM: \
        result = make_x86_arg_##ARG_NAME(elc_data); \
        result = MIRK_X86_ARG_##ARG_ENUM; \
    break;

    switch (arg_type)
    {
        #include "../elc_spec/ElcArgTypes.h"
        default: 
            return MIRK_X86_ARG_NUL;
    }

#undef MIRK_ELC_ARGTYPE

    return result;
}

EMirkX86ArgType
CGenElcX86::
make_x86_arg_nul(const UMirkElcWord* elc_data)
{
    return MIRK_X86_ARG_NUL;
}

EMirkX86ArgType
CGenElcX86::
make_x86_arg_imm(const UMirkElcWord* elc_data)
{
    text_vec_.push_back(UMirkX86Word{ elc_data[0u].as_imm });
    return MIRK_X86_ARG_IMM;
}

EMirkX86ArgType
CGenElcX86::
make_x86_arg_flt(const UMirkElcWord* elc_data)
{
    text_vec_.push_back(UMirkX86Word{ elc_data[0u].as_imm });
    return MIRK_X86_ARG_IMM;
}

EMirkX86ArgType
CGenElcX86::
make_x86_arg_lbl(const UMirkElcWord* elc_data)
{
    text_vec_.push_back(UMirkX86Word{ elc_data[0u].as_imm });
    return MIRK_X86_ARG_IMM;
}

EMirkX86ArgType
CGenElcX86::
make_x86_arg_reg(const UMirkElcWord* elc_data)
{
    text_vec_.push_back(UMirkX86Word{ MIRK_REG_MAP[elc_data[0u].as_reg] });
    return MIRK_X86_ARG_REG;
}

EMirkX86ArgType
CGenElcX86::
make_x86_arg_mem_imm(const UMirkElcWord* elc_data)
{
    text_vec_.push_back(UMirkX86Word{ elc_data[0u].as_imm });
    return MIRK_X86_ARG_MEM_IMM;
}

EMirkX86ArgType
CGenElcX86::
make_x86_arg_mem_reg(const UMirkElcWord* elc_data)
{
    text_vec_.push_back(UMirkX86Word{ MIRK_REG_MAP[elc_data[0u].as_reg] });
    return MIRK_X86_ARG_MEM_REG;
}

EMirkX86ArgType
CGenElcX86::
make_x86_arg_mem_reg_imm(const UMirkElcWord* elc_data)
{
    text_vec_.push_back(UMirkX86Word{ MIRK_REG_MAP[elc_data[0u].as_reg] });
    text_vec_.push_back(UMirkX86Word{ elc_data[0u].as_imm });
    return MIRK_X86_ARG_MEM_REG_IMM;
}

EMirkX86ArgType
CGenElcX86::
make_x86_arg_mem_reg_reg(const UMirkElcWord* elc_data)
{
    text_vec_.push_back(UMirkX86Word{ MIRK_REG_MAP[elc_data[0u].as_reg] });
    text_vec_.push_back(UMirkX86Word{ MIRK_REG_MAP[elc_data[1u].as_reg] });
    return MIRK_X86_ARG_MEM_REG_REG;
}

// [addr here and then is the first of dst]
// protected here and then

void 
CGenElcX86::
generate_x86(std::vector<SX86Data>& cmd_vec, EMirkX86Command cmd,
             EMirkX86ArgType dst, EMirkX86ArgType src,
             const UMirkX86Word* addr)
{
    SX86Data data = {
        .instr = SMirkX86Instruction { 
            .cmd = cmd, 
            .dst = dst, .src = src
        },
        .word_idx = text_vec_.size(), 
        .dst_len = 0u, .src_len = 0u
    };

    size_t len = 0u;

#define MIRK_X86_ARGTYPE(ARG_ENUM, ARG_CODE, ARG_NAME, ARG_SIZE) \
    case MIRK_X86_ARG_##ARG_ENUM: len = ARG_SIZE; break;

    switch (dst)
    {
        #include "../x86_spec/X86ArgTypes.h"
        default: return;
    }
    data.dst_len = len;

    switch (src)
    {
        #include "../x86_spec/X86ArgTypes.h"
        default: return;
    }
    data.src_len = len;

#undef MIRK_X86_ARGTYPE

    cmd_vec.push_back(data);
    text_vec_.push_back(UMirkX86Word{ cmd_vec.back().instr });

    text_vec_.insert(text_vec_.end(), addr, addr + data.dst_len + data.src_len);
}

#define MIRK_GENERATE_SIMPLE(CMD_ENUM, CMD_NAME) \
    void CGenElcX86:: \
    generate_cmd_##CMD_NAME(std::vector<SX86Data>& cmd_vec, \
                            EMirkElcArgType dst, EMirkElcArgType src, \
                            const UMirkElcWord* addr) \
    { \
        size_t dst_len = 0u, src_len = 0u; \
        cmd_vec.push_back(make_x86(MIRK_X86_CMD_##CMD_ENUM, text_vec_.size(), \
                                   dst, src, &dst_len, &src_len)); \
        text_vec_.push_back(UMirkX86Word{ cmd_vec.back().instr }); \
        \
        make_x86_arg(dst, addr + 1u); \
        make_x86_arg(src, addr + 1u + dst_len); \
    }

//----------------------------------------
//<DATA-CONTROL>
MIRK_GENERATE_SIMPLE(PUSH, push)
MIRK_GENERATE_SIMPLE(POP, pop)
MIRK_GENERATE_SIMPLE(MOV, mov)
//<\DATA-CONTROL>
//----------------------------------------

//----------------------------------------
//<ARITHMETIC>
MIRK_GENERATE_SIMPLE(INC, inc)
MIRK_GENERATE_SIMPLE(DEC, dec)

MIRK_GENERATE_SIMPLE(ADD, add)
MIRK_GENERATE_SIMPLE(SUB, sub)
MIRK_GENERATE_SIMPLE(MUL, mul)
MIRK_GENERATE_SIMPLE(DIV, div)
//<\ARITHMETIC>
//----------------------------------------

//----------------------------------------
//<BOOLEAN>
MIRK_GENERATE_SIMPLE(CMP, cmp)

MIRK_GENERATE_SIMPLE(OR,  or)
MIRK_GENERATE_SIMPLE(AND, and)
MIRK_GENERATE_SIMPLE(XOR, xor)
//<\BOOLEAN>
//----------------------------------------

//----------------------------------------
//<CONTROL-FLOW>
MIRK_GENERATE_SIMPLE(JMP,   jmp)
MIRK_GENERATE_SIMPLE(CALLF, call)
MIRK_GENERATE_SIMPLE(RETF,  ret)

MIRK_GENERATE_SIMPLE(JZ,  jz)
MIRK_GENERATE_SIMPLE(JZ,  je)
MIRK_GENERATE_SIMPLE(JNZ, jnz)
MIRK_GENERATE_SIMPLE(JNZ, jne)
MIRK_GENERATE_SIMPLE(JG,  jg)
MIRK_GENERATE_SIMPLE(JGE, jge)
MIRK_GENERATE_SIMPLE(JB,  jl)
MIRK_GENERATE_SIMPLE(JBE, jle)

//<\CONTROL-FLOW>
//----------------------------------------

#undef MIRK_GENERATE_SIMPLE

//----------------------------------------
//<MANUALLY-IMPL>

#define MIRK_GEN_X86(CMD_VEC, CMD_ENUM, DST_TYPE, SRC_TYPE, ADDR) \
    generate_x86(CMD_VEC, MIRK_X86_CMD_##CMD_ENUM, \
                 MIRK_X86_ARG_##DST_TYPE, MIRK_X86_ARG_##SRC_TYPE, ADDR)

void 
CGenElcX86::
generate_cmd_hlt(std::vector<SX86Data>& cmd_vec,
                 EMirkElcArgType dst_type, EMirkElcArgType src_type,
                 const UMirkElcWord* addr)
{
    static constexpr UMirkX86Word st_args[4] = { 
        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RAX }, // 0x0 dst
            UMirkX86Word{ 0x3C },                           // 0x1 src

        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RDI }, // 0x2 dst
            UMirkX86Word{ 0x00 },                           // 0x3 src
    };

    //STORE
    MIRK_GEN_X86(cmd_vec, PUSH, REG, NUL, st_args + 0x0); // push RAX
    MIRK_GEN_X86(cmd_vec, PUSH, REG, NUL, st_args + 0x2); // push RDI

    //PASS
    MIRK_GEN_X86(cmd_vec, MOV, REG, IMM, st_args + 0x0); // mov RAX, 0x3C
    MIRK_GEN_X86(cmd_vec, MOV, REG, IMM, st_args + 0x2); // mov RDI, 0x00

    //SYSCALL
    MIRK_GEN_X86(cmd_vec, SYSCALL, NUL, NUL, st_args + 0x4); // syscall

    //LOAD
    MIRK_GEN_X86(cmd_vec, POP, REG, NUL, st_args + 0x2); // push RDI
    MIRK_GEN_X86(cmd_vec, POP, REG, NUL, st_args + 0x0); // push RAX
}

void 
CGenElcX86::
generate_cmd_in(std::vector<SX86Data>& cmd_vec,
                EMirkElcArgType dst_type, EMirkElcArgType src_type,
                const UMirkElcWord* addr)
{
    static constexpr UMirkX86Word st_args[12] = { 
        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RAX },     // 0x0 dst
            UMirkX86Word{ 0x00 },                               // 0x1 src

        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RDI },     // 0x2 dst
            UMirkX86Word{ 0x00 },                               // 0x3 src

        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RSI },     // 0x4 dst
            UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RSP }, // 0x5 src

        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RDX },     // 0x6 dst
            UMirkX86Word{ 0x08 },                               // 0x7 src

        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RSI },     // 0x8 dst
            UMirkX86Word{ 0x20 },                               // 0x9 src

        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RSP },     // 0xA dst
            UMirkX86Word{ 0x8 },                                // 0xB src
    };

    //STORE
    MIRK_GEN_X86(cmd_vec, PUSH, REG, NUL, st_args + 0x0); // push RAX
    MIRK_GEN_X86(cmd_vec, PUSH, REG, NUL, st_args + 0x2); // push RDI
    MIRK_GEN_X86(cmd_vec, PUSH, REG, NUL, st_args + 0x4); // push RSI
    MIRK_GEN_X86(cmd_vec, PUSH, REG, NUL, st_args + 0x6); // push RDX

    //PASS
    MIRK_GEN_X86(cmd_vec, MOV, REG, IMM, st_args + 0x0); // mov RAX, 0x0
    MIRK_GEN_X86(cmd_vec, MOV, REG, IMM, st_args + 0x2); // mov RDI, 0x0
    MIRK_GEN_X86(cmd_vec, MOV, REG, IMM, st_args + 0x6); // mov RDX, 0x8
                               
    MIRK_GEN_X86(cmd_vec, MOV, REG, MEM_REG, st_args + 0x4); // mov RSI, RSP
    MIRK_GEN_X86(cmd_vec, ADD, REG, IMM, st_args + 0x8); // add RSI, 0x20

    //SYSCALL
    MIRK_GEN_X86(cmd_vec, SYSCALL, NUL, NUL, st_args + 0xC); // syscall

    //LOAD
    MIRK_GEN_X86(cmd_vec, POP, REG, NUL, st_args + 0x6); // pop RDX
    MIRK_GEN_X86(cmd_vec, POP, REG, NUL, st_args + 0x4); // pop RSI
    MIRK_GEN_X86(cmd_vec, POP, REG, NUL, st_args + 0x2); // pop RDI
    MIRK_GEN_X86(cmd_vec, POP, REG, NUL, st_args + 0x0); // pop RAX
                               
    MIRK_GEN_X86(cmd_vec, ADD, REG, IMM, st_args + 0xA); // add RSP, 0x08
}

void
CGenElcX86::
generate_cmd_out(std::vector<SX86Data>& cmd_vec,
                 EMirkElcArgType dst_type, EMirkElcArgType src_type,
                 const UMirkElcWord* addr)
{
    static constexpr UMirkX86Word st_args[12] = { 
        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RAX },     // 0x0 dst
            UMirkX86Word{ 0x01 },                               // 0x1 src

        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RDI },     // 0x2 dst
            UMirkX86Word{ 0x01 },                               // 0x3 src

        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RSI },     // 0x4 dst
            UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RSP }, // 0x5 src

        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RDX },     // 0x6 dst
            UMirkX86Word{ 0x08 },                               // 0x7 src

        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RSI },     // 0x8 dst
            UMirkX86Word{ 0x20 },                               // 0x9 src

        UMirkX86Word{ EMirkX86Register::MIRK_X86_REG_RSP },     // 0xA dst
            UMirkX86Word{ 0x8 },                                // 0xB src
    };

    //STORE
    MIRK_GEN_X86(cmd_vec, PUSH, REG, NUL, st_args + 0x0); // push RAX
    MIRK_GEN_X86(cmd_vec, PUSH, REG, NUL, st_args + 0x2); // push RDI
    MIRK_GEN_X86(cmd_vec, PUSH, REG, NUL, st_args + 0x4); // push RSI
    MIRK_GEN_X86(cmd_vec, PUSH, REG, NUL, st_args + 0x6); // push RDX

    //PASS
    MIRK_GEN_X86(cmd_vec, MOV, REG, IMM, st_args + 0x0); // mov RAX, 0x0
    MIRK_GEN_X86(cmd_vec, MOV, REG, IMM, st_args + 0x2); // mov RDI, 0x0
    MIRK_GEN_X86(cmd_vec, MOV, REG, IMM, st_args + 0x6); // mov RDX, 0x8
                               
    MIRK_GEN_X86(cmd_vec, MOV, REG, MEM_REG, st_args + 0x4); // mov RSI, RSP
    MIRK_GEN_X86(cmd_vec, ADD, REG, IMM, st_args + 0x8); // add RSI, 0x20

    //SYSCALL
    MIRK_GEN_X86(cmd_vec, SYSCALL, NUL, NUL, st_args + 0xC); // syscall

    //LOAD
    MIRK_GEN_X86(cmd_vec, POP, REG, NUL, st_args + 0x6); // pop RDX
    MIRK_GEN_X86(cmd_vec, POP, REG, NUL, st_args + 0x4); // pop RSI
    MIRK_GEN_X86(cmd_vec, POP, REG, NUL, st_args + 0x2); // pop RDI
    MIRK_GEN_X86(cmd_vec, POP, REG, NUL, st_args + 0x0); // pop RAX
                               
    MIRK_GEN_X86(cmd_vec, ADD, REG, IMM, st_args + 0xA); // add RSP, 0x08
}

#undef MIRK_GEN_X86

//<\MANUALLY-IMPL>
//----------------------------------------
