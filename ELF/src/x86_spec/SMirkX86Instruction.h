#ifndef X86_SPEC_SMIRK_X86INSTRUCTION
#define X86_SPEC_SMIRK_X86INSTRUCTION

#include "EMirkX86Command.h"
#include "EMirkX86ArgType.h"

struct SMirkX86Instruction
{
    EMirkX86Command cmd;
    EMirkX86ArgType dst, src;
};

#endif //X86_SPEC_SMIRK_X86INSTRUCTION
