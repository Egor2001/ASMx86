#ifndef SRC_BINGEN_CBINGEN_HPP
#define SRC_BINGEN_CBINGEN_HPP

#include <vector>

#include "../x86_spec/SMirkX86Instruction.h"
#include "../x86_spec/UMirkX86Word.h"

class CBinGen
{
public:
    enum EInstrMask
    {
        MIRK_BIN_PREF_REX = 0x01,
        MIRK_BIN_PREF_0FH = 0x02,
        MIRK_BIN_BYTE_MRM = 0x04,
        MIRK_BIN_BYTE_SIB = 0x08,
        MIRK_BIN_DATA_DIS = 0x10,
        MIRK_BIN_DATA_IMM = 0x20
    };

#pragma pack(push, 1)

    struct SInstrData
    {
        uint8_t mask;

        uint8_t rex; 
        uint8_t opc; 
        uint8_t mrm, sib;
        uint32_t dis, imm;
    };

#pragma pack(pop, 1)

    CBinGen(size_t word_cnt, const UMirkX86Word* word_buf);

    CBinGen             (const CBinGen&) = delete;
    CBinGen& operator = (const CBinGen&) = delete;
    CBinGen             (CBinGen&&) = delete;
    CBinGen& operator = (CBinGen&&) = delete;

    ~CBinGen();

    void translate_next(const SMirkX86Instruction& instr, 
                        const UMirkX86Word* addr);

    const std::vector<uint8_t>& text_vec() const noexcept { return text_vec_; }

protected:
    uint8_t instr_set_rex    (uint8_t mask, SInstrData* data, uint8_t rex);
    uint8_t instr_set_0fh    (uint8_t mask, SInstrData* data);

    uint8_t instr_set_opc    (uint8_t mask, SInstrData* data, uint8_t opc);

    uint8_t instr_add_reg_opc(uint8_t mask, SInstrData* data, uint8_t reg_opc);
    uint8_t instr_set_reg_ext(uint8_t mask, SInstrData* data, uint8_t reg_ext);

    uint8_t instr_set_mod_r_m(uint8_t mask, SInstrData* data,
                              uint8_t mod, uint8_t r_m);
    uint8_t instr_set_mrm_d32(uint8_t mask, SInstrData* data);
    uint8_t instr_set_mrm_sib(uint8_t mask, SInstrData* data, 
                              uint8_t scale, uint8_t index, uint8_t base);

    uint8_t instr_set_dis    (uint8_t mask, SInstrData* data, uint32_t dis);
    uint8_t instr_set_imm    (uint8_t mask, SInstrData* data, uint32_t imm);
    uint8_t instr_set_dis_imm(uint8_t mask, SInstrData* data, uint64_t val);

    void translate_x86(const SInstrData& data, uint8_t mask,
                       uint8_t dis_cnt, uint8_t imm_cnt);

#define MIRK_X86_COMMAND(CMD_ENUM, CMD_CODE, CMD_NAME) \
    void translate_cmd_##CMD_NAME(EMirkX86ArgType dst, EMirkX86ArgType src, \
                                  const UMirkX86Word* addr);

    #include "../x86_spec/X86Commands.h"

#undef MIRK_X86_COMMAND

private:
    size_t word_cnt_ = 0u;
    const uint8_t* word_buf_ = nullptr;

    std::vector<uint8_t> text_vec_;
};

#endif //SRC_BINGEN_CBINGEN_HPP
