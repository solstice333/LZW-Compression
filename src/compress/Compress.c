#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "LZWCmp.h"
#include "MyLib.h"

#define RECYCLE_CODE 4096
#define EOD 256

// 000t cbrs
typedef enum Pos {
   SPOS, RPOS, BPOS, CPOS, TPOS
} Pos;

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
               traceFlags |= 1 << TPOS; 

            if (argv[i][j] == 'c')
               traceFlags |= 1 << CPOS;

            if (argv[i][j] == 'b')
               traceFlags |= 1 << BPOS;

            if (argv[i][j] == 'r')
               traceFlags |= 1 << RPOS;

            if (argv[i][j] == 's')
               traceFlags |= 1 << SPOS;
         }
      }
      else 
         numFiles++;
   }

   assert(traceFlags == 24 || traceFlags == 16 || traceFlags == 8);
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
      LZWCmpInit(&cmp, Sink, "Sending code %d\n", RECYCLE_CODE, traceFlags);

      FILE *ifs = fopen(files[i], "r");

      char c;
      while ((c = fgetc(ifs)) != EOF)
         LZWCmpEncode(&cmp, c);
      LZWCmpEncode(&cmp, c);
   }

   return 0;
}
