#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "LZWCmp.h"
#include "MyLib.h"
#include "SmartAlloc.h"

#define RECYCLE_CODE 4096
#define EOD 256
#define NEWLINE 8
#define OUTPUT_EXT_SIZE 3
#define OUTPUT_EXT ".K"

// 000t cbrs
typedef enum Pos {
   SPOS, RPOS, BPOS, CPOS, TPOS
} Pos;

// data sink writes out to file and does pretty printing
void Sink(void *state, UInt code, int done) {
   static int occur = 0;

   if (occur == NEWLINE - 1) {
      fprintf(state, "%08X\n", code);
      occur = 0;
   }
   else {
      fprintf(state, "%08X ", code);
      ++occur;
   }

   if (done) {
      fprintf(state, "\n");
      fclose(state);
   }
}

int main(int argc, char **argv) {
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

            else if (argv[i][j] == 'c')
               traceFlags |= 1 << CPOS;

            else if (argv[i][j] == 'b')
               traceFlags |= 1 << BPOS;

            else if (argv[i][j] == 'r')
               traceFlags |= 1 << RPOS;

            else if (argv[i][j] == 's')
               traceFlags |= 1 << SPOS;

            else
               printf("Bad argument: %c\n", argv[i][j]);
         }
      }
      else 
         numFiles++;
   }

   // put all the filenames into the |files| container
   char *files[numFiles]; 
   for (i = 1, j = 0; i < argc; i++) {
      if (argv[i][0] != '-') 
         files[j++] = argv[i];
   }

   // begin compressing each file and verbose output based on flag set
   for (i = 0; i < numFiles; i++) {
      LZWCmp cmp;

      // set up output stream 
      int num = strlen(files[i]) + OUTPUT_EXT_SIZE;
      char *output = malloc(num*sizeof(char));
      strcpy(output, files[i]);
      strcat(output, OUTPUT_EXT);
      FILE *state = fopen(output, "w");    

      // intiialize LZWcmp object and open input stream
      LZWCmpInit(&cmp, Sink, state, RECYCLE_CODE, traceFlags);
      FILE *ifs = fopen(files[i], "r");
      
      // The following block checks ahead to see if the feof indicator
      // has been set since we don't want stuff like |10 255| to be added
      // to the dictionary at the EOF (where 255 is unsigned EOF). 
      // If feof has not been set, return to old pos, and 
      // send the character to Encoder
      char c = 0;
      LZWCmpEncode(&cmp, fgetc(ifs));
      c = fgetc(ifs);
      while (!feof(ifs)) {
         LZWCmpEncode(&cmp, c);
         c = fgetc(ifs);
      }

      LZWCmpStop(&cmp);

      if (cmp.traceFlags >> SPOS & 1)
         printf("Space after LZWCmpStop for %s: %ld\n", 
          files[i], report_space()); 

      LZWCmpDestruct(&cmp);
      LZWCmpClearFreelist();
      fclose(ifs);
      free(output);

      if (cmp.traceFlags >> SPOS & 1)
         printf("Final space: %ld\n", report_space()); 
   }

   return 0;
}
