#ifndef UMIRKELCWORD_H
#define UMIRKELCWORD_H

#include <cstdlib>
#include <cstdint>

#include "SMirkElcInstruction.h"
#include "EMirkElcRegister.h"

#pragma pack(push, 1)
union UMirkElcWord
{
    UMirkElcWord() = default;
    UMirkElcWord(SMirkElcInstruction instr): as_instr(instr) {}
    UMirkElcWord(EMirkElcRegister    reg):   as_reg(reg) {}
    UMirkElcWord(uint32_t            imm):   as_imm(imm) {}

    SMirkElcInstruction as_instr;
    EMirkElcRegister    as_reg;
    uint32_t            as_imm = 0xffffffff;
};
#pragma pack(pop)

#endif // UMIRKELCWORD_H
