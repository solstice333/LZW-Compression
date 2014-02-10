#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "LZWCmp.h"
#include "MyLib.h"

#define RECYCLE_CODE 4096
#define tPos 4
#define cPos 3
#define bPos 2
#define rPos 1
#define sPos 0

void Sink(void *state, UInt code, int done) {
   printf(state, code);
}

int main(int argc, char **argv) {
   /*
   t = dump contents of BST
   c = show individual codes
   b = announce each increase in bits per code as it occurs
   r = announce each dictionary recycle as it occurs
   s = announce space used after each file is compressed, and after all
    files are compressed and the freelist is cleared
   */
   int numFiles = -1;
   char traceFlags = 0; // 000t cbrs

   // Set the flags while obtaining the size of how many files are to be 
   // compressed
   int i = 0, j;
   for (; i < argc; i++) {
      j = 0;
      if (argv[i][j++] == '-') {
         for (; argv[i][j]; j++) {
            if (argv[i][j] == 't') 
               traceFlags |= 1 << tPos; 

            if (argv[i][j] == 'c')
               traceFlags |= 1 << cPos;

            if (argv[i][j] == 'b')
               traceFlags |= 1 << bPos;

            if (argv[i][j] == 'r')
               traceFlags |= 1 << rPos;

            if (argv[i][j] == 's')
               traceFlags |= 1 << sPos;
         }
      }
      else 
         numFiles++;
   }

   assert(traceFlags == 16);
   assert(numFiles == 1);

   // put all the filenames into the |files| container
   char *files[numFiles]; 
   for (i = 1, j = 0; i < argc; i++) {
      if (argv[i][0] != '-') 
         files[j++] = argv[i];
   }

   // begin compressing each file and verbose output based on flag set
   void (*fp)(void *, UInt, int) = Sink; 
   for (i = 0; i < numFiles; i++) {
      LZWCmp cmp;
      LZWCmpInit(&cmp, Sink, "Sending code %d\n", 4096, traceFlags);
      // TODO test initial dictionary here


      /*
      FILE *ifs = fopen(files[i], "r");
      printf("contents in %s:\n", files[i]);

      char c;
      while ((c = fgetc(ifs)) != EOF)
         fp("Sending code %d\n", c, 0); 
      fp("Sending code %d\n", 256, 1);
      printf("\n");
      */
   }

   return 0;
}
