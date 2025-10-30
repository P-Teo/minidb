# Compiler & flags
CC = clang
CFLAGS = -Wall -Iinclude -std=c11

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
	$(RM) $(OBJ_MAIN) $(OBJ_BUFFER) $(TARGET_MAIN) $(TARGET_BUFFER)
