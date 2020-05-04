CC= g++
LD= $(CC)

CFLAGS= -std=c++17 -march=native -O3 #CFLAGS= -std=c++17 -O0 -ggdb
LFLAGS= -msse4 -m64

OBJDIR= obj
BINDIR= bin
SRCDIR= src

HEADERS= $(SRCDIR)/data_struct.h $(SRCDIR)/naive.h $(SRCDIR)/sse_blend.h
SRCFILES= main.c $(SRCDIR)/naive.c $(SRCDIR)/sse_blend.c $(SRCDIR)/bitmap.c
OBJFILES= $(OBJDIR)/main.o $(OBJDIR)/naive.o $(OBJDIR)/sse_blend.o $(OBJDIR)/bitmap.o
TARGET= $(BINDIR)/main

.PHONY: clean

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(LD) $(LFLAGS) $(OBJFILES) -o $(TARGET)

$(OBJFILES): $(SRCFILES) $(HEADERS)
	$(CC) -c $(CFLAGS) $(SRCFILES)
	mv $(patsubst $(OBJDIR)/%.o, %.o, $(OBJFILES)) $(OBJDIR)

clean:
	rm -f $(OBJDIR)/*.o
