CC=gcc
FILES=CodeSet.c LZWCmp.c SmartAlloc.c Compress.c
OBJ=CodeSet.o LZWCmp.o SmartAlloc.o Compress.o
OUT=a.out
TESTS=test*.out

all: build

build: $(OBJ)
	$(CC) $(OBJ)

CodeSet.o: CodeSet.c
	$(CC) -c CodeSet.c

LZWCmp.o: LZWCmp.c
	$(CC) -c LZWCmp.c

SmartAlloc.o: SmartAlloc.c
	$(CC) -c SmartAlloc.c

Compress.o: Compress.c
	$(CC) -c Compress.c

clean: 
	rm -rf $(OBJ) $(OUT) $(TESTS)

rebuild: clean build
