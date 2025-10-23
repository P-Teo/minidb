CC = clang
CFLAGS = -Wall -Iinclude
OBJ = src/main.o src/pageio.o
TARGET = minidb

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

src/main.o: src/main.c include/common.h include/pageio.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/pageio.o: src/pageio.c include/pageio.h include/common.h
	$(CC) $(CFLAGS) -c src/pageio.c -o src/pageio.o

.PHONY: clean

clean:
	rm -f src/*.o
	rm -f $(TARGET)
