#ifndef SRC_BINGEN_INSTR_ARG
#define SRC_BINGEN_INSTR_ARG

#include "../x86_spec/EMirkX86ArgType.h"
#include "../x86_spec/UMirkX86Word.h"

#include "../../yagg/src/enums/EYaggArgType.h"

struct SInstrArg
{
    EMirkX86ArgType type;
    const UMirkX86Word* addr;

    EYaggArgType yagg_type() const noexcept
    {
        EYaggArgType result = YAGG_ARG_TYPE_NUL;

        switch (type)
        {
            case MIRK_X86_ARG_IMM:
            case MIRK_X86_ARG_FLT:
            case MIRK_X86_ARG_LBL:
                result = YAGG_ARG_TYPE_IMM;
            break;

            case MIRK_X86_ARG_REG:
                result = YAGG_ARG_TYPE_REG;
            break;

            case MIRK_X86_ARG_MEM_IMM:
            case MIRK_X86_ARG_MEM_REG:
            case MIRK_X86_ARG_MEM_REG_IMM:
            case MIRK_X86_ARG_MEM_REG_REG:
                result = YAGG_ARG_TYPE_MEM;
            break;

            case MIRK_X86_ARG_NUL:
            default:
                result = YAGG_ARG_TYPE_NUL;
            break;
        }

        return result;
    }
};

#endif //SRC_BINGEN_INSTR_ARG
