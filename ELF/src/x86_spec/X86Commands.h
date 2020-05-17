// http://ref.x86asm.net/coder64.html

MIRK_X86_COMMAND (PUSH,    0x00, push   )
MIRK_X86_COMMAND (POP,     0x00, pop    )

MIRK_X86_COMMAND (MOV,     0x00, mov    )
MIRK_X86_COMMAND (LEA,     0x00, lea    )
                                         
MIRK_X86_COMMAND (INC,     0x00, inc    )
MIRK_X86_COMMAND (DEC,     0x00, dec    )
                                        
MIRK_X86_COMMAND (NEG,     0x00, neg    )
MIRK_X86_COMMAND (ADD,     0x00, add    )
MIRK_X86_COMMAND (SUB,     0x00, sub    )
MIRK_X86_COMMAND (MUL,     0x00, mul    )
MIRK_X86_COMMAND (DIV,     0x00, div    )
                                        
MIRK_X86_COMMAND (NOT,     0x00, not    )
MIRK_X86_COMMAND (OR,      0x00, or     )
MIRK_X86_COMMAND (AND,     0x00, and    )
MIRK_X86_COMMAND (XOR,     0x00, xor    )
                                        
MIRK_X86_COMMAND (CMP,     0x00, cmp    )
                                       
MIRK_X86_COMMAND (JMP,     0x00, jmp    )
MIRK_X86_COMMAND (JZ,      0x00, jz     )
MIRK_X86_COMMAND (JNZ,     0x00, jnz    )
MIRK_X86_COMMAND (JG,      0x00, jg     )
MIRK_X86_COMMAND (JB,      0x00, jb     )
MIRK_X86_COMMAND (JGE,     0x00, jge    )
MIRK_X86_COMMAND (JBE,     0x00, jbe    )
                                        
MIRK_X86_COMMAND (CALLF,   0x00, callf  )
MIRK_X86_COMMAND (RETF,    0x00, retf   )
                                        
MIRK_X86_COMMAND (SYSCALL, 0x00, syscall)
