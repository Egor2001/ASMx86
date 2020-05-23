#include "CBinGen.hpp"

void CBinGen::translate_instr(std::vector<uint8_t>& text_vec)
{
    for (const auto& data : data_vec_)
    {
        size_t off = text_vec.size();
        text_vec.resize(text_vec.size() + data.byte_size(), 0u);

        data.translate(text_vec.data() + off);
    }
}

void CBinGen::calculate_jumps()
{
}

bool CBinGen::push_instr(const SMirkX86Instruction& instr, 
                         const UMirkX86Word* addr)
{
    bool result = true;

    size_t off = 0u;
    switch (instr.dst)
    {
    #define MIRK_X86_ARGTYPE(ARG_ENUM, ARG_CODE, ARG_NAME, ARG_SIZE) \
        case MIRK_X86_ARG_##ARG_ENUM: \
            off = ARG_SIZE; \
        break;

        #include "../x86_spec/X86ArgTypes.h"
        default: 
            off = 0u; 
            result = false; 
        break;

    #undef MIRK_X86_ARGTYPE
    }

    SInstrArg dst = { .type = instr.dst, .addr = addr };
    SInstrArg src = { .type = instr.src, .addr = addr + off };

    switch (instr.cmd)
    {
    #define MIRK_X86_COMMAND(CMD_ENUM, CMD_CODE, CMD_NAME) \
        case MIRK_X86_CMD_##CMD_ENUM: \
            result = push_x86_##CMD_NAME(dst, src); \
        break;

        #include "../x86_spec/X86Commands.h"
        default: 
            result = false; 
        break;

    #undef MIRK_X86_COMMAND
    }

    return result;
}

#define YB_INSTR_FNC(NAME) CBinGen::push_x86_##NAME

#define YB_ARG_TYPE SInstrArg
#define YB_DATA_TYPE SInstrData

#define YB_TYPE_FNC(ARG) ARG.yagg_type()

#define YB_DATA_SET_REX(DATA, VAL)      DATA.opt_rex(VAL)
#define YB_DATA_SET_0FH(DATA)           DATA.opt_0fh()
#define YB_DATA_SET_OPC(DATA, VAL)      DATA.opt_opc(VAL)
#define YB_DATA_SET_EXT(DATA, VAL)      DATA.opt_ext(VAL)
#define YB_DATA_SET_ARG(DATA, TGT, ARG) DATA.opt_arg_##TGT(ARG)

#define YB_NEW_JUMP(DATA) { jump_vec_.push_back(data_vec_.size()); }
#define YB_NEW_DATA(DATA) { data_vec_.push_back(DATA); \
                            size_ += data_vec_.back().byte_size(); \
                            addr_vec_.push_back(size_); }

//#include "../yagg/macro_check.h"
#include "../gen/x86_layout.h"
//#include "../yagg/macro_undef.h"

#undef YB_INSTR_FNC

#undef YB_ARG_TYPE
#undef YB_DATA_TYPE

#undef YB_TYPE_FNC

#undef YB_DATA_SET_REX
#undef YB_DATA_SET_0FH
#undef YB_DATA_SET_OPC
#undef YB_DATA_SET_EXT
#undef YB_DATA_SET_ARG

#undef YB_NEW_JUMP
#undef YB_NEW_DATA
