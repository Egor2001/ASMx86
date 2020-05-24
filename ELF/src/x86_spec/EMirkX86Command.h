#ifndef X86_SPEC_EMIRKX86COMMAND_H
#define X86_SPEC_EMIRKX86COMMAND_H

#include <cstdint>

#define MIRK_X86_COMMAND(CMD_ENUM, CMD_CODE, CMD_NAME) \
    MIRK_X86_CMD_##CMD_ENUM = CMD_CODE,

enum EMirkX86Command : uint16_t
{
    #include "X86Commands.h"
};

#undef MIRK_X86_COMMAND

#endif //X86_SPEC_EMIRKX86COMMAND_H
