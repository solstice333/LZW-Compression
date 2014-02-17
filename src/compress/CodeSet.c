#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "SmartAlloc.h"
#include "CodeSet.h"

#define DEBUG 0

// This holds code information that can be reconstructed into a Code object
typedef struct CodeEntry {
   char value;
   struct CodeEntry *prev;
   char *data;
   unsigned short size, calls;
} CodeEntry;

// This holds a pointer to the set of CodeEntry objects
typedef struct CodeSet {
   CodeEntry *open, *codeList;
   int size;
} CodeSet;

void *CreateCodeSet(int numCodes) {
   CodeEntry *codeEntry = calloc(numCodes, sizeof(CodeEntry));

   CodeSet *codeSet = (CodeSet *) malloc(sizeof(CodeSet));
   codeSet->open = codeSet->codeList = codeEntry;
   codeSet->size = 0;

   return codeSet;
}

int NewCode(void *codeSet, char val) {
   CodeSet *cs = codeSet;
   cs->open++->value = val;

   return cs->size++;
}

int ExtendCode(void *codeSet, int oldCode) {
   CodeSet *cs = codeSet;
   cs->open++->prev = cs->codeList + oldCode;

   return cs->size++;
}

void SetSuffix(void *codeSet, int code, char suffix) {
   CodeSet *cs = codeSet;

   if (cs->codeList[code].calls > 0) {
      int tail = cs->codeList[code].size - 1;
      cs->codeList[code].data[tail] = suffix;
   }

   cs->codeList[code].value = suffix;
}

static int getSize(CodeEntry *ce) {
   int size;
   for (size = 0; ce; size++, ce = ce->prev);

   return size;
}

static int getInOrder(CodeEntry *ce, char *str) {
   if (ce) {
      int index = getInOrder(ce->prev, str);
      str[index] = ce->value;
      return ++index;
   }
   return 0;
}

static char *reconstruct(CodeEntry *ce, int *size) {
   *size = getSize(ce);
   char *str = (char *) malloc(*size * sizeof(char));
   getInOrder(ce, str);

   return str;
}

Code GetCode(void *codeSet, int code) {
   int size = 0;
   CodeSet *cs = codeSet;

   if (cs->codeList[code].calls < 1) {
      cs->codeList[code].data = reconstruct(cs->codeList + code,
            &size);
      cs->codeList[code].size = size;
      cs->codeList[code].calls = 1;
   }
   else
      cs->codeList[code].calls++;

   Code c = {cs->codeList[code].data, cs->codeList[code].size}; 

   return c;
}

void FreeCode(void *codeSet, int code) {
   CodeSet *cs = codeSet;

   if (cs->codeList[code].calls > 0) {
      if (cs->codeList[code].calls == 1)
         free(cs->codeList[code].data);
      cs->codeList[code].calls--;
   }
}

void DestroyCodeSet(void *codeSet) {
   CodeSet *cs = codeSet;
   free(cs->codeList);
   free(cs);
}
