#ifndef X86_SPEC_EMIRKX86ARGTYPE_H
#define X86_SPEC_EMIRKX86ARGTYPE_H

#include <cstdint>

#define MIRK_X86_ARGTYPE(ARG_TYPE, ARG_CODE, ARG_NAME, ARG_SIZE) \
    MIRK_X86_ARG_##ARG_TYPE = ARG_CODE,

enum EMirkX86ArgType : uint8_t
{
    #include "X86ArgTypes.h"
};

#undef MIRK_X86_ARGTYPE

#endif //X86_SPEC_EMIRKX86ARGTYPE_H
