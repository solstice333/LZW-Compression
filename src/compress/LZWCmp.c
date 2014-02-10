#include <stdio.h>
#include <stdlib.h>
#include "LZWCmp.h"

#define tPos 4
#define cPos 3
#define bPos 2
#define rPos 1
#define sPos 0

static TreeNode *freelist = NULL;

static TreeNode *newNode() {
   TreeNode *node = freelist;
   freelist = freelist->right;
   return node;
}

static int codeCmp(int cNum1, int cNum2, void *cs) {
   Code c1 = GetCode(cs, cNum1);
   Code c2 = GetCode(cs, cNum2);

   int codeLen = c1.size;
   if (codeLen < c2.size)
      codeLen = c2.size;
   
   return memcmp(c1.data, c2.data, codeLen);
}

static TreeNode* BSTCreate() {
   return NULL;
}

static TreeNode *BSTInsert(int code, TreeNode *root, void *cs) {
   if (!root) {
      root = freelist ? newNode() : malloc(sizeof(TreeNode));
      root->cNum = code;
      root->left = root->right = NULL;
   }

   else {
      if (codeCmp(code, root->cNum, cs) < 0)
         root->left = BSTInsert(code, root->left, cs);
      else
         root->right = BSTInsert(code, root->right, cs);
   }

   return root;
}

static void printBSTContents(TreeNode *root) {
   if (root) {
      printBSTContents(root->left);
      printf("|%d", root->cNum);
      printBSTContents(root->right);
   }
}

static void BSTPrint(TreeNode *root) {
   printBSTContents(root);
   printf("|\n\n");
}

void LZWCmpInit(LZWCmp *cmp, CodeSink sink, void *sinkState, int recycleCode,
 int traceFlags) {
   void *cs = CreateCodeSet(recycleCode);
   TreeNode *bst = BSTCreate();

   int i = 0;
   for (; i < 256; i++) {
      NewCode(cs, i);
      bst = BSTInsert(i, bst, cs);
   }

   BSTPrint(bst);
}

