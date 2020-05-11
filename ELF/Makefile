CC= gcc
CFLAGS= -Isrc

LD= $(CC)
LFLAGS= 

SRCDIR= src
OBJDIR= obj
BINDIR= bin

SRCFILES= main.c $(SRCDIR)/mirk.c
OBJFILES= $(OBJDIR)/main.o $(OBJDIR)/mirk.o
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
