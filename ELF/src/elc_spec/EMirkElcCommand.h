#ifndef ELC_SPEC_EMIRKELCCOMMAND_H
#define ELC_SPEC_EMIRKELCCOMMAND_H

#include <cstdint>

#define MIRK_ELC_COMMAND(CMD_ENUM, CMD_CODE, CMD_NAME, CMD_DST, CMD_SRC) \
    MIRK_ELC_CMD_##CMD_ENUM = CMD_CODE,

enum EMirkElcCommand : uint16_t
{
    #include "ElcCommands.h"
};

#undef MIRK_ELC_COMMAND

#endif //ELC_SPEC_EMIRKELCCOMMAND_H
