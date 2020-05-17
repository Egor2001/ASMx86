#include "CNamoDisElc.hpp"

CNamoDisElc::CNamoDisElc(size_t word_cnt, const UWord* text_buf):
    word_idx_{}, word_cnt_{ word_idx_ },
    text_buf_{ text_buf }
{}

CNamoDisElc::SElcData CNamoDisElc::decode_next()
{
    SElcData elc_data = {
        .instr = {},
        .addr = text_buf_ + word_idx,
        .len = 0u
    };

    int err = decode_instr(&elc_data.instr, &elc_data.len);
    assert(!err);

    word_idx_ += elc_data.len;

    return elc_data;
}

//decode full instruction
int CNamoDisElc::decode_instr(SMirkElcInstruction* elc_instr, 
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
int CNamoDisElc::decode_cmd(EMirkElcCommand* elc_cmd) noexcept
{
    assert(elc_cmd);
    int err = 0;

    *elc_cmd = static_cast<uint16_t>(text_buf_[word_idx_]);

    return err;
}

//decode argtype info
int CNamoDisElc::decode_arg(EMirkElcArgType* elc_dst, EMirkElcArgType* elc_src,
                            size_t* dst_len, size_t* src_len) noexcept
{
    assert(elc_dst); assert(dst_len);
    assert(elc_src); assert(src_len);
    int err = 0;

    *elc_dst = static_cast<uint8_t>(text_buf_[word_idx_] >> 16u);
    *elc_src = static_cast<uint8_t>(text_buf_[word_idx_] >> 24u);

    size_t len = 0u;

#define MIRK_ELC_ARGTYPE(ARG_ENUM, ARG_NAME, ARG_SIZE) \
    case ARG_ENUM: len = ARG_SIZE; break;

    switch (*elc_dst)
    {
        #include "ElcArgTypes.h"
        default: err = 1u; return err;
    }
    *dst_len = len;

    switch (*elc_src)
    {
        #include "ElcArgTypes.h"
        default: err = 2u; return err;
    }
    *src_len = len;

#undef MIRK_ELC_ARGTYPE

    return err;
}
