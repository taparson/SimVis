# SOIL makefile for linux (based on the AngelScript makefile)
# Type 'make' then 'make install' to complete the installation of the library

# For 'make install' to work, set LOCAL according to your system configuration
LOCAL = /usr/local

LIB = libSOIL.a
INC = SOIL.h

SRCDIR = .
LIBDIR = ..
INCDIR = .
OBJDIR = obj

CXX = gcc
CXXFLAGS = -O2 -s -Wall
DELETER = rm -f
COPIER = cp

SRCNAMES = \
  image_helper.c \
  stb_image_aug.c  \
  image_DXT.c \
  SOIL.c \

OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SRCNAMES:.c=.o)))
BIN = $(LIBDIR)/$(LIB)

all: $(BIN)

$(BIN): $(OBJ)
	ar r $(BIN) $(OBJ)
	ranlib $(BIN)
	@echo -------------------------------------------------------------------
	@echo Done. As root, type 'make install' to install the library.

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CXX) $(CXXFLAGS) -o $@ -c $<


clean:
	$(DELETER) $(OBJ) $(BIN)

install: $(BIN)
	@echo Installing to: $(LOCAL)/lib and $(LOCAL)/include...
	@echo -------------------------------------------------------------------
	$(COPIER) $(BIN) $(LOCAL)/lib
	$(COPIER) $(INCDIR)/$(INC) $(LOCAL)/include
	@echo -------------------------------------------------------------------
	@echo SOIL library installed. Enjoy!

uninstall:
	$(DELETER) $(LOCAL)/include/$(INC) $(LOCAL)/lib/$(LIB)
	@echo -------------------------------------------------------------------
	@echo SOIL library uninstalled.

.PHONY: all clean install uninstall
