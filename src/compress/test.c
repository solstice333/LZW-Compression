#include <stdio.h>
#include <stdlib.h>

int main() {
   int *array = malloc(4*sizeof(int));
   array[0] = 1;
   array[1] = 2;
   array[2] = 3;
   array[3] = 4;


   int i = 0;
   for (; i < 4; i++) {
      printf("%d ", array[i]);
   }

   return 0;
}
