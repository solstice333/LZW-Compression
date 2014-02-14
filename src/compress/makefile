CC=gcc
FILES=CodeSet.c LZWCmp.c SmartAlloc.c Compress.c 
OBJ=CodeSet.o LZWCmp.o SmartAlloc.o 
COMPRESS=Compress.o
STRESS=Stress.o
OUT=a.out
FLAGS=-g
TESTS=test*.out test*.in.K test*.in.Z

all: build

build: $(OBJ) $(COMPRESS)
	$(CC) $(FLAGS) $(OBJ) $(COMPRESS)
	#$(CC) $(FLAGS) $(OBJ) $(STRESS) 

CodeSet.o: CodeSet.c
	$(CC) -c CodeSet.c

LZWCmp.o: LZWCmp.c
	$(CC) -c LZWCmp.c

SmartAlloc.o: SmartAlloc.c
	$(CC) -c SmartAlloc.c

Compress.o: Compress.c
	$(CC) -c Compress.c

clean: 
	rm -rf $(OBJ) $(OUT) $(TESTS) $(COMPRESS)

rebuild: clean build
