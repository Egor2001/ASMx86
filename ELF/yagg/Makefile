CC= g++
CFLAGS= -Isrc -std=c++17 -Wall -O2

LD= $(CC)
LFLAGS= 

SRCDIR= src
OBJDIR= obj
BINDIR= bin

HDRFILES= \
	$(SRCDIR)/frontend/CYaggParser.hpp \
	$(SRCDIR)/backend/CYaggBuilder.hpp \
	$(SRCDIR)/SYaggData.h \
	$(SRCDIR)/SYaggEntry.h \
	$(SRCDIR)/SYaggInstr.h 

SRCFILES= main.cpp \
	$(SRCDIR)/frontend/CYaggParser.cpp \
	$(SRCDIR)/backend/CYaggBuilder.cpp \
	$(SRCDIR)/struct/SYaggData.cpp \
	$(SRCDIR)/struct/SYaggEntry.cpp \
	$(SRCDIR)/struct/SYaggInstr.cpp 

OBJFILES= $(OBJDIR)/main.o \
	$(OBJDIR)/CYaggParser.o \
	$(OBJDIR)/CYaggBuilder.o \
	$(OBJDIR)/SYaggData.o \
	$(OBJDIR)/SYaggEntry.o \
	$(OBJDIR)/SYaggInstr.o 

TARGET= $(BINDIR)/main

TAGFILE= tags

.PHONY: clean tags

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(LD) $(LFLAGS) $(OBJFILES) -o $(TARGET)

$(OBJFILES): $(SRCFILES) $(HDRFILES)
	$(CC) -c $(CFLAGS) $(SRCFILES)
	mv $(patsubst $(OBJDIR)/%.o,%.o,$(OBJFILES)) $(OBJDIR)

clean:
	rm -f $(OBJFILES) $(TAGFILE)

tags:
	ctags -R -o $(TAGFILE)
