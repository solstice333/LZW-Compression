#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "LZWCmp.h"
#include "SmartAlloc.h"

#define NUMBITS 9
#define EOD 256

#define DEBUG 0

typedef enum Pos {
   SPOS, RPOS, BPOS, CPOS, TPOS
} Pos;

// freelist contains recycled nodes
static TreeNode *freelist = NULL;

// newNode() subsitutes malloc if recycled nodes exist in the freelist
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

// Abstract function to create BST
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

// Searches for the UChar |sym| within the BST and returns the associating
// code number
// **Deprecated** - Use BSTSearchCode
static int BSTSearchSym(UChar sym, TreeNode *root, void *cs) {
   if (root) {
      Code rootCode = GetCode(cs, root->cNum);

      if (memcmp(&sym, rootCode.data, 1) < 0) 
         return BSTSearchSym(sym, root->left, cs);
      else if (memcmp(&sym, rootCode.data, 1) > 0) 
         return BSTSearchSym(sym, root->right, cs);
      else
         return root->cNum;
   }
}

// Searches for Code |code| within the BST TreeNode |*root| and given CodeSet |*cs|.
// Returns NULL if code is not found
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

// Helper function for resetting the dictionary
static void dictionaryReset(LZWCmp *cmp) {
   LZWCmpDestruct(cmp);
   LZWCmpInit(cmp, cmp->sink, cmp->sinkState, cmp->recycleCode, 
    cmp->traceFlags);
}

// helper function for findMatchingPattern - checks for matching patterns given
// |data| which contains the pattern you want to find within cmp->pCode.data
static int regexCheck(UChar data[], LZWCmp *cmp) {
   int i = 0;
   for (; i < cmp->pCode.size - 1; i++) 
      if (data[0] == cmp->pCode.data[i] && data[1] == cmp->pCode.data[i + 1])
         return 1;
   return 0;
}

// finds the matching pattern in cmp->pCode.data
static int findMatchingPattern(LZWCmp *cmp) {
   UChar data1[] = { 10, 255 };
   return regexCheck(data1, cmp);
}

// Initialize the LZWCmp object
void LZWCmpInit(LZWCmp *cmp, CodeSink sink, void *sinkState, int recycleCode,
 int traceFlags) {
   // TODO initialize |nextInt|, |bitsUsed| 
   cmp->recycleCode = recycleCode;
   cmp->cst = CreateCodeSet(cmp->recycleCode + 1);
   cmp->root = BSTCreate();

   int i = 0;
   for (; i < EOD; i++) {
      NewCode(cmp->cst, i);
      cmp->root = BSTInsert(i, cmp->cst, cmp->root);
   }
   cmp->numBits = NUMBITS;
   NewCode(cmp->cst, 0);
   cmp->maxCode = EOD;

   cmp->sink = sink;
   cmp->sinkState = sinkState;
   cmp->traceFlags = traceFlags;
   
   cmp->curLoc = cmp->root;
   cmp->curCode = GetCode(cmp->cst, cmp->curLoc->cNum);

   cmp->pCodeLimit = EOD*2;
   cmp->pCode.data = malloc(cmp->pCodeLimit * sizeof(char));
   cmp->pCode.size = 0;
}

// TODO debug this
void LZWCmpEncode(LZWCmp *cmp, UChar sym) {
   if (cmp->maxCode == cmp->recycleCode) 
      dictionaryReset(cmp);

   cmp->pCode.data[cmp->pCode.size++] = sym;
   TreeNode *explore = BSTSearchCode(cmp->pCode, cmp->cst, cmp->root); 

   if (explore) 
      cmp->curLoc = explore; 
   else {
      cmp->maxCode = ExtendCode(cmp->cst, cmp->curLoc->cNum);  
      SetSuffix(cmp->cst, cmp->maxCode, sym); 
      BSTInsert(cmp->maxCode, cmp->cst, cmp->root);

      if (cmp->traceFlags >> CPOS & 1)  
         cmp->sink(cmp->sinkState, cmp->curLoc->cNum, 0);
      if (cmp->traceFlags >> TPOS & 1) 
         BSTPrint(cmp->root, cmp->cst);

      int oldsize = cmp->pCode.size;
      cmp->pCode.size = 0;
      cmp->curLoc = cmp->root;
      LZWCmpEncode(cmp, cmp->pCode.data[oldsize - 1]);
   }
}

void LZWCmpStop(LZWCmp *cmp) {
   if (cmp->maxCode > EOD) {
      if (cmp->traceFlags >> CPOS & 1) 
         cmp->sink(cmp->sinkState, cmp->curLoc->cNum, 1);

      if (cmp->traceFlags >> TPOS & 1) 
         BSTPrint(cmp->root, cmp->cst);
   }

   if (cmp->traceFlags >> CPOS & 1) 
      cmp->sink(cmp->sinkState, EOD, 1);

   if (cmp->traceFlags >> TPOS & 1) 
      BSTPrint(cmp->root, cmp->cst);
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
