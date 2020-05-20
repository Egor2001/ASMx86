#ifndef ELC_SPEC_SMIRKELCINSTRUCTION_H
#define ELC_SPEC_SMIRKELCINSTRUCTION_H

#include "EMirkElcCommand.h"
#include "EMirkElcArgType.h"

struct SMirkElcInstruction
{
    EMirkElcCommand cmd;
    EMirkElcArgType dst, src;
};

#endif //ELC_SPEC_SMIRKELCINSTRUCTION_H
