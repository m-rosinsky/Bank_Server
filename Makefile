# Compiler selection.
CC=gcc

# Compiler flags.
CFLAGS=-Wall -Wextra -pedantic -pthread

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

# Link server executable.
	@$(CC) $(CFLAGS) -o ./$(BINS)/server ./source/server/main.c

# Link test executables.
	@$(CC) $(CFLAGS) -o ./$(BINS)/test_queue ./test/test_queue.c -lcunit

	@echo "   done"

clean:
	@$(RM) -rf $(OBJS) $(BINS)