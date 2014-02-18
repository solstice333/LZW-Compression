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
#define OUTPUT_EXT ".Z"

// 000t cbrs
typedef enum Pos {
   TPOS, BPOS, RPOS, CPOS, SPOS
} Pos;

// Config struct
typedef struct Config {
   FILE *ofs;
   int occur;
} Config;

// data sink writes out to file and does pretty printing
void Sink(void *state, UInt code, int done) {
   Config *config = state;
   if (done) {
      fprintf(config->ofs, "\n");
      fclose(config->ofs);
   }
   else {
      if (config->occur == NEWLINE - 1) {
         fprintf(config->ofs, "%08X\n", code);
         config->occur = 0;
      }
      else {
         fprintf(config->ofs, "%08X ", code);
         ++config->occur;
      }
   }
}

int main(int argc, char **argv) {
   int numFiles = 0;
   char traceFlags = 0; // 000t cbrs

   // Set the flags while obtaining the size of how many files are to be 
   // compressed
   int i = 1, j, lock = 1, lockAtIndex = 0;

   for (; i < argc; i++) {
      j = 0;
      if (argv[i][j++] == '-' && lock) {
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
      else if (lock && !lockAtIndex) {
         lock = 0;
         lockAtIndex = i;
         numFiles++;
      }
      else 
         numFiles++;
   }

   // put all the filenames into the |files| container
   char *files[numFiles]; 
   for (i = lockAtIndex, j = 0; i < argc; i++)
      files[j++] = argv[i];

   // begin compressing each valid file and verbose output based on flag set
   LZWCmp cmp;
   for (i = 0; i < numFiles; i++) {
      // open input stream
      FILE *ifs = fopen(files[i], "r");
      if (!ifs) {
         printf("Cannot open %s\n", files[i]);
         continue;
      }

      // set up output stream 
      int num = strlen(files[i]) + OUTPUT_EXT_SIZE;
      char *output = malloc(num*sizeof(char));
      strcpy(output, files[i]);
      strcat(output, OUTPUT_EXT);
      FILE *ofs = fopen(output, "w");    
      Config state = { ofs, 0 };

      // initialize LZWCmp object
      LZWCmpInit(&cmp, Sink, &state, RECYCLE_CODE, traceFlags);

      // TODO apparently benchmark Compress.o calls one less
      // The following block checks ahead to see if the feof indicator
      // has been set since we don't want stuff like |10 255| to be added
      // to the dictionary at the EOF (where 255 is unsigned EOF). 
      // If feof has not been set, return to old pos, and 
      // send the character to Encoder
      char oneStep = 0;

      LZWCmpEncode(&cmp, fgetc(ifs));
      oneStep = fgetc(ifs);
      while (!feof(ifs)) {
         LZWCmpEncode(&cmp, oneStep);
         oneStep = fgetc(ifs);
      }
      LZWCmpStop(&cmp);

      if (cmp.traceFlags >> SPOS & 1)
         printf("Space after LZWCmpStop for %s: %ld\n", 
          files[i], report_space()); 

      LZWCmpDestruct(&cmp);
      fclose(ifs);
      free(output);
   }

   LZWCmpClearFreelist();
   if (cmp.traceFlags >> SPOS & 1)
      printf("Final space: %ld\n", report_space()); 

   return 0;
}
