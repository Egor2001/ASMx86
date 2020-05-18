#include "CElcDisasm.hpp"

CElcDisasm::CElcDisasm(size_t word_cnt, const UMirkElcWord* text_buf):
    word_idx_{}, word_cnt_{ word_cnt },
    text_buf_{ text_buf }
{}

CElcDisasm::SElcData CElcDisasm::decode_next()
{
    SElcData elc_data = {
        .instr = {},
        .addr = text_buf_ + word_idx_,
        .len = 0u
    };

    if (word_idx_ >= word_cnt_)
        return elc_data;

    int err = decode_instr(&elc_data.instr, &elc_data.len);
    assert(!err);

    word_idx_ += elc_data.len;

    return elc_data;
}

//decode full instruction
int CElcDisasm::decode_instr(SMirkElcInstruction* elc_instr, 
                             size_t* instr_len) noexcept
{
    assert(elc_instr);
    assert(instr_len);
    int err = 0;

    err = decode_cmd(&(elc_instr->cmd));
    assert(!err);

    size_t dst_len = 0u, src_len = 0u;
    err = decode_arg(&(elc_instr->dst), &(elc_instr->src),
                     &dst_len, &src_len);
    assert(!err);

    *instr_len = 1u + dst_len + src_len;

    return err;
}

//decode command info
int CElcDisasm::decode_cmd(EMirkElcCommand* elc_cmd) noexcept
{
    assert(elc_cmd);
    int err = 0;

    *elc_cmd = text_buf_[word_idx_].as_instr.cmd;

    return err;
}

//decode argtype info
int CElcDisasm::decode_arg(EMirkElcArgType* elc_dst, EMirkElcArgType* elc_src,
                           size_t* dst_len, size_t* src_len) noexcept
{
    assert(elc_dst); assert(dst_len);
    assert(elc_src); assert(src_len);
    int err = 0;

    *elc_dst = text_buf_[word_idx_].as_instr.dst;
    *elc_src = text_buf_[word_idx_].as_instr.src;

    size_t len = 0u;

#define MIRK_ELC_ARGTYPE(ARG_ENUM, ARG_CODE, ARG_NAME, ARG_SIZE) \
    case MIRK_ELC_ARG_##ARG_ENUM: len = ARG_SIZE; break;

    switch (*elc_dst)
    {
        #include "../elc_spec/ElcArgTypes.h"
        default: err = 1u; return err;
    }
    *dst_len = len;

    switch (*elc_src)
    {
        #include "../elc_spec/ElcArgTypes.h"
        default: err = 2u; return err;
    }
    *src_len = len;

#undef MIRK_ELC_ARGTYPE

    return err;
}
