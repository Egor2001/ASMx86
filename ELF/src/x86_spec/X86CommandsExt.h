// http://ref.x86asm.net/coder64.html

MIRK_X86_COMMAND (PUSH,    0x50,   push, )
MIRK_X86_COMMAND (POP,     0x58,   pop,  )

MIRK_X86_COMMAND (MOV,     0xb8,   mov,  )
MIRK_X86_COMMAND (LEA,     0x8d04, lea,  )
                                         
MIRK_X86_COMMAND (INC,     0xffc0, inc,  )
MIRK_X86_COMMAND (DEC,     0xffc8, dec,  )
                                        
MIRK_X86_COMMAND (NOT,     0xf7d0, not,  )
MIRK_X86_COMMAND (NEG,     0xf7d8, neg,  )
MIRK_X86_COMMAND (MUL,     0xf7e0, mul,  )
MIRK_X86_COMMAND (DIV,     0xf7f0, div,  )

MIRK_X86_COMMAND (ADD,     0x83c0, add,  )
MIRK_X86_COMMAND (OR,      0x83c8, or,   )
MIRK_X86_COMMAND (AND,     0x83e0, and,  )
MIRK_X86_COMMAND (SUB,     0x83e8, sub,  )
MIRK_X86_COMMAND (XOR,     0x83f0, xor,  )
MIRK_X86_COMMAND (CMP,     0x83f8, cmp,  )
                                       
MIRK_X86_COMMAND (JMP,     0xe9,   jmp    )
MIRK_X86_COMMAND (JZ,      0x0f84, jz     )
MIRK_X86_COMMAND (JNZ,     0x0f85, jnz    )
MIRK_X86_COMMAND (JG,      0x0f8f, jg     )
MIRK_X86_COMMAND (JB,      0x0f82, jb     )
MIRK_X86_COMMAND (JGE,     0x0f8d, jge    )
MIRK_X86_COMMAND (JBE,     0x0f86, jbe    )
                                        
MIRK_X86_COMMAND (CALLF,   0xe8, callf  )
MIRK_X86_COMMAND (RETF,    0xc3, retf   )
                                        
MIRK_X86_COMMAND (SYSCALL, 0x0f05, syscall)
