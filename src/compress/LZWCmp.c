#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "LZWCmp.h"
#include "SmartAlloc.h"

#define INIT_NUMBITS 9
#define MAX_NUMBITS 13 // log automation?
#define EOD 256
#define MAXBITS 32
#define ALLOC_PCODE 1024

#define DEBUG 0

typedef enum Pos {
   TPOS, BPOS, RPOS, CPOS, SPOS
} Pos;

// freelist contains recycled nodes
static TreeNode *freelist = NULL;

// Subsitutes malloc if recycled nodes exist in the freelist
static TreeNode *newNode() {
   TreeNode *node = freelist;
   freelist = freelist->right;
   return node;
}

// Abstract function to compare codes during BST insertion. Should be compared
// lexicographically
static int codeCmp(Code c1, Code c2) {
   int codeLen = c1.size;
   if (codeLen > c2.size)
      codeLen = c2.size;

   int rtn = memcmp(c1.data, c2.data, codeLen);

   if (!rtn && c1.size != c2.size)
      rtn = c1.size - c2.size;

   return rtn;
}

// Abstract function to create inital BST
static TreeNode* BSTCreate() {
   return NULL;
}

// Insert codes into BST given code number |cNum|, code set |cs|, and 
// BST |root|. Updated BST is returned
static TreeNode *BSTInsert(int cNum, void *cs, TreeNode *root) {
   if (!root) {
      root = freelist ? newNode() : malloc(sizeof(TreeNode));
      root->cNum = cNum;
      root->left = root->right = NULL;
   }

   else {
      Code rootCode = GetCode(cs, root->cNum);
      Code insertCode = GetCode(cs, cNum);

      if (codeCmp(insertCode, rootCode) < 0)
         root->left = BSTInsert(cNum, cs, root->left);
      else
         root->right = BSTInsert(cNum, cs, root->right);
   }

   return root;
}

// Searches for Code |code| within the BST TreeNode |*root| matching the 
// given CodeSet |*cs|. Returns NULL if code is not found
static TreeNode *BSTSearchCode(Code code, void *cs, TreeNode *root) {
   if (root) {
      Code rootCode = GetCode(cs, root->cNum);

      if(codeCmp(code, rootCode) < 0)
         return BSTSearchCode(code, cs, root->left);
      else if (codeCmp(code, rootCode) > 0)
         return BSTSearchCode(code, cs, root->right);
      else
         return root;
   }
   return NULL;
}

// Destroys the BST and sends the nodes to the freelist
static void BSTDestroy(TreeNode *root) {
   if (root) {
      BSTDestroy(root->left);
      BSTDestroy(root->right);
      
      root->right = freelist;
      root->left = NULL;
      freelist = root;
   }
}

// Helper function for BSTPrint
static void printBSTContents(TreeNode *root, void *cs) {
   if (root) {
      printBSTContents(root->left, cs);

      printf("|");
      Code c = GetCode(cs, root->cNum); 
      int i = 0;
      for (; i < c.size; i++) {
         if (i < c.size - 1)
            printf("%d ", c.data[i]);
         else
            printf("%d", c.data[i]);
      }

      printBSTContents(root->right, cs);
   }
}

// Print contents of BST given |root|
static void BSTPrint(TreeNode *root, void *cs) {
   printBSTContents(root, cs);
   printf("|\n\n");
}

// Convenience function for printing out codes
static void printCode(Code c) {
   int i = 0;
   for (; i < c.size; i++) 
      printf("%d ", c.data[i]);
   printf("\n");
}

// Destroys char *data inside Code objects
static void destroyCodeData(LZWCmp *cmp) {
   int i = 0; 
   for (; i <= cmp->maxCode; i++) {
      Code c = GetCode(cmp->cst, i);
      free(c.data);
   }
}

// Helper function for resetting the dictionary
static void dictionaryReset(LZWCmp *cmp) {
   int saveNextInt = cmp->nextInt, saveBitsUsed = cmp->bitsUsed;

   if (cmp->traceFlags >> RPOS & 1)
      printf("Recycling dictionary...\n");

   destroyCodeData(cmp);
   LZWCmpDestruct(cmp);
   LZWCmpInit(cmp, cmp->sink, cmp->sinkState, cmp->recycleCode, 
    cmp->traceFlags);

   cmp->nextInt = saveNextInt;
   cmp->bitsUsed = saveBitsUsed;
}

// Handles most verbose trace flag options. LZWCmp |*cmp| is needed for 
// printing the BST and int |cNum| is the code sent to the sink
static void traceFlagHandler(LZWCmp *cmp, int cNum) {
   if (cmp->traceFlags >> CPOS & 1)  
      printf("Sending code %d\n", cNum);
   if (cmp->traceFlags >> TPOS & 1) 
      BSTPrint(cmp->root, cmp->cst);
}

// Pack those bits into integers and send to sink
static void packBits(LZWCmp *cmp, int done) {
   int cNum = done ? EOD : cmp->curLoc->cNum;

   cmp->bitsUsed += cmp->numBits;
   if (cmp->bitsUsed > MAXBITS) {
      cmp->bitsUsed -= MAXBITS;
      cmp->nextInt |= cNum >> cmp->bitsUsed;
      cmp->sink(cmp->sinkState, cmp->nextInt, 0);
      cmp->nextInt = 0;
   }
   cmp->nextInt |= cNum << MAXBITS - cmp->bitsUsed;

   if (cmp->maxCode == 1 << cmp->numBits) {
      ++cmp->numBits;

      if (cmp->traceFlags >> BPOS & 1 && cmp->numBits != MAX_NUMBITS)
         printf("Bump numBits to %d\n", cmp->numBits);
   }

   if (done) {
      cmp->sink(cmp->sinkState, cmp->nextInt, 0);
      cmp->sink(cmp->sinkState, 0, done);
   }
}

// Used for reallocating cmp->pCode.data since realloc 
// doesn't work with smartalloc
static void reallocPcode(LZWCmp *cmp) {
   char *temp = cmp->pCode.data;
   int oldsize = cmp->pCodeLimit;
   
   cmp->pCodeLimit = cmp->pCodeLimit + ALLOC_PCODE;
   cmp->pCode.data = malloc(cmp->pCodeLimit * sizeof(char));
   memmove(cmp->pCode.data, temp, oldsize); 

   free(temp);
}

void LZWCmpInit(LZWCmp *cmp, CodeSink sink, void *sinkState, int recycleCode,
 int traceFlags) {
   cmp->recycleCode = recycleCode;
   cmp->cst = CreateCodeSet(cmp->recycleCode + 1);
   cmp->root = BSTCreate();

   int i = 0;
   for (; i < EOD; i++) {
      NewCode(cmp->cst, i);
      cmp->root = BSTInsert(i, cmp->cst, cmp->root);
   }
   cmp->numBits = INIT_NUMBITS;
   NewCode(cmp->cst, 0);
   cmp->maxCode = EOD;

   cmp->sink = sink;
   cmp->sinkState = sinkState;
   cmp->traceFlags = traceFlags;
   
   cmp->curLoc = cmp->root;
   cmp->curCode = GetCode(cmp->cst, cmp->curLoc->cNum);

   cmp->pCodeLimit = ALLOC_PCODE;
   cmp->pCode.data = malloc(cmp->pCodeLimit * sizeof(char));
   cmp->pCode.size = 0;

   cmp->nextInt = 0;
   cmp->bitsUsed = 0;
}

void LZWCmpEncode(LZWCmp *cmp, UChar sym) {
   if (cmp->maxCode == cmp->recycleCode) 
      dictionaryReset(cmp);

   if (cmp->pCode.size == cmp->pCodeLimit)
      reallocPcode(cmp);
   cmp->pCode.data[cmp->pCode.size++] = sym;

   TreeNode *explore = BSTSearchCode(cmp->pCode, cmp->cst, cmp->curLoc); 

   if (explore) 
      cmp->curLoc = explore; 
   else {
      cmp->maxCode = ExtendCode(cmp->cst, cmp->curLoc->cNum);  
      SetSuffix(cmp->cst, cmp->maxCode, sym); 
      BSTInsert(cmp->maxCode, cmp->cst, cmp->root);
      traceFlagHandler(cmp, cmp->curLoc->cNum);

      packBits(cmp, 0);

      int oldsize = cmp->pCode.size;
      cmp->pCode.size = 0;
      cmp->curLoc = cmp->root;
      LZWCmpEncode(cmp, cmp->pCode.data[oldsize - 1]);
   }
}

void LZWCmpStop(LZWCmp *cmp) {
   if (cmp->maxCode > EOD) {
      traceFlagHandler(cmp, cmp->curLoc->cNum);
      packBits(cmp, 0);
   }
   traceFlagHandler(cmp, EOD);
   packBits(cmp, 1);
   destroyCodeData(cmp);
}

void LZWCmpDestruct(LZWCmp *cmp) {
   DestroyCodeSet(cmp->cst);
   BSTDestroy(cmp->root);
   free(cmp->pCode.data);
}

void LZWCmpClearFreelist() {
   while (freelist) {
      TreeNode *temp = freelist;
      freelist = temp->right;
      free(temp);
   }
}
