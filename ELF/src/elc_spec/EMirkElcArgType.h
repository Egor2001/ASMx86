#ifndef ELC_SPEC_EMIRKELCARGTYPE_H
#define ELC_SPEC_EMIRKELCARGTYPE_H

#include <cstdint>

#define MIRK_ELC_ARGTYPE(ARG_ENUM, ARG_CODE, ARG_NAME, ARG_SIZE) \
    MIRK_ELC_ARG_##ARG_ENUM = ARG_CODE,

enum EMirkElcArgType : uint8_t
{
    #include "ElcArgTypes.h"
};

#undef MIRK_ELC_ARGTYPE

#endif //ELC_SPEC_EMIRKELCARGTYPE_H
