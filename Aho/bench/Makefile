PROJECT= "aho-bench"

CDIR= src_c
ASMDIR= src_asm

BINDIR= bin
OBJDIR= obj

# toolchain
CC= gcc
ASMC= nasm

LD= $(CC)

# flags
CFLAGS= -c -O0 -I$(CDIR) -g
ASMFLAGS= -fELF64 -I$(ASMDIR) -g 

LFLAGS= -m64

# files
SRCFILE= main.c 
OBJFILE= $(OBJDIR)/main.o

C_SRC= $(CDIR)/bench.c
C_OBJ= $(OBJDIR)/bench.c.o
CTARGET= $(BINDIR)/c_bench

ASM_SRC= $(ASMDIR)/bench.asm
ASM_OBJ= $(OBJDIR)/bench.asm.o
ASMTARGET= $(BINDIR)/asm_bench

# common targets
.PHONY: clean

all: $(CTARGET) $(ASMTARGET)

clean:
	rm $(OBJDIR)/*.o

# targets
$(CTARGET): $(OBJFILE) $(C_OBJ)
	$(LD) $(LFLAGS) $(OBJFILE) $(C_OBJ) -o $(CTARGET)

$(ASMTARGET): $(OBJFILE) $(ASM_OBJ)
	$(LD) $(LFLAGS) $(OBJFILE) $(ASM_OBJ) -o $(ASMTARGET)

# obj targets
$(OBJFILE): $(SRCFILE)
	$(CC) $(CFLAGS) $(SRCFILE) -o $(OBJFILE)

$(C_OBJ): $(C_SRC)
	$(CC) $(CFLAGS) $(C_SRC) -o $(C_OBJ) 

$(ASM_OBJ): $(ASM_SRC)
	$(ASMC) $(ASMFLAGS) $(ASM_SRC) -o $(ASM_OBJ) 
