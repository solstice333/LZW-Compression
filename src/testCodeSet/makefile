CC=gcc
FILES=CodeSet.c SmartAlloc.c CodeSetTest.c
OBJ=CodeSet.o SmartAlloc.o CodeSetTest.o
OUT=a.out

all: build

build: $(OBJ)
	$(CC) -w $(OBJ)

CodeSet.o: CodeSet.c
	$(CC) -c -w CodeSet.c

SmartAlloc.o: SmartAlloc.c
	$(CC) -c -w SmartAlloc.c

CodeSetTest.o: CodeSetTest.c
	$(CC) -c -w CodeSetTest.c

clean:
	rm -rf $(OBJ) $(OUT)

rebuild: clean build
