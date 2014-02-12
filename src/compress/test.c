#include <stdio.h>
#include "MyLib.h"
#include "CodeSet.h"

static int isDone(Code c2) {
   UChar data[] = { 10, 255 };

   printf("%d %d\n", data[0], data[1]);

   int i = 0;
   for (; i < c2.size - 1; i++) 
      if (data[0] == c2.data[i] && data[1] == c2.data[i + 1])
            return 1;
   return 0;
}

static int codeCmp(Code c1, Code c2) {
   int codeLen = c1.size;
   if (codeLen > c2.size)
      codeLen = c2.size;

   int rtn = memcmp(c1.data, c2.data, codeLen);

   if (!rtn && c1.size != c2.size)
      rtn = c1.size - c2.size;

   return rtn;
}

static void printCode(Code c) {
   int i = 0;
   for (; i < c.size; i++)
      printf("%d ", c.data[i]);
   printf("\n");
}

int main() {
   char data1[] = { 10, 255 };
   Code c1 = { data1, 2 };

   //char data2[] = { 116, 115, 10, 255 };
   char data2[] = { 10, 255, 44 };
   Code c2 = { data2, 4 };

   printf("isDone: %d\n", isDone(c2));

   return 0;
}
