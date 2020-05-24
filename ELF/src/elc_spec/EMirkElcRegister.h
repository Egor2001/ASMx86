#ifndef ELC_SPEC_EMIRKELCREGISTER_H
#define ELC_SPEC_EMIRKELCREGISTER_H

#include <cstdint>

#define MIRK_ELC_REGISTER(REG_ENUM, REG_CODE, REG_NAME) \
    MIRK_ELC_REG_##REG_ENUM = REG_CODE,

enum EMirkElcRegister : uint32_t
{
    #include "ElcRegisters.h"

    MIRK_ELC_REGISTER_CNT
};

#undef MIRK_ELC_REGISTER

#endif //ELC_SPEC_EMIRKELCREGISTER_H
