#include <stdio.h>
#include <stdlib.h>

int main() {
   unsigned char sym = -1;

   printf("EOF: %08X\n", (char signed)sym);
   printf("EOF: %d\n", (char signed)sym);
   return 0;
}
