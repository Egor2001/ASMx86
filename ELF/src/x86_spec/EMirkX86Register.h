#ifndef X86_SPEC_EMIRKX86REGISTER_H
#define X86_SPEC_EMIRKX86REGISTER_H

#include <cstdint>

#define MIRK_X86_REGISTER(REG_ENUM, REG_CODE, REG_NAME) \
    MIRK_X86_REG_##REG_ENUM = REG_CODE,

enum EMirkX86Register : uint32_t
{
    #include "X86Registers.h"

    MIRK_X86_REGISTER_CNT
};

#undef MIRK_X86_REGISTER

#endif //X86_SPEC_EMIRKX86REGISTER_H
