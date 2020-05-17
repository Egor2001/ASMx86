//Signature:
//MIRK_ELC_COMMAND(enum_name, code, name, dst, src)

//Markdown:
//halt           [0x0000]         (HLT)
//reserved       [0x0001, 0x000F] ()
//interaction    [0x0010, 0x001F] (IN, OUT, DUMP, OK)
//stack          [0x0020, 0x003F] (PUSH, POP, DUP)
//memory         [0x0040, 0x005F] (MOV)
//control-flow   [0x0060, 0x007F] (CALL, RET, LOOP, JMP, JZ, JNZ, JE, JNE, JG, JGE, JL, JLE)
//integer        [0x0080, 0x00BF] (ADD, SUB, MUL, DIV, MOD, INC, DEC, AND, OR, XOR, INV, CMP)
//floating-point [0x00C0, 0x00FF] (FADD, FSUB, FMUL, FDIV, FTOI, ITOF, FSIN, FCOS, FSQRT, FCMP)
//reserved       [0x0100, 0xFFFE] ()
//error          [0xFFFF]         (ERROR_VALUE)

//halt
MIRK_ELC_COMMAND (HLT,  0x0000, hlt,  nul_arg, nul_arg)

//interaction
MIRK_ELC_COMMAND (IN,   0x0010, in,   nul_arg, nul_arg)
MIRK_ELC_COMMAND (OUT,  0x0011, out,  nul_arg, nul_arg)
MIRK_ELC_COMMAND (OK,   0x0012, ok,   nul_arg, nul_arg)
MIRK_ELC_COMMAND (DUMP, 0x0013, dump, nul_arg, nul_arg)

//stack
MIRK_ELC_COMMAND (PUSH, 0x0020, push, all_arg, nul_arg)
MIRK_ELC_COMMAND (POP,  0x0021, pop,  mov_arg, nul_arg)
MIRK_ELC_COMMAND (DUP,  0x0022, dup,  nul_arg, nul_arg)

//memory
MIRK_ELC_COMMAND (MOV, 0x0040, mov, mov_arg, all_arg)

//control-flow
MIRK_ELC_COMMAND (CALL, 0x0060, call, all_arg, nul_arg)
MIRK_ELC_COMMAND (RET,  0x0061, ret,  nul_arg, nul_arg)
MIRK_ELC_COMMAND (LOOP, 0x0062, loop, lbl_arg, nul_arg)

MIRK_ELC_COMMAND (JMP, 0x0070, jmp, all_arg, nul_arg)
MIRK_ELC_COMMAND (JZ,  0x0071, jz,  all_arg, nul_arg)
MIRK_ELC_COMMAND (JNZ, 0x0072, jnz, all_arg, nul_arg)
MIRK_ELC_COMMAND (JE,  0x0073, je,  all_arg, nul_arg)
MIRK_ELC_COMMAND (JNE, 0x0074, jne, all_arg, nul_arg)
MIRK_ELC_COMMAND (JG,  0x0075, jg,  all_arg, nul_arg)
MIRK_ELC_COMMAND (JGE, 0x0076, jge, all_arg, nul_arg)
MIRK_ELC_COMMAND (JL,  0x0077, jl,  all_arg, nul_arg)
MIRK_ELC_COMMAND (JLE, 0x0078, jle, all_arg, nul_arg)

//integer
MIRK_ELC_COMMAND (ADD, 0x0080, add, mov_arg, all_arg)
MIRK_ELC_COMMAND (SUB, 0x0081, sub, mov_arg, all_arg)
MIRK_ELC_COMMAND (MUL, 0x0082, mul, mov_arg, all_arg)
MIRK_ELC_COMMAND (DIV, 0x0083, div, mov_arg, all_arg)
MIRK_ELC_COMMAND (MOD, 0x0084, mod, mov_arg, all_arg)

MIRK_ELC_COMMAND (INC, 0x0085, inc, mov_arg, nul_arg)
MIRK_ELC_COMMAND (DEC, 0x0086, dec, mov_arg, nul_arg)

MIRK_ELC_COMMAND (AND, 0x00A0, and, mov_arg, all_arg)
MIRK_ELC_COMMAND (OR,  0x00A1, or,  mov_arg, all_arg)
MIRK_ELC_COMMAND (XOR, 0x00A2, xor, mov_arg, all_arg)
MIRK_ELC_COMMAND (INV, 0x00A3, inv, mov_arg, nul_arg)

MIRK_ELC_COMMAND (CMP, 0x00B0, cmp, all_arg, all_arg)

//TODO: carry out to FPU
//TODO: replace NO_PARAM with PARAM
//floating-point
MIRK_ELC_COMMAND (FADD, 0x00C0, fadd, mov_arg, all_arg)
MIRK_ELC_COMMAND (FSUB, 0x00C1, fsub, mov_arg, all_arg)
MIRK_ELC_COMMAND (FMUL, 0x00C2, fmul, mov_arg, all_arg)
MIRK_ELC_COMMAND (FDIV, 0x00C3, fdiv, mov_arg, all_arg)

MIRK_ELC_COMMAND (FTOI, 0x00C4, ftoi, mov_arg, nul_arg)
MIRK_ELC_COMMAND (ITOF, 0x00C5, itof, mov_arg, nul_arg)

MIRK_ELC_COMMAND (FSIN,  0x00D0, fsin,  mov_arg, nul_arg)
MIRK_ELC_COMMAND (FCOS,  0x00D1, fcos,  mov_arg, nul_arg)
MIRK_ELC_COMMAND (FSQRT, 0x00D2, fsqrt, mov_arg, nul_arg)

MIRK_ELC_COMMAND (FCMP, 0x00E0, fcmp, all_arg, all_arg)

//Will be defined in the end of EArgument
//error
//HANDLE_ARGUMENT_ (CMD_ERROR_VALUE, 0xFFFF, error_value, nul_arg, nul_arg)
