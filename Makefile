# Compiler selection.
CC=gcc

# Compiler flags.
CFLAGS=-Wall -Wextra -pednatic -pthread

# Object directory.
OBJS=objs

# Binary directory.
BINS=bins

all: setup compile link

setup:
	@echo -n "Creating subdirectories..."
	@mkdir -p $(OBJS) $(BINS)
	@echo "   done"

compile: setup
	@echo -n "Compiling sources..."
	
	@echo "   done"

link: setup compile
	@echo -n "Linking binaries..."

	@echo "   done"

clean:
	@$(RM) -rf $(OBJS) $(BINS)