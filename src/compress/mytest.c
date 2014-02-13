#include <stdio.h>
#include <string.h>
#include "MyLib.h"
#include "CodeSet.h"

#define SIZE 5
#define MAXBITS 32

int main(int argc, char *argv[]) {
   // vars to mimic the members of LZWCmp struct
   int numBits = 9;
   int bitsUsed = 24;
   int nextInt = 0;

   // Here's how you do the filename extension 
   if (argc > 1) {
      char *filename = argv[1];
      char output[128];
      strcpy(output, filename);
      strcat(output, ".Z");
      FILE* ofs = fopen(output, "w");
      fprintf(ofs, "%08X %08X\n", 97, 98);
   }

   // pack these code numbers
   int cNum[] = { 97, 98, 257, 10, 256 };

   int i = 0;
   for (; i < SIZE; i++) {
      printf("cNum[%d]: %08X\n", i, cNum[i]);
   }
   printf("\n");

   // pack the bits in succession
   for (i = 0; i < SIZE; i++) {
      bitsUsed += numBits;

      if (bitsUsed > MAXBITS) {
         printf("bitsUsed at beginning: %d\n", bitsUsed);
         bitsUsed -= MAXBITS;
         nextInt |= cNum[i] >> bitsUsed;

         printf("Sending nextInt %08X\n", nextInt);
         nextInt = 0;
      }
      cNum[i] <<= MAXBITS - bitsUsed;
      nextInt |= cNum[i];

      printf("MAXBITS - bitsUsed: %d\n", MAXBITS - bitsUsed);
      printf("bitsUsed: %d\n", bitsUsed);
      printf("cNum[%d] shifted: %08X\n", i, cNum[i]);
      printf("nextInt: %08X\n", nextInt);

      printf("\n");
   }

   // if (maxCode == 1 << numBits) ++numBits
   printf("numBits shifted: %d\n", 1 << numBits);

   return 0;
}
