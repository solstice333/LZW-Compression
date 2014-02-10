#include <stdio.h>
#include <stdlib.h>

int main() {
   char *str1 = "a";
   char *str2 = "b";
   printf("%d\n", memcmp(str1, str2, sizeof(char)));
   return 0;
}
