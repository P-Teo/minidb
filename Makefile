# Compiler & flags
CC = clang
CFLAGS = -Wall -Wextra -Iinclude -std=c11 -D_POSIX_C_SOURCE=200112L

# OS-specific
ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    EXEEXT = .exe
    OBJDIR_SEP = \  # pentru Windows
else
    RM = rm -f
    EXEEXT =
    OBJDIR_SEP = /
endif

# Targets
TARGET_MAIN = minidb$(EXEEXT)
TARGET_BUFFER = test_buffer$(EXEEXT)

# Source files
SRC_MAIN = src/main.c src/pageio.c
SRC_BUFFER = src/test_buffer.c src/pageio.c src/buffer.c
OBJ_MAIN = src/main.o src/pageio.o src/buffer.o src/btree.o src/parser.o

# Object files
OBJ_MAIN = $(SRC_MAIN:.c=.o)
OBJ_BUFFER = $(SRC_BUFFER:.c=.o)

# Default rule
all: $(TARGET_MAIN) $(TARGET_BUFFER)

# Linking
$(TARGET_MAIN): $(OBJ_MAIN)
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET_BUFFER): $(OBJ_BUFFER)
	$(CC) $(CFLAGS) $^ -o $@

# Compile rule (implicit)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run
.PHONY: run_main run_buffer clean

run_main: $(TARGET_MAIN)
	@echo - Rulez MiniDB 
	@./$(TARGET_MAIN)

run_buffer: $(TARGET_BUFFER)
	@echo - Rulez test_buffer 
	@./$(TARGET_BUFFER)

# Clean
clean:
	rm -f src/*.o minidb.exe test_buffer.exe

