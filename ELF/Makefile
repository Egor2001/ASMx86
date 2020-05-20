CC= g++
CFLAGS= -Isrc -std=c++17 -O2

LD= $(CC)
LFLAGS= 

SRCDIR= src
OBJDIR= obj
BINDIR= bin

SRCFILES= main.cpp \
	$(SRCDIR)/elfgen/context.cpp \
	$(SRCDIR)/disasm/CElcDisasm.cpp \
	$(SRCDIR)/exegen/CGenElcX86.cpp
OBJFILES= $(OBJDIR)/main.o \
	$(OBJDIR)/context.o \
	$(OBJDIR)/CElcDisasm.o \
	$(OBJDIR)/CGenElcX86.o
TARGET= $(BINDIR)/main

TAGFILE= tags

.PHONY: clean tags

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(LD) $(LFLAGS) $(OBJFILES) -o $(TARGET)

$(OBJFILES): $(SRCFILES)
	$(CC) -c $(CFLAGS) $(SRCFILES)
	mv $(patsubst $(OBJDIR)/%.o,%.o,$(OBJFILES)) $(OBJDIR)

clean:
	rm -f $(OBJFILES) $(TAGFILE)

tags:
	ctags -R -o $(TAGFILE)
