CC= nasm
LD= ld

CFLAGS= -f elf64 -g
LFLAGS=

BINDIR= bin

SRCFILE= test.asm
OBJFILE= $(BINDIR)/test.o

TARGET= $(BINDIR)/test

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJFILE)
	$(LD) -o $(TARGET) $(OBJFILE)

$(OBJFILE): $(SRCFILE)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(OBJFILE) $(SRCFILE)

clean:
	rm $(OBJFILE)
