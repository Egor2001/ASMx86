CC= g++
ASMC= nasm
LD= $(CC)

CFLAGS= -std=c++17 -march=native -O3 #CFLAGS= -std=c++17 -O0 -ggdb
ASMCFLAGS= -fELF64 -Isrc -o hash.o
LFLAGS= -m64

OBJDIR= obj
BINDIR= bin
SRCDIR= src

HEADERS= $(SRCDIR)/CHashTable.h
SRCFILES= main.cpp $(SRCDIR)/CHashTable.cpp
ASMSRC= $(SRCDIR)/hash.asm
OBJFILES= $(OBJDIR)/main.o $(OBJDIR)/CHashTable.o $(OBJDIR)/hash.o
TARGET= $(BINDIR)/main

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(LD) $(LFLAGS) $(OBJFILES) -o $(TARGET)

$(OBJFILES): $(SRCFILES) $(HEADERS)
	$(CC) -c $(CFLAGS) $(SRCFILES)
	$(ASMC) $(ASMCFLAGS) $(ASMSRC)
	mv $(patsubst $(OBJDIR)/%.o, %.o, $(OBJFILES)) $(OBJDIR)

clean:
	rm -f $(OBJDIR)/*.o
