#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "LZWCmp.h"

#define NUMBITS 9
#define EOD 256
#define SIZE 256
#define SEARCH_FAILED -1

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
      // TODO make sure to free this later
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
// Returns SEARCH_FAILED if code is not found
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

// Initialize the LZWCmp object
void LZWCmpInit(LZWCmp *cmp, CodeSink sink, void *sinkState, int recycleCode,
 int traceFlags) {
   // TODO initialize |nextInt|, |bitsUsed| 
   cmp->recycleCode = recycleCode;
   cmp->cst = CreateCodeSet(cmp->recycleCode);
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

// TODO implement creation of new codes
void LZWCmpEncode(LZWCmp *cmp, UChar sym) {
   cmp->pCode.data[cmp->pCode.size++] = sym;
   

#if DEBUG
   int i = 0;
   printf("cmp->pCode: ");
   printCode(cmp->pCode);
#endif


   TreeNode *explore = BSTSearchCode(cmp->pCode, cmp->cst, cmp->root); 


#if DEBUG
   printf("Search success: %d\n", explore != NULL);
   printf("Current location init: ");
   printCode(GetCode(cmp->cst, cmp->curLoc->cNum));
#endif


   if (explore) 
      cmp->curLoc = explore; 
   else {
#if DEBUG
      printf("Extending off of ");
      printCode(GetCode(cmp->cst, cmp->curLoc->cNum));
#endif


      cmp->maxCode = ExtendCode(cmp->cst, cmp->curLoc->cNum);  
      SetSuffix(cmp->cst, cmp->maxCode, sym); 
      BSTInsert(cmp->maxCode, cmp->cst, cmp->root);


      if (cmp->traceFlags >> CPOS & 1)  
         cmp->sink(cmp->sinkState, cmp->curLoc->cNum, 0);

      int oldsize = cmp->pCode.size;
      cmp->pCode.size = 0;
      cmp->curLoc = cmp->root;

#if DEBUG
      printf("\n\nEntering LZWCmpEncode recursively:\n");  
#endif

      LZWCmpEncode(cmp, cmp->pCode.data[oldsize - 1]);

#if DEBUG
      printf("Out of LZWCmpEncode recursive call:\n");  
#endif


      if (cmp->traceFlags >> CPOS & 1) { 
         if ((char signed)sym == EOF)   // Note: this overrides code 255
            cmp->sink(cmp->sinkState, EOD, 1);
      }
   }


#if DEBUG
   printf("Current location final: ");
   printCode(GetCode(cmp->cst, cmp->curLoc->cNum));
#endif




   

   // traceFlags options
   if (cmp->traceFlags >> TPOS & 1)
      BSTPrint(cmp->root, cmp->cst);

}
