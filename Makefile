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
	
# Compile common sources.
	@$(CC) $(CFLAGS) -o ./$(OBJS)/queue.o -c ./source/common/queue.c
	@$(CC) $(CFLAGS) -o ./$(OBJS)/threadpool.o -c ./source/common/threadpool.c

# Compile server sources.
	@$(CC) $(CFLAGS) -o ./$(OBJS)/user_db.o -c ./source/server/user_db.c

	@echo "   done"

link: setup compile
	@echo -n "Linking binaries..."

# Link server executable.
	@$(CC) $(CFLAGS) -o ./$(BINS)/server ./source/server/main.c $(OBJS)/*.o

	@echo "   done"

test: setup compile
	@echo -n "Linking test binaries..."

# Link test executables.
	@$(CC) $(CFLAGS) -o ./$(BINS)/test_queue ./test/test_queue.c -lcunit $(OBJS)/*.o
	@$(CC) $(CFLAGS) -o ./$(BINS)/test_threadpool ./test/test_threadpool.c -lcunit $(OBJS)/*.o
	@$(CC) $(CFLAGS) -o ./$(BINS)/test_user_db ./test/test_user_db.c -lcunit $(OBJS)/*.o

	@echo "   done"

clean:
	@$(RM) -rf $(OBJS) $(BINS)