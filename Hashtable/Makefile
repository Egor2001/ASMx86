CC= g++
LD= $(CC)

CFLAGS= -std=c++17 -march=native -O3
#CFLAGS= -std=c++17 -O0 -ggdb
LFLAGS=

OBJDIR= obj
BINDIR= bin
SRCDIR= src

HEADERS= $(SRCDIR)/CHashTable.h
SRCFILES= main.cpp $(SRCDIR)/CHashTable.cpp
OBJFILES= $(OBJDIR)/main.o $(OBJDIR)/CHashTable.o
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
