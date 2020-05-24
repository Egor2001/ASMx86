#include "CBinGen.hpp"
#include <cstdio>

CBinGen::CBinGen():
    bin_size_{}, x86_size_{},
    bin_indx_vec_{}, x86_indx_vec_{},
    data_vec_{}
{
    bin_size_ = x86_size_ = 0u;
    bin_indx_vec_.push_back(bin_size_);
    x86_indx_vec_.push_back(x86_size_);
}

void CBinGen::translate_instr(std::vector<uint8_t>& text_vec)
{
    for (const auto& data : data_vec_)
    {
        size_t off = text_vec.size();
        text_vec.resize(text_vec.size() + data.byte_size(), 0u);

        data.translate(text_vec.data() + off);

        bin_size_ += text_vec.size() - off;
        bin_indx_vec_.push_back(bin_size_);
    }
}

void CBinGen::calculate_jumps()
{
    for (size_t idx = 0u; idx != data_vec_.size(); ++idx)
    {
        if ((data_vec_[idx].i_mask & SInstrData::MIRK_BIN_FLAG_JMP) && 
            (data_vec_[idx].i_mask & SInstrData::MIRK_BIN_DATA_DIS))
        {
            uint32_t x86_jmp_indx = 
                x86_indx_vec_[idx] + static_cast<int32_t>(data_vec_[idx].i_dis);
            uint32_t bin_jmp_indx = 0u;

            size_t lt = 0u, rt = x86_indx_vec_.size();
            while (lt + 1u < rt && x86_indx_vec_[lt] != x86_jmp_indx)
            {
                size_t mid = (lt + rt)/2u;
                printf("BIN %lu %lu %zu %zu\n", 
                        x86_indx_vec_[mid], x86_jmp_indx, lt, rt);
                if (x86_indx_vec_[mid] <= x86_jmp_indx) lt = mid;
                else                                    rt = mid;
            }

            if (x86_indx_vec_[lt] == x86_jmp_indx)
                bin_jmp_indx = bin_indx_vec_[lt];
            else
                return;

            data_vec_[idx].i_dis = bin_indx_vec_[idx + 1u] - bin_jmp_indx;
        }
    }
}

bool CBinGen::push_instr(const SMirkX86Instruction& instr, 
                         const UMirkX86Word* addr)
{
    bool result = true;

    size_t off = 0u;
    #define MIRK_X86_ARGTYPE(ARG_ENUM, ARG_CODE, ARG_NAME, ARG_SIZE) \
        case MIRK_X86_ARG_##ARG_ENUM: \
            off += ARG_SIZE; \
        break;

    SInstrArg dst = { .type = instr.dst, .addr = addr + 1u + off };
    switch (instr.dst)
    {
        #include "../x86_spec/X86ArgTypes.h"
        default: 
            off = 0u; result = false; 
        break;
    }

    SInstrArg src = { .type = instr.src, .addr = addr + 1u + off };
    switch (instr.dst)
    {
        #include "../x86_spec/X86ArgTypes.h"
        default: 
            off = 0u; result = false; 
        break;
    }

    #undef MIRK_X86_ARGTYPE

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

    x86_size_ += 1u + off;
    x86_indx_vec_.push_back(x86_size_);

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

#define YB_NEW_JUMP(DATA) { DATA.opt_jmp(); }
#define YB_NEW_DATA(DATA) { data_vec_.push_back(DATA); \
                            bin_size_ += data_vec_.back().byte_size(); \
                            bin_indx_vec_.push_back(bin_size_); }

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
