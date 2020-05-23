#include "SInstrData.h"

//---------------------------------------- 

#define MIRK_MAKE_MRM(MRM_MOD, MRM_REG, MRM_R_M) \
    (((MRM_MOD) << 6) | ((MRM_REG) << 3) | ((MRM_R_M) << 0))

#define MIRK_MAKE_SIB(SIB_SCALE, SIB_INDEX, SIB_BASE) \
    (((SIB_SCALE) << 6) | ((SIB_INDEX) << 3) | ((SIB_BASE) << 0))

#define MIRK_GET_MRM_MOD(MRM) (((MRM) >> 6) & 0x3)
#define MIRK_GET_MRM_REG(MRM) (((MRM) >> 3) & 0x7)
#define MIRK_GET_MRM_R_M(MRM) (((MRM) >> 0) & 0x7)

#define MIRK_GET_SIB_SCALE(SIB) (((SIB) >> 6) & 0x3)
#define MIRK_GET_SIB_INDEX(SIB) (((SIB) >> 3) & 0x7)
#define MIRK_GET_SIB_BASE(SIB)  (((SIB) >> 0) & 0x7)

#define MIRK_MRM_R_M_SIB 0b100
#define MIRK_SIB_NO_IDX 0b100
#define MIRK_SIB_NO_BASE 0b101

//---------------------------------------- 

size_t SInstrData::byte_size() const
{
    size_t result = 1u; //opc
    for (size_t off = 0u; off != 8u; ++off)
    {
        if ((i_mask >> off) & 0x1)
            result += FIELD_SIZE[off];
    }

    return result;
}

size_t SInstrData::translate(uint8_t* addr) const
{
    assert(addr);
    uint8_t* init = addr;

    if (i_mask & MIRK_BIN_PREF_REX) *addr++ = i_rex;
    if (i_mask & MIRK_BIN_PREF_0FH) *addr++ = 0x0F;

    *addr++ = i_opc;

    if (i_mask & MIRK_BIN_BYTE_MRM) *addr++ = i_mrm;
    if (i_mask & MIRK_BIN_BYTE_SIB) *addr++ = i_sib;

    if (i_mask & MIRK_BIN_DATA_DIS) 
    {
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&i_dis);
        memcpy(addr, &i_dis, sizeof(uint32_t));
        addr += sizeof(uint32_t);
    }

    if (i_mask & MIRK_BIN_DATA_IMM) 
    {
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&i_imm);
        memcpy(addr, &i_imm, sizeof(uint32_t));
        addr += sizeof(uint32_t);
    }

    return (addr - init);
}

//---------------------------------------- 

// set REX prefix [3 0] (+flag_rex)
uint8_t SInstrData::set_rex(uint8_t rex)
{
    i_mask |= MIRK_BIN_PREF_REX;
    i_rex = 0x40 + (rex & 0xF);

    return i_mask;
}

// set 0FH prefix [7 0] (+flag_0fh)
uint8_t SInstrData::set_0fh()
{
    i_mask |= MIRK_BIN_PREF_0FH;

    return i_mask;
}

// set OPCODE
uint8_t SInstrData::set_opc(uint8_t opc)
{
    i_opc = opc;

    return i_mask;
}

// add REG [2 0] to OPCODE
uint8_t SInstrData::add_reg_opc(uint8_t reg_opc)
{
    i_opc += reg_opc;

    return i_mask;
}

// set REG [5 3] of MOD_R_M (+flag_modrm)
uint8_t SInstrData::set_reg_ext(uint8_t reg_ext)
{
    i_mask |= MIRK_BIN_BYTE_MRM;
    i_mrm = MIRK_MAKE_MRM(MIRK_GET_MRM_MOD(i_mrm), reg_ext, 
                          MIRK_GET_MRM_R_M(i_mrm));

    return i_mask;
}

// set MOD [7 6] & R_M [2 0] of MOD_R_M (+flag_modrm)
uint8_t SInstrData::set_mod_r_m(uint8_t mod, uint8_t r_m)
{
    i_mask |= MIRK_BIN_BYTE_MRM;
    i_mrm = MIRK_MAKE_MRM(mod, MIRK_GET_MRM_REG(i_mrm), r_m);

    return i_mask;
}

// set MOD [7 6] & R_M [2 0] of MOD_R_M (+flag_modrm)
uint8_t SInstrData::set_mrm_d32()
{
    i_mask |= MIRK_BIN_BYTE_MRM;
    i_mrm = MIRK_MAKE_MRM(0b00, MIRK_GET_MRM_REG(i_mrm), 0b101);

    return i_mask;
}

// set MOD [7 6] & R_M [2 0] of MOD_R_M & SIB[7 0] (+flag_modrm) (+flag_sib)
uint8_t SInstrData::set_mrm_sib(uint8_t scale, uint8_t index, 
                                uint8_t base)
{
    i_mask |= MIRK_BIN_BYTE_MRM | MIRK_BIN_BYTE_SIB;
    i_mrm = MIRK_MAKE_MRM(0b00, MIRK_GET_MRM_REG(i_mrm), 0b100);
    i_sib = MIRK_MAKE_SIB(scale, index, base);

    return i_mask;
}

// set DISPLACEMENT [31 0] (+flag_dis)
uint8_t SInstrData::set_dis(uint32_t dis)
{
    i_mask |= MIRK_BIN_DATA_DIS;
    i_dis = dis;

    return i_mask;
}

// set IMMEDIATE [31 0] (+flag_imm)
uint8_t SInstrData::set_imm(uint32_t imm)
{
    i_mask |= MIRK_BIN_DATA_IMM;
    i_imm = imm;

    return i_mask;
}

// set DISPLACEMENT [31 0] & IMMEDIATE [31 0] (+flag_dis) (+flag_imm)
uint8_t SInstrData::set_dis_imm(uint64_t val)
{
    i_mask |= MIRK_BIN_DATA_DIS | MIRK_BIN_DATA_IMM;
    i_dis = static_cast<uint32_t>(val);
    i_imm = static_cast<uint32_t>(val >> 32u);

    return i_mask;
}

//---------------------------------------- 

uint8_t SInstrData::opt_rex(uint8_t val)
{
    set_rex(val);
    return i_mask;
}

uint8_t SInstrData::opt_0fh()
{
    set_0fh();
    return i_mask;
}

uint8_t SInstrData::opt_opc(uint8_t val)
{
    set_opc(val);
    return i_mask;
}

uint8_t SInstrData::opt_ext(uint8_t val)
{
    set_reg_ext(val);
    return i_mask;
}

uint8_t SInstrData::opt_arg_add(const SInstrArg& arg)
{
    switch (arg.type)
    {
        case MIRK_X86_ARG_REG: 
            add_reg_opc(arg.addr[0].as_reg); 
            break;

        default: i_mask = 0u; break;
    }

    return i_mask;
}

uint8_t SInstrData::opt_arg_reg(const SInstrArg& arg)
{
    switch (arg.type)
    {
        case MIRK_X86_ARG_REG: 
            set_reg_ext(arg.addr[0].as_reg);
            break;

        default: i_mask = 0u; break;
    }

    return i_mask;
}

uint8_t SInstrData::opt_arg_r_m(const SInstrArg& arg)
{
    switch (arg.type)
    {
        case MIRK_X86_ARG_REG: 
            set_mod_r_m(0b11, arg.addr[0].as_reg);
            break;

        case MIRK_X86_ARG_MEM_IMM: 
            set_mrm_d32();
            set_dis(arg.addr[1].as_reg);
            break;

        case MIRK_X86_ARG_MEM_REG: 
            set_mod_r_m(0b00, arg.addr[0].as_reg);
            break;
            
        case MIRK_X86_ARG_MEM_REG_IMM: 
            set_mod_r_m(0b10, arg.addr[0].as_reg);
            set_dis(arg.addr[1].as_reg);
            break;

        case MIRK_X86_ARG_MEM_REG_REG: 
            set_mrm_sib(0b00, arg.addr[1].as_reg, arg.addr[0].as_reg);
            break;

        default: i_mask = 0u; break;
    }

    return i_mask;
}

uint8_t SInstrData::opt_arg_imm(const SInstrArg& arg)
{
    switch (arg.type)
    {
        case MIRK_X86_ARG_IMM: 
            set_imm(arg.addr[0].as_imm); 
            break;

        default: i_mask = 0u; break;
    }

    return i_mask;
}

uint8_t SInstrData::opt_arg_dis(const SInstrArg& arg)
{
    switch (arg.type)
    {
        case MIRK_X86_ARG_IMM: 
            set_dis(arg.addr[0].as_imm); 
            break;

        default: i_mask = 0u; break;
    }

    return i_mask;
}


//---------------------------------------- 

#undef MIRK_MAKE_SIB
#undef MIRK_MAKE_MRM

#undef MIRK_GET_MRM_MOD
#undef MIRK_GET_MRM_REG
#undef MIRK_GET_MRM_R_M

#undef MIRK_GET_SIB_SCALE
#undef MIRK_GET_SIB_INDEX
#undef MIRK_GET_SIB_BASE

#undef MIRK_MRM_R_M_SIB
#undef MIRK_SIB_NO_IDX
#undef MIRK_SIB_NO_BASE

//---------------------------------------- 
