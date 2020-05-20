#include "CBinGen.hpp"
/*
    size_t word_cnt_;
    const UMirkX86Word* word_buf_;
    std::vector<uint8_t> text_vec_;
*/

// public
CBinGen::CBinGen(size_t word_cnt, const UMirkX86Word* word_buf):
    word_cnt_{ word_cnt },
    word_buf_{ word_buf },
    text_vec_{}
{}

CBinGen::~CBinGen()
{
    word_buf_ = nullptr;
    word_cnt_ = 0u;
}

void CBinGen::translate_next(const SMirkX86Instruction& instr, 
                             const UMirkX86Word* addr)
{
#define MIRK_X86_COMMAND(CMD_ENUM, CMD_CODE, CMD_NAME) \
    case MIRK_X86_CMD_##CMD_ENUM: \
        translate_cmd_##CMD_NAME(instr.dst, instr.src, addr); \
    break;

    switch (instr.cmd)
    {
        #include "../x86_spec/X86Commands.h"
        default: 
            return;
    }

#undef MIRK_X86_COMMAND
}

// protected
void CBinGen::translate_x86(const SInstrData& data, uint8_t mask,
                            uint8_t dis_cnt, uint8_t imm_cnt)
{
    if (mask & MIRK_BIN_PREF_REX) text_vec_.push_back(data.rex);
    if (mask & MIRK_BIN_PREF_0FH) text_vec_.push_back(0x0F);

    text_vec_.push_back(data.cmd);

    if (mask & MIRK_BIN_BYTE_MRM) text_vec_.push_back(data.mrm);
    if (mask & MIRK_BIN_BYTE_SIB) text_vec_.push_back(data.sib);

    if (mask & MIRK_BIN_DATA_DIS) 
    {
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&data.dis);
        text_vec_.insert(text_vec_.end(), ptr, ptr + dis_cnt);
    }

    if (mask & MIRK_BIN_DATA_IMM) 
    {
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&data.imm);
        text_vec_.insert(text_vec_.end(), ptr, ptr + imm_cnt);
    }
}

#define MIRK_MAKE_MRM(MRM_MOD, MRM_REG, MRM_R_M) \
    (((MRM_MOD) << 6) | ((MRM_REG) << 3) | ((MRM_R_M) << 0))

#define MIRK_MAKE_SIB(SIB_SCALE, SIB_INDEX, SIB_BASE) \
    (((SIB_SCALE) << 6) | ((SIB_INDEX) << 3) | ((SIB_BASE) << 0))

#define MIRK_MRM_R_M_SIB
#define MIRK_SIB_NO_IDX
#define MIRK_SIB_NO_BASE

//---------------------------------------- 

// set REX prefix [3 0] (+flag_rex)
uint8_t CBinGen::instr_set_rex(SInstrData* data, 
                               uint8_t rex)
{
    assert(data);
    data->mask |= MIRK_BIN_PREF_REX;
    data->rex = 0x40 + (rex & 0xF);

    return data->mask;
}

// set 0FH prefix [7 0] (+flag_0fh)
uint8_t CBinGen::instr_set_0fh(SInstrData* data)
{
    assert(data);
    data->mask |= MIRK_BIN_PREF_0FH;

    return data->mask;
}

// set OPCODE
uint8_t CBinGen::instr_set_opc(SInstrData* data, uint8_t opc)
{
    assert(data);
    data->opc = opc;

    return data->mask;
}

// add REG [2 0] to OPCODE
uint8_t CBinGen::instr_add_reg_opc(SInstrData* data, 
                                   uint8_t reg_opc)
{
    assert(data);
    data->opc += reg_opc;

    return data->mask;
}

// set REG [5 3] of MOD_R_M (+flag_modrm)
uint8_t CBinGen::instr_set_reg_ext(SInstrData* data, 
                                   uint8_t reg_ext)
{
    assert(data);
    data->mask |= MIRK_BIN_BYTE_MRM;
    data->mrm = MIRK_MAKE_MRM(MIRK_GET_MRM_MOD(data->mrm), reg_ext, 
                              MIRK_GET_MRM_R_M(data->mrm));

    return data->mask;
}

// set MOD [7 6] & R_M [2 0] of MOD_R_M (+flag_modrm)
uint8_t CBinGen::instr_set_mod_r_m(SInstrData* data,
                                   uint8_t mod, uint8_t r_m)
{
    assert(data);
    data->mask |= MIRK_BIN_BYTE_MRM;
    data->mrm = MIRK_MAKE_MRM(mod, MIRK_GET_MRM_REG(data->mrm), r_m);

    return data->mask;
}

// set MOD [7 6] & R_M [2 0] of MOD_R_M (+flag_modrm)
uint8_t CBinGen::instr_set_mrm_d32(SInstrData* data)
{
    assert(data);
    data->mask |= MIRK_BIN_BYTE_MRM;
    data->mrm = MIRK_MAKE_MRM(0b00, MIRK_GET_MRM_REG(data->mrm), 0b101);

    return data->mask;
}

// set MOD [7 6] & R_M [2 0] of MOD_R_M & SIB[7 0] (+flag_modrm) (+flag_sib)
uint8_t CBinGen::instr_set_mrm_sib(SInstrData* data, 
                                   uint8_t scale, uint8_t index, uint8_t base)
{
    assert(data);
    data->mask |= MIRK_BIN_BYTE_MRM | MIRK_BIN_BYTE_SIB;
    data->mrm = MIRK_MAKE_MRM(0b00, MIRK_GET_MRM_REG(data->mrm), 0b100);
    data->sib = MIRK_MAKE_SIB(scale, index, base);

    return data->mask;
}

// set DISPLACEMENT [31 0] (+flag_dis)
uint8_t CBinGen::instr_set_dis(SInstrData* data, 
                               uint32_t dis)
{
    assert(data);
    data->mask |= MIRK_BIN_DATA_DIS;
    data->dis = dis;

    return data->mask;
}

// set IMMEDIATE [31 0] (+flag_imm)
uint8_t CBinGen::instr_set_imm(SInstrData* data, 
                               uint32_t imm)
{
    assert(data);
    data->mask |= MIRK_BIN_DATA_IMM;
    data->imm = imm;

    return data->mask;
}

// set DISPLACEMENT [31 0] & IMMEDIATE [31 0] (+flag_dis) (+flag_imm)
uint8_t CBinGen::instr_set_imm_dis(SInstrData* data, 
                                   uint64_t val)
{
    assert(data);
    data->mask |= MIRK_BIN_DATA_DIS | MIRK_BIN_DATA_IMM;
    data->dis = static_cast<uint32_t>(val);
    data->imm = static_cast<uint32_t>(val >> 32u);

    return data->mask;
}

//---------------------------------------- 
/*
SInstrData CBinGen::make_instr_i32(uint8_t opc, uint32_t imm)
SInstrData CBinGen::make_instr_i64(uint8_t opc, uint64_t imm)
SInstrData CBinGen::make_instr_reg(uint8_t opc, )
SInstrData CBinGen::make_instr_mem(uint8_t opc, )
SInstrData CBinGen::make_instr_reg_imm(uint8_t opc, )
SInstrData CBinGen::make_instr_reg_mem(uint8_t opc, )
SInstrData CBinGen::make_instr_mem_imm(uint8_t opc, )
SInstrData CBinGen::make_instr_mem_reg(uint8_t opc, )

void 
CBinGen::
translate_cmd_add(EMirkX86ArgType dst, const UMirkX86Word* dst_addr, 
                  EMirkX86ArgType src, const UMirkX86Word* src_addr)
{
    uint8_t mask = 0u;
    SInstrData data = {};

    switch (dst)
    {
        case MIRK_X86_ARG_REG:
            
            break;

        case MIRK_X86_ARG_MEM_IMM:
        case MIRK_X86_ARG_MEM_REG:
        case MIRK_X86_ARG_MEM_REG_IMM:
        case MIRK_X86_ARG_MEM_REG_REG:

            break;

        default:
            throw;
            break;
    }

    if (src_data.type == MIRK_BIN_REG)
    {
        mask |= MIRK_BIN_PREF_REX;
        data.cmd = 0x01; // add r/m64 to r64
        data.rex = 0x48; // Wxxx

        if (dst_data.type == MIRK_BIN_REG)
        {
            mask |= MIRK_BIN_BYTE_MRM;
            data.mrm = MIRK_MAKE_MRM(0b11, src_data.as_reg, dst_data.as_reg);
        }
        else if (dst_data.type == MIRK_BIN_MEM)
        {
            if (dst == MIRK_X86_ARG_MEM_IMM) 
            {
                mask |= MIRK_BIN_BYTE_MRM 
                      | MIRK_BIN_DATA_IMM;

                data.mrm = MIRK_MAKE_MRM(0b00, src_data.lhs, 0b101);
                data.imm = dst_data.lhs;
            }
            else if (dst == MIRK_X86_ARG_MEM_REG)
            {
                mask |= MIRK_BIN_BYTE_MRM;

                data.mrm = MIRK_MAKE_MRM(0b00, src_data.lhs, dst_data.lhs);
            }
            else if (dst == MIRK_X86_ARG_MEM_REG_IMM) 
            {
                mask |= MIRK_BIN_BYTE_MRM 
                      | MIRK_BIN_BYTE_SIB 
                      | MIRK_BIN_BYTE_IMM;

                data.mrm = MIRK_MAKE_MRM(0b00, src_data.lhs, 0b101);
                data.sib = MIRK_MAKE_SIB(0b00, src_data.lhs, dst_data.lhs);
                data.imm = dst_data.rhs;
            }
            else if (dst == MIRK_X86_ARG_MEM_REG_REG)
            {
                mask |= MIRK_BIN_BYTE_MRM 
                      | MIRK_BIN_BYTE_SIB 
                      | MIRK_BIN_BYTE_IMM;

                data.mrm = MIRK_MAKE_MRM(0b00, src_data.lhs, dst_data.lhs);
            }
            else
            {
            }
        }
        else
        {
        }
    }
    else if (src_data.type == MIRK_BIN_MEM)
    {
        mask |= MIRK_BIN_PREF_REX;
        data.rex = 0x48; // Wxxx
        data.cmd = 0x03; // add r64 to r/m64

        if (dst_data.type == MIRK_BIN_REG) 
        {
            mask |= MIRK_BIN_BYTE_MRM;
            data.mrm = 
        }
        else
        {
        }
    }
    else if (src_data.type == MIRK_BIN_IMM)
    {
        mask |= MIRK_BIN_PREF_REX;
        data.rex = 0x48; // Wxxx
        data.cmd = 0x81; // add r64 to r/m64

        if (dst_data.type == MIRK_BIN_MEM) 
        {
            mask |= MIRK_BIN_BYTE_MRM;
            data.mrm = 
        }
        else if (dst_data.type == MIRK_BIN_REG)
        {
            mask |= MIRK_BIN_BYTE_MRM;
            data.mrm = 
        }
    }
}
*/
#undef MIRK_MAKE_SIB
#undef MIRK_MAKE_MRM
