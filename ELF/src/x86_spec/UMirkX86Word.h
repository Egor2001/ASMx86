#ifndef UMIRKX86WORD_H
#define UMIRKX86WORD_H

#include <cstdlib>
#include <cstdint>

#include "SMirkX86Instruction.h"
#include "EMirkX86Register.h"

#pragma pack(push, 1)
union UMirkX86Word
{
    constexpr UMirkX86Word() {}
    constexpr UMirkX86Word(SMirkX86Instruction instr): as_instr(instr) {}
    constexpr UMirkX86Word(EMirkX86Register    reg):   as_reg(reg) {}
    constexpr UMirkX86Word(uint32_t            imm):   as_imm(imm) {}

    SMirkX86Instruction as_instr;
    EMirkX86Register    as_reg;
    uint32_t            as_imm = 0xffffffff;
};
#pragma pack(pop)

#endif // UMIRKX86WORD_H
