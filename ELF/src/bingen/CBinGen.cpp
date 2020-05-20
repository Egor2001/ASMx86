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
void CBinGen::translate_x86(const SInstrData& data)
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
uint8_t CBinGen::SInstrData::set_rex(uint8_t rex)
{
    assert(data);
    i_mask |= MIRK_BIN_PREF_REX;
    i_rex = 0x40 + (rex & 0xF);

    return i_mask;
}

// set 0FH prefix [7 0] (+flag_0fh)
uint8_t CBinGen::SInstrData::set_0fh()
{
    assert(data);
    i_mask |= MIRK_BIN_PREF_0FH;

    return i_mask;
}

// set OPCODE
uint8_t CBinGen::SInstrData::set_opc(uint8_t opc)
{
    assert(data);
    i_opc = opc;

    return i_mask;
}

// add REG [2 0] to OPCODE
uint8_t CBinGen::SInstrData::add_reg_opc(uint8_t reg_opc)
{
    assert(data);
    i_opc += reg_opc;

    return i_mask;
}

// set REG [5 3] of MOD_R_M (+flag_modrm)
uint8_t CBinGen::SInstrData::set_reg_ext(uint8_t reg_ext)
{
    assert(data);
    i_mask |= MIRK_BIN_BYTE_MRM;
    i_mrm = MIRK_MAKE_MRM(MIRK_GET_MRM_MOD(i_mrm), reg_ext, 
                              MIRK_GET_MRM_R_M(i_mrm));

    return i_mask;
}

// set MOD [7 6] & R_M [2 0] of MOD_R_M (+flag_modrm)
uint8_t CBinGen::SInstrData::set_mod_r_m(uint8_t mod, uint8_t r_m)
{
    assert(data);
    i_mask |= MIRK_BIN_BYTE_MRM;
    i_mrm = MIRK_MAKE_MRM(mod, MIRK_GET_MRM_REG(i_mrm), r_m);

    return i_mask;
}

// set MOD [7 6] & R_M [2 0] of MOD_R_M (+flag_modrm)
uint8_t CBinGen::SInstrData::set_mrm_d32()
{
    assert(data);
    i_mask |= MIRK_BIN_BYTE_MRM;
    i_mrm = MIRK_MAKE_MRM(0b00, MIRK_GET_MRM_REG(i_mrm), 0b101);

    return i_mask;
}

// set MOD [7 6] & R_M [2 0] of MOD_R_M & SIB[7 0] (+flag_modrm) (+flag_sib)
uint8_t CBinGen::SInstrData::set_mrm_sib(uint8_t scale, uint8_t index, 
                                         uint8_t base)
{
    assert(data);
    i_mask |= MIRK_BIN_BYTE_MRM | MIRK_BIN_BYTE_SIB;
    i_mrm = MIRK_MAKE_MRM(0b00, MIRK_GET_MRM_REG(i_mrm), 0b100);
    i_sib = MIRK_MAKE_SIB(scale, index, base);

    return i_mask;
}

// set DISPLACEMENT [31 0] (+flag_dis)
uint8_t CBinGen::SInstrData::set_dis(uint32_t dis)
{
    assert(data);
    i_mask |= MIRK_BIN_DATA_DIS;
    i_dis = dis;

    return i_mask;
}

// set IMMEDIATE [31 0] (+flag_imm)
uint8_t CBinGen::SInstrData::set_imm(uint32_t imm)
{
    assert(data);
    i_mask |= MIRK_BIN_DATA_IMM;
    i_imm = imm;

    return i_mask;
}

// set DISPLACEMENT [31 0] & IMMEDIATE [31 0] (+flag_dis) (+flag_imm)
uint8_t CBinGen::SInstrData::set_imm_dis(uint64_t val)
{
    assert(data);
    i_mask |= MIRK_BIN_DATA_DIS | MIRK_BIN_DATA_IMM;
    i_dis = static_cast<uint32_t>(val);
    i_imm = static_cast<uint32_t>(val >> 32u);

    return i_mask;
}

//---------------------------------------- 

void CBinGen::SInstrData::arg_imm(const UMirkX86Word* addr)
{
    data.set_imm(addr[0].as_imm);
}

void CBinGen::SInstrData::arg_dis(const UMirkX86Word* addr)
{
    data.set_dis(addr[0].as_imm);
}

void CBinGen::SInstrData::arg_opc_reg(const UMirkX86Word* addr)
{
    data.add_reg_opc(addr[0].as_reg & 0x7);
}

void CBinGen::SInstrData::arg_reg_ext(const UMirkX86Word* addr)
{
    data.set_reg_ext(addr[0].as_imm & 0x7);
}

void CBinGen::SInstrData::arg_reg_reg(const UMirkX86Word* addr)
{
    data.set_reg_ext(addr[0].as_reg & 0x7);
}

void CBinGen::SInstrData::arg_r_m_reg(const UMirkX86Word* addr)
{
    data.set_mod_r_m(0b11, addr[0].as_reg & 0x7);
}

void CBinGen::SInstrData::arg_r_m_mem_imm(const UMirkX86Word* addr)
{
    data.set_mrm_d32();
    data.set_dis(src_addr[0].as_imm);
}

void CBinGen::SInstrData::arg_r_m_mem_reg(const UMirkX86Word* addr)
{
    data.set_mod_r_m(0b11, addr[0].as_reg & 0x7);
}

void CBinGen::SInstrData::arg_r_m_mem_reg_imm(const UMirkX86Word* addr)
{
    data.set_mod_r_m(0b10, addr[0].as_reg & 0x7);
    data.set_dis(addr[1].as_imm);
}

void CBinGen::SInstrData::arg_r_m_mem_reg_reg(const UMirkX86Word* addr)
{
    data.set_mrm_sib(0b00, addr[1].as_reg & 0x7, addr[0].as_reg & 0x7);
}

void 
CBinGen::
translate_cmd_add(EMirkX86ArgType dst, const UMirkX86Word* dst_addr, 
                  EMirkX86ArgType src, const UMirkX86Word* src_addr)
{
    SInstrData data = {};

    switch (dst)
    {
    // immediate
        case MIRK_X86_ARG_IMM: 
            data.set_rex(0b100);
            data.set_opc(0x05);
            data.arg_imm(dst_addr); 
        break;

    // register
        case MIRK_X86_ARG_REG: 
            data.set_rex(0b100);

            switch (src)
            {
            // immediate
                case MIRK_X86_ARG_IMM: 
                    data.set_opc(0x81);
                    data.arg_r_m_reg(dst_addr); 
                    data.arg_imm(src_addr); 
                break;

            // register
                case MIRK_X86_ARG_REG: 
                    data.set_opc(0x03);
                    data.arg_reg_reg(dst_addr); 
                    data.arg_r_m_reg(src_addr); 
                break;

            // memory
                case MIRK_X86_ARG_MEM_IMM: 
                    data.set_opc(0x03);
                    data.arg_reg_reg(dst_addr); 
                    data.arg_r_m_mem_imm(src_addr); 
                break;

                case MIRK_X86_ARG_MEM_REG: 
                    data.set_opc(0x03);
                    data.arg_reg_reg(dst_addr); 
                    data.arg_r_m_mem_reg(src_addr); 
                break;

                case MIRK_X86_ARG_MEM_REG_IMM: 
                    data.set_opc(0x03);
                    data.arg_reg_reg(dst_addr); 
                    data.arg_r_m_mem_reg_imm(src_addr); 
                break;

                case MIRK_X86_ARG_MEM_REG_REG: 
                    data.set_opc(0x03);
                    data.arg_reg_reg(dst_addr); 
                    data.arg_r_m_mem_reg_reg(src_addr); 
                break;

            // error
                default:
                    throw;
                break;
            }
        break;

    // memory
        case MIRK_X86_ARG_MEM_IMM: 
        {
            data.set_(dst_addr[0].as_reg & 0x7);
        }
        goto DST_ARG_MEM_;

        case MIRK_X86_ARG_MEM_REG: 
        {
            data.set_reg_ext(dst_addr[0].as_reg & 0x7);
        }
        goto DST_ARG_MEM_;

        case MIRK_X86_ARG_MEM_REG_IMM: 
        {
            data.set_reg_ext(dst_addr[0].as_reg & 0x7);
        }
        goto DST_ARG_MEM_;

        case MIRK_X86_ARG_MEM_REG_REG: 
            data.set_opc(0x03);
            data.arg_reg_reg(dst_addr); 
            data.arg_r_m_mem_reg_reg(src_addr); 
        goto DST_ARG_MEM_;

        DST_ARG_MEM_:
        {
            switch (src)
            {
            // register
                case MIRK_X86_ARG_REG: 
                {
                    data.set_rex(0b100);
                }
                break;

            // error
                default:
                {
                    throw;
                }
                break;
            }

            data.set_rex(0b100);
        }
        break;

    // error
        default:
            throw;
        break;
    }
}

#undef MIRK_MAKE_SIB
#undef MIRK_MAKE_MRM
