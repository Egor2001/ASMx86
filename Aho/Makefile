PROJECT= "aho-corasick"

CC= gcc
LD= $(CC)

CFLAGS= -c -O2 -Ofast -Wall
DFLAGS= -ggdb -O0 -Wall

LFLAGS=

SRCDIR= src
SRCFILES= main.c $(SRCDIR)/asm_dfa.c $(SRCDIR)/aho_corasick.c

BINDIR= bin
OBJDIR= obj
OBJFILES= $(OBJDIR)/main.o $(OBJDIR)/asm_dfa.o $(OBJDIR)/aho_corasick.o
TARGET= $(BINDIR)/main

INCLUDE= -I$(SRCDIR)

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJFILES)
	mkdir -p $(BINDIR)
	$(LD) $(LFLAGS) $(OBJFILES) -o $(TARGET)

$(OBJFILES): $(SRCFILES)
	$(CC) $(CFLAGS) $(INCLUDE) $(SRCFILES)
	mkdir -p $(OBJDIR)
	mv $(patsubst $(OBJDIR)/%.o, %.o, $(OBJFILES)) $(OBJDIR)

clean:
	rm -rf $(OBJFILES)
