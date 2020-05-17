//Signature:
//MIRK_ELC_ARGTYPE(enum_name, code, name, size)

//Markdown:
//null     [0x00]       (NUL)
//reserved [0x01, 0x1F] ()
//unitary  [0x20, 0x3F] (IMM, REG, LBL, FLT)
//memory   [0x40, 0x7F] (MEM_IMM, MEM_REG, MEM_REG_IMM, MEM_REG_REG)
//reserved [0x80, 0xFE] ()
//error    [0xFF]       (ERROR_VALUE)

MIRK_ELC_ARGTYPE (NUL,         0x00, nul,         0)

MIRK_ELC_ARGTYPE (IMM,         0x20, imm,         1)
MIRK_ELC_ARGTYPE (REG,         0x21, reg,         1)
MIRK_ELC_ARGTYPE (LBL,         0x22, lbl,         1)
MIRK_ELC_ARGTYPE (FLT,         0x23, flt,         1)

MIRK_ELC_ARGTYPE (MEM_IMM,     0x40, mem_imm,     1)
MIRK_ELC_ARGTYPE (MEM_REG,     0x41, mem_reg,     1)
MIRK_ELC_ARGTYPE (MEM_REG_IMM, 0x42, mem_reg_imm, 2)
MIRK_ELC_ARGTYPE (MEM_REG_REG, 0x43, mem_reg_reg, 2)

//Will be defined in the end of EArgument
//MIRK_ELC_ARGTYPE (ERR_VALUE, 0xFF, error_value, 0)
