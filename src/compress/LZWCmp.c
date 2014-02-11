#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "LZWCmp.h"

#define TPOS 4
#define CPOS 3
#define BPOS 2
#define RPOS 1
#define SPOS 0
#define NUMBITS 9
#define EOD 256

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
   if (codeLen < c2.size)
      codeLen = c2.size;
   
   return memcmp(c1.data, c2.data, codeLen);
}

// Abstract function to create BST
static TreeNode* BSTCreate() {
   return NULL;
}

// Insert codes into BST given |code|, TreeNode root of the BST |root|, and
// CodeSet void pointer |cs|. Note that the updated BST root |root| is 
// returned
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

// TODO finish implementing this
/*
static int BSTSearchCode(Code code, TreeNode *root, void *cs) {
   if (root) {
      Code rootCode = GetCode(cs, root->cNum);

   }
}
*/

// Helper function for BSTPrint
static void printBSTContents(TreeNode *root) {
   if (root) {
      printBSTContents(root->left);
      printf("|%d", root->cNum);
      printBSTContents(root->right);
   }
}

// Print contents of BST given |root|
static void BSTPrint(TreeNode *root) {
   printBSTContents(root);
   printf("|\n\n");
}

// Initialize the LZWCmp object
void LZWCmpInit(LZWCmp *cmp, CodeSink sink, void *sinkState, int recycleCode,
 int traceFlags) {
   // TODO initialize |maxCode|, |nextInt|, |bitsUsed|, 
   // |curLoc|, |curCode|, |pCode|, |pCodeLimit|  
   cmp->recycleCode = recycleCode;
   cmp->cst = CreateCodeSet(cmp->recycleCode);
   cmp->root = BSTCreate();

   int i = 0;
   for (; i < 256; i++) {
      NewCode(cmp->cst, i);
      cmp->root = BSTInsert(i, cmp->cst, cmp->root);
   }

   cmp->maxCode = i;
   cmp->numBits = NUMBITS;
   cmp->sink = sink;
   cmp->sinkState = sinkState;
   cmp->traceFlags = traceFlags;
}

// TODO implement creation of new codes
void LZWCmpEncode(LZWCmp *cmp, UChar sym) {
   int cNum = BSTSearchSym(sym, cmp->root, cmp->cst); 

   if (cmp->traceFlags >> CPOS & 1) { 
      if ((char signed)sym == EOF)   // Note: this overrides code 255
         cmp->sink(cmp->sinkState, EOD, 1);
      else
         cmp->sink(cmp->sinkState, cNum, 0);
   }

   if (cmp->traceFlags >> TPOS & 1)
      BSTPrint(cmp->root);
}



