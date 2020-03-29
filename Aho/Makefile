PROJECT= "aho-corasick"

SRCDIR= src

BINDIR= bin
OBJDIR= obj

#C/C++ toolchain
CC= gcc
LD= $(CC)

#C/C++ flags
CFLAGS= -c -O2 -Ofast -Wall
LFLAGS=
DFLAGS= -ggdb -O0 -Wall

#C/C++ include path
INCLUDE= -I$(SRCDIR)

#ASM toolchain
ASM_CC= nasm
ASM_LD= ld

#ASM flags
ASM_CFLAGS= -f ELF64 -g 
ASM_LFLAGS= 

#C/C++ files
SRCFILES= main.c \
	  $(SRCDIR)/aho_context.c $(SRCDIR)/asm_dfa.c $(SRCDIR)/aho_corasick.c

OBJFILES= $(OBJDIR)/main.o \
	  $(OBJDIR)/aho_context.o $(OBJDIR)/asm_dfa.o $(OBJDIR)/aho_corasick.o
TARGET= $(BINDIR)/main

#ASM files
ASM_SRCFILE= test.asm

ASM_OBJFILE= $(OBJDIR)/test.o
ASM_TARGET= $(BINDIR)/test

#common targets
.PHONY: clean

cpp: $(TARGET)

asm: $(ASM_TARGET)

clean:
	rm -rf $(OBJFILES) $(ASM_OBJFILE)

#C/C++ targets
$(TARGET): $(OBJFILES)
	mkdir -p $(BINDIR)
	$(LD) $(LFLAGS) $(OBJFILES) -o $(TARGET)

$(OBJFILES): $(SRCFILES)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCFILES)
	mkdir -p $(OBJDIR)
	mv $(patsubst $(OBJDIR)/%.o, %.o, $(OBJFILES)) $(OBJDIR)

#ASM targets
$(ASM_TARGET): $(ASM_OBJFILE)
	$(ASM_LD) $(ASM_LFLAGS) $(ASM_OBJFILE) -o $(ASM_TARGET)

$(ASM_OBJFILE): $(ASM_SRCFILE)
	$(ASM_CC) $(ASM_CFLAGS) $(ASM_SRCFILE) -o $(ASM_OBJFILE) 
