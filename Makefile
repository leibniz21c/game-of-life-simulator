#
# Author: Heesung Yang
# Since: 07.12.2021
#
include project.conf

# project variables 
ifndef PROJECT_NAME
$(error Missing PROJECT_NAME.)
endif
ifndef BINARY
$(error Missing BINARY.)
endif
ifndef PROJECT_PATH
$(error Missing PROJECT_PATH.)
endif

# Gets the Operating system name
OS := $(shell uname -s)

# Default shell
SHELL := bash

# Color prefix for Linux distributions
COLOR_PREFIX := e

ifeq ($(OS),Darwin)
	COLOR_PREFIX := 033
endif

# Color definition for print purpose
BROWN=\$(COLOR_PREFIX)[0;33m
BLUE=\$(COLOR_PREFIX)[1;34m
END_COLOR=\$(COLOR_PREFIX)[0m

# Source code directory structure
BINDIR := bin
SRCDIR := src
LIBDIR := lib

# Source code file extension
SRCEXT := c

# Defines the C Compiler
CC := cc

# Defines the language standards for GCC
STD := -std=gnu99 # See man gcc for more options

# Flags for compiling
CFLAGS := -O3 $(STD) -lncurses

# Debug options
DEBUG := -g3 -DDEBUG=1

# Dependency libraries
LIBS := # -lm  -I some/path/to/library

# %.o file names
NAMES := $(notdir $(basename $(wildcard $(SRCDIR)/*.$(SRCEXT))))
OBJECTS :=$(patsubst %,$(LIBDIR)/%.o,$(NAMES))


# --------------------------------------------------------------------------- #
#                             COMPILATION RULES
# --------------------------------------------------------------------------- #
default: all

# Rule for link and generate the binary file
all: $(OBJECTS)
	@echo -en "$(BROWN)LD $(END_COLOR)";
	$(CC) -o $(BINDIR)/$(BINARY) $+ $(DEBUG) $(CFLAGS) $(LIBS)
	@echo -en "\n--\nBinary file placed at" \
			  "$(BROWN)$(BINDIR)/$(BINARY)$(END_COLOR)\n";

# Rule for object binaries compilation
$(LIBDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo -en "$(BROWN)CC $(END_COLOR)";
	$(CC) -c $^ -o $@ $(DEBUG) $(CFLAGS) $(LIBS)

clean:
	@rm -rvf $(BINDIR)/* $(LIBDIR)/* $(LOGDIR)/*;
