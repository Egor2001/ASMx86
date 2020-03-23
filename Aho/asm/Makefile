CC= nasm
LD= ld

CFLAGS= -f elf64 -g
LFLAGS=

BINDIR= bin

SRCFILE= printf.asm
OBJFILE= $(BINDIR)/printf.o

TARGET= $(BINDIR)/printf

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJFILE)
	$(LD) -o $(TARGET) $(OBJFILE)

$(OBJFILE): $(SRCFILE)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(OBJFILE) $(SRCFILE)

clean:
	rm $(OBJFILE)
