#ifndef SRC_BINGEN_INSTR_DATA
#define SRC_BINGEN_INSTR_DATA

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>

#include "SInstrArg.h"

//#pragma pack(push, 1)

struct SInstrData
{
//mask fields
    enum EInstrMask
    {
        MIRK_BIN_PREF_REX = 0x01,
        MIRK_BIN_PREF_0FH = 0x02,
        MIRK_BIN_BYTE_MRM = 0x04,
        MIRK_BIN_BYTE_SIB = 0x08,
        MIRK_BIN_DATA_DIS = 0x10,
        MIRK_BIN_DATA_IMM = 0x20
    };

    static constexpr uint8_t FIELD_SIZE[8u] = { 1u, 1u, 1u, 1u, 4u, 4u };

//used fields mask;
    uint8_t i_mask;

//x86 instruction fields;
    uint8_t i_rex; 
    uint8_t i_opc; 
    uint8_t i_mrm, i_sib;
    uint32_t i_dis, i_imm;

//translation;
    size_t byte_size() const;
    size_t translate(uint8_t* addr) const;

//x86 fields setters;
    uint8_t set_rex    (uint8_t rex);
    uint8_t set_0fh    ();

    uint8_t set_opc    (uint8_t opc);

    uint8_t add_reg_opc(uint8_t reg_opc);
    uint8_t set_reg_ext(uint8_t reg_ext);

    uint8_t set_mod_r_m(uint8_t mod, uint8_t r_m);
    uint8_t set_mrm_d32();
    uint8_t set_mrm_sib(uint8_t scale, uint8_t index, uint8_t base);

    uint8_t set_dis    (uint32_t dis);
    uint8_t set_imm    (uint32_t imm);
    uint8_t set_dis_imm(uint64_t val);

//x86 opt setters
    uint8_t opt_rex(uint8_t val);
    uint8_t opt_0fh();
    uint8_t opt_opc(uint8_t val);
    uint8_t opt_ext(uint8_t val);

    uint8_t opt_arg_add(const SInstrArg& arg);
    uint8_t opt_arg_reg(const SInstrArg& arg);
    uint8_t opt_arg_r_m(const SInstrArg& arg);
    uint8_t opt_arg_dis(const SInstrArg& arg);
    uint8_t opt_arg_imm(const SInstrArg& arg);
};

//#pragma pack(pop, 1)

#endif //SRC_BINGEN_INSTR_DATA
