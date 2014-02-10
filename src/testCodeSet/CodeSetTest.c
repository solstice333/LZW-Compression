#include <stdio.h>
#include "SmartAlloc.h"
#include "CodeSet.h"

int main() {
   char command;
   void *cs[3];  // Array of three void pointers
   int ndx, numCodes, val, set, depth;
   Code code;
   
   while (EOF != scanf(" %c", &command)) {
      switch (command) {
      case 'C': // create new codeSet     C <set> <maxCodes>
         scanf("%d%d", &set, &numCodes);
         cs[set] = CreateCodeSet(numCodes);
         printf("Set %d initialized\n", set);
         break;
      case 'N': // add new code           N <set> <value>
         scanf("%d%d", &set, &val);
         printf("New code in set %d at index %d\n", set, NewCode(cs[set], val));
         break;
      case 'E': // extend existing code   E <set> <codeIndex>
         scanf("%d%d", &set, &ndx);
         printf("Added extended code %d\n", ExtendCode(cs[set], ndx));
         break;
      case 'S': // set suffix             S <set> <codeIndex> <suffix>
         scanf("%d%d%d", &set, &ndx, &val);
         SetSuffix(cs[set], ndx, val);
         break;
      case 'G': // get and print code     G <set> <codeIndex>
         scanf("%d%d", &set, &ndx);
         printf("Code %d in set %d has value: ", ndx, set);
         Code c = GetCode(cs[set], ndx);

         int i;
         printf("%d", c.data[0]);
         for (i = 1; i < c.size; i++) {
            printf(" %d", c.data[i]);
         }

         printf("\n");
         break;
      case 'F': // free code              F <set> <codeIndex>
         scanf("%d%d", &set, &ndx);
         FreeCode(cs[set], ndx);
         break;
      case 'R': // Report current space
         printf("Current space: %d\n", report_space());
         break;
      case 'X': // Add many codes in prefix order  X <set> <depth>
         scanf("%d%d", &set, &depth);
         ndx = NewCode(cs[set], 'X');
         while (--depth) {
            ndx = ExtendCode(cs[set], ndx);
            SetSuffix(cs[set], ndx, 'X');
         }
         break;
      case 'D': // Destroy existing code set  D <set>
         scanf("%d", &set);
         DestroyCodeSet(cs[set]);
         printf("Set %d destroyed\n", set);
      }
   }

}

/* Sample input
C 0 10
C 1 20
C 2 100000
R
N 0 1
R
N 0 41
E 0 1
G 0 2
E 0 0
S 0 2 42
G 0 2
S 0 3 43
G 0 3
E 0 2
S 0 4 45
E 0 4
S 0 5 46
E 0 5
S 0 6 46
G 0 6
G 0 2
R
F 0 6
R
F 0 2
R
F 0 2
F 0 3
R
E 0 5
R
S 0 7 46
R
G 0 7
N 1 2
R
F 0 7
R
D 0
R
D 1
R
X 2 1000
G 2 999
R
F 2 999
R
G 2 999
D 2
R
*/
