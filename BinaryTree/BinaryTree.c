#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <queue>

struct node {
   int data;
    
   struct node *leftChild;
   struct node *rightChild;
};

struct node *root = NULL;

void insert(int data) {
   struct node *tempNode = (struct node*) malloc(sizeof(struct node));
   struct node *current;
   struct node *parent;

   tempNode->data = data;
   tempNode->leftChild = NULL;
   tempNode->rightChild = NULL;

   //if tree is empty
   if(root == NULL) {
      root = tempNode;
   } else {
      current = root;
      parent = NULL;

      while(1) {
         parent = current;
         
         //go to left of the tree
         if(data < parent->data) {
            current = current->leftChild;                
            
            //insert to the left
            if(current == NULL) {
               parent->leftChild = tempNode;
               return;
            }
         }//go to right of the tree
         else {
            current = current->rightChild;

            //insert to the right
            if(current == NULL) {
               parent->rightChild = tempNode;
               return;
            }
         }
      }            
   }
}

struct node* search(int data) {
   struct node *current = root;
   printf("Visiting elements: ");

   while(current->data != data) {
      if(current != NULL)
         printf("%d ",current->data);

      //go to left tree
      if(current->data > data) {
         current = current->leftChild;
      }
      //else go to right tree
      else {                
         current = current->rightChild;
      }

      //not found
      if(current == NULL) {
         return NULL;
      }
   }
   
   return current;
}

void pre_order_traversal(struct node* root) {
   if(root != NULL) {
      printf("%d ",root->data);
      pre_order_traversal(root->leftChild);
      pre_order_traversal(root->rightChild);
   }
}

void inorder_traversal(struct node* root) {
   if(root != NULL) {
      inorder_traversal(root->leftChild);
      printf("%d ",root->data);          
      inorder_traversal(root->rightChild);
   }
}

void post_order_traversal(struct node* root) {
   if(root != NULL) {
      post_order_traversal(root->leftChild);
      post_order_traversal(root->rightChild);
      printf("%d ", root->data);
   }
}

/* Compute the "height" of a tree -- the number of
    nodes along the longest path from the root node
    down to the farthest leaf node.*/
int height(struct node* node)
{
    if (node==NULL)
        return 0;
    else
    {
        /* compute the height of each subtree */
        int lheight = height(node->leftChild);
        int rheight = height(node->rightChild);
 
        /* use the larger one */
        if (lheight > rheight)
            return(lheight+1);
        else return(rheight+1);
    }
}

/* Print nodes at a given level */
void printGivenLevel(struct node* root, int level)
{
    if (root == NULL)
        return;
    if (level == 1)
        printf("%d ", root->data);
    else if (level > 1)
    {
        printGivenLevel(root->leftChild, level-1);
        printGivenLevel(root->rightChild, level-1);
    }
}
 
/* Function to print level order traversal a tree*/
void level_order_traversal(struct node* root)
{
    int h = height(root);
    int i;
    for (i=1; i<=h; i++){
        printGivenLevel(root, i);
        printf("-");
    }
}

/* implement an Iterator class to sort */
template <class T>
class Iterator
{
    public:    
    Iterator(){};
    ~Iterator(){};
    
     public:
     bool hasNext(){return false;};
     T  next(){return NULL;};
};

class InOrderNodeIterator: public Iterator <node*>
{
      public:
       InOrderNodeIterator(node* root);
       ~InOrderNodeIterator(){};
      public:
        bool hasNext();
        node* next();
        int  maxStackDepth(){ return stackDepth;};
      protected:
        void  initStack(node*);
      private:
        std::stack <node*> mStack;
        int stackDepth;
};

InOrderNodeIterator::InOrderNodeIterator(node* root)
{
   stackDepth = 0;
   initStack(root);
}

bool InOrderNodeIterator::hasNext()
{
     return !mStack.empty();
}

node* InOrderNodeIterator::next()
{
    /* check the right child and go to very left of the node */
    node* pNode = mStack.top();
    mStack.pop();

    if(pNode->rightChild){
        initStack(pNode->rightChild);
    }
    
    return pNode;
}  

/* protected */
void InOrderNodeIterator::initStack(node* root)
{
    mStack.push(root);
    
    while(root->leftChild){
        mStack.push(root->leftChild);
        root = root->leftChild;
    }
    
    if(mStack.size() > stackDepth)
        stackDepth = mStack.size();
}

/* pre-order Iterator */
class PreOrderNodeIterator: public Iterator <node*>
{
      public:
       PreOrderNodeIterator(node* pRoot);
       ~PreOrderNodeIterator(){};
      public:
        bool hasNext();
        node* next();
        int  maxStackDepth(){ return stackDepth;};
      protected:
        void  initStack(node* pNode);
      private:
        std::stack <node*> mStack;
        int stackDepth;
};

PreOrderNodeIterator::PreOrderNodeIterator(node* root)
{
    stackDepth = 2;
    mStack.push(root);
}

bool PreOrderNodeIterator::hasNext()
{
     return !mStack.empty();
}

node* PreOrderNodeIterator::next()
{
   /* check the right child and go to very left of the node */
   node* pTmp = mStack.top();
    mStack.pop();
    
    if(pTmp!= NULL){       
         if(pTmp->rightChild){
            mStack.push(pTmp->rightChild);
         }
         if(pTmp->leftChild){
             mStack.push(pTmp->leftChild);
         }
    }
    
    return pTmp;

}  

/* protected */
void PreOrderNodeIterator::initStack(node* root)
{
    /* don't need it */
}

/* post-order Iterator */
class PostOrderNodeIterator: public Iterator <node*>
{
      public:
       PostOrderNodeIterator(node* pRoot);
       ~PostOrderNodeIterator(){};
      public:
        bool hasNext();
        node* next();
        int  maxStackDepth(){ return stackDepth;};
      protected:
        void  initStack(node* pNode);
      private:
        std::stack <node*> mStack;
        int stackDepth;
};

PostOrderNodeIterator::PostOrderNodeIterator(node* root)
{
    stackDepth = 0;
    if(root){
        mStack.push(root);
        initStack(root);
    }
}

bool PostOrderNodeIterator::hasNext()
{
     return !mStack.empty();
}

node* PostOrderNodeIterator::next()
{
    /* check the right child and go to very left of the node */
    node* pNode = mStack.top();
    mStack.pop();
    
    if(mStack.empty())
        return pNode;
    
    node* pCur = mStack.top();
    
    if(!pCur || pCur->leftChild == pNode || pCur->rightChild == pNode)
        return pNode;
    
    initStack(pCur);
    
    return pNode;
}  

/* protected */
void PostOrderNodeIterator::initStack(node* root)
{
    node* pNode = NULL;
    
    while(root){
        pNode = NULL;
        if(root->rightChild){
            mStack.push(root->rightChild);
            pNode = root->rightChild;
        }    
        if(root->leftChild){
            mStack.push(root->leftChild);
            pNode = root->leftChild;
        }
        
        root = pNode;
    }

    if(mStack.size() > stackDepth)
        stackDepth = mStack.size();
}

/* level-order Iterator */
class LevelOrderNodeIterator: public Iterator <node*>
{
      public:
       LevelOrderNodeIterator(node* pRoot);
       ~LevelOrderNodeIterator(){};
      public:
        bool hasNext();
        node* next();
        int  maxStackDepth(){ return stackDepth;};
      protected:
        void  initStack(node* pNode);
      private:
        std::queue <node*> mStack;
        int stackDepth;
};

LevelOrderNodeIterator::LevelOrderNodeIterator(node* root)
{
    stackDepth = 0;
    if(root){
        mStack.push(root);
        stackDepth = 1;
    }
}

bool LevelOrderNodeIterator::hasNext()
{
     return !mStack.empty();
}

node* LevelOrderNodeIterator::next()
{
    /* check the right child and go to very left of the node */
    node* pNode = mStack.front();
    mStack.pop();
    
    if(pNode->leftChild) mStack.push(pNode->leftChild);
    if(pNode->rightChild) mStack.push(pNode->rightChild);

    if(mStack.size() > stackDepth)
        stackDepth = mStack.size();
    
    return pNode;
}  

/* protected */
void LevelOrderNodeIterator::initStack(node* root)
{
    if(mStack.size() > stackDepth)
        stackDepth = mStack.size();
}

#define MAX 20
int main() {
   int i;
   int array[MAX] = { 27, 14, 35, 15, 36,34,99, 25, 12 ,45,89, 54,38,24, 66, 96, 37, 11, 55,32};

   for(i = 0; i < MAX; i++)
      insert(array[i]);

   i = 31;
   struct node * temp = search(i);

   if(temp != NULL) {
      printf("[%d] Element found.", temp->data);
      printf("\n");
   }else {
      printf("[ x ] Element not found (%d).\n", i);
   }

   i = 15;
   temp = search(i);

   if(temp != NULL) {
      printf("[%d] Element found.", temp->data);
      printf("\n");
   }else {
      printf("[ x ] Element not found (%d).\n", i);
   }            

   printf("\nPreorder traversal: ");
   pre_order_traversal(root);

   printf("\nInorder traversal: ");
   inorder_traversal(root);

   printf("\nPost order traversal: ");
   post_order_traversal(root);       

   printf("\nLevel order traversal: ");
   level_order_traversal(root);       
   
   printf("\nPreorder traversal w Iterator:\n[ ");
   PreOrderNodeIterator preIter(root);
   while(preIter.hasNext()){
       printf("%d ", preIter.next()->data);
   }
   printf("]\n");
   printf("MaxStackDepth: %d\n", preIter.maxStackDepth());

   printf("\nInorder traversal w Iterator:\n[ ");
   InOrderNodeIterator inIter(root);
   while(inIter.hasNext()){
       printf("%d ", inIter.next()->data);
   }
   printf("]\n");
   printf("MaxStackDepth: %d\n", inIter.maxStackDepth());
    
   printf("\nPostorder traversal w Iterator:\n[ ");
   PostOrderNodeIterator postIter(root);
   while(postIter.hasNext()){
       printf("%d ", postIter.next()->data);
   }
   printf("]\n");
   printf("MaxStackDepth: %d\n", postIter.maxStackDepth());

   printf("\nLevel order traversal w Iterator:\n[ ");
   LevelOrderNodeIterator levelIter(root);
   while(levelIter.hasNext()){
       printf("%d ", levelIter.next()->data);
   }
   printf("]\n");
   printf("MaxStackDepth: %d\n", levelIter.maxStackDepth());
    
   return 0;
}