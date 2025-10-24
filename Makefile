CC = clang
CFLAGS = -Wall -Iinclude

ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    EXEEXT = .exe
    RMOBJ = del /Q /F src\*.o
else
    RM = rm -f
    EXEEXT =
    RMOBJ = rm -f src/*.o
endif

TARGET_MAIN = minidb$(EXEEXT)
TARGET_BUFFER = test_buffer$(EXEEXT)

OBJ_MAIN = src/main.o src/pageio.o
OBJ_BUFFER = src/test_buffer.o src/pageio.o src/buffer.o

all: $(TARGET_MAIN) $(TARGET_BUFFER)

$(TARGET_MAIN): $(OBJ_MAIN)
	$(CC) $(CFLAGS) $(OBJ_MAIN) -o $(TARGET_MAIN)

$(TARGET_BUFFER): $(OBJ_BUFFER)
	$(CC) $(CFLAGS) $(OBJ_BUFFER) -o $(TARGET_BUFFER)

src/main.o: src/main.c include/common.h include/pageio.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/pageio.o: src/pageio.c include/pageio.h include/common.h
	$(CC) $(CFLAGS) -c src/pageio.c -o src/pageio.o

src/test_buffer.o: src/test_buffer.c include/buffer.h include/pageio.h include/common.h
	$(CC) $(CFLAGS) -c src/test_buffer.c -o src/test_buffer.o

src/buffer.o: src/buffer.c include/buffer.h include/pageio.h include/common.h
	$(CC) $(CFLAGS) -c src/buffer.c -o src/buffer.o

.PHONY: clean run_main run_buffer

clean:
	$(RMOBJ)
	$(RM) $(TARGET_MAIN) $(TARGET_BUFFER)

run_main: $(TARGET_MAIN)
	@echo === Rulez MiniDB ===
	@$(TARGET_MAIN)

run_buffer: $(TARGET_BUFFER)
	@echo === Rulez test_buffer ===
	@$(TARGET_BUFFER)
