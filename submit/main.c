#include <stdio.h>
#include <stdlib.h>
#define N 1000
#define max(a, b) (((a) > (b)) ? (a) : (b))

typedef struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
    int height;
}Node;

typedef struct
{
    Node* data[N];
    int top;
}StackType;

void init(StackType *S)
{
    S-> top = -1;
}

int isEmpty(StackType *S)
{
    return S-> top == -1;
}

int isFull(StackType *S)
{
    return S->top == N-1;
}

void push(StackType *S, Node* c)
{
    if(!isFull(S))
    {
        S-> top++;
        S-> data[S->top] = c;
    }
}

Node* pop(StackType *S)
{
    if(isEmpty(S))
        return NULL;

    Node* c = S->data[S->top];
    S->top--;
    return c;
}

int Height(Node* node)
{
    if(node == NULL)
        return 0;
    int left = Height(node->left);
    int right = Height(node->right);

    return 1 + max(left,right);
}

int size(Node* node){
    if (node == NULL)
        return 0;
    int left = size(node->left);
    int right = size(node->right);

    return left + right + 1;
}

void inOrder(Node* root)
{
    if(root != NULL)
    {
        printf("<");
        inOrder(root->left);
        printf(" %d ", root->key);
        inOrder(root->right);
        printf(">");
    }
}

void clear(Node* node){
    node->left = NULL;
    node->right = NULL;
    free(node);
}

Node* getNodeBST(){
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = 0;
    node->left = node->right = NULL;
    node->height = 0;

    return node;
}

Node* minNode(Node* root, StackType* stack){
    while(root->left != NULL){
        push(stack, root);
        root = root->left;
    }
    return root;
}

Node* maxNode(Node* root, StackType* stack){
    while(root->right != NULL){
        push(stack, root);
        root = root->right;
    }
    return root;
}

void clearNode(Node* node){
    if (node== NULL)
        return;
    if (node->left != NULL)
        clearNode(node->left);
    if (node->right != NULL)
        clearNode(node->right);
    node->left = NULL;
    node->right = NULL;
    free(node);
    return;
}

void insertBST(Node** node, int key){
    Node *p = *node;
    Node *q = NULL;
    StackType* stack = (StackType*)malloc(sizeof(StackType));
    init(stack);

    while (p != NULL){
        if(key == p->key){
            printf("i %d: The key already exists", key);
            free(stack);
            free(q);
            return;
        }else{
            q = p;
            push(stack, q);
            if (key < p->key)
                p = p->left;
            else
                p = p->right;
        }
    };
    
    Node* newNode = getNodeBST();
    newNode->key = key;

    if (*node == NULL)
        *node = newNode;
    else if (key < q->key)
        q->left = newNode;
    else
        q->right = newNode;

    while (isEmpty(stack) != 1){
        q = pop(stack);
        q->height = Height(q);
    }

    free(stack);
    inOrder(*node);
}

void deleteBST(Node** node, int key){
    Node *p = *node;
    Node *q = NULL;
    StackType* stack = (StackType*)malloc(sizeof(StackType));
    init(stack);
    Node* tempNode;
    while (p != NULL && key != p->key){
        q = p;
        push(stack, q);
        if (key < p->key)
            p = p->left;
        else
            p = p->right;
    }

    if (p == NULL){
        printf("d %d: The key does not exist", key);
        free(stack);
        free(p);
        return;
    }
    if (p->left != NULL && p->right != NULL){
        push(stack, p);
        tempNode = p;

        if (p->left->height < p->right->height){
            p = minNode(p->right, stack);
        }
        else if(p->left->height < p->right->height){
            p = maxNode(p->left, stack);
        }
        else{
            if(size(p->left) >= size(p->right))
                p = maxNode(p->left, stack);
            else
                p = minNode(p->right, stack);
        }

        tempNode->key = p->key;
        q = pop(stack);
    }
    
    if (p->left == NULL && p->right == NULL) {
        if (q == NULL)
            *node = NULL;
        else if (q->left == p)
            q->left = NULL;
        else
            q->right = NULL;
    } else {
        tempNode = (p->left != NULL) ? p->left : p->right;
        if (q == NULL)
            *node = tempNode;
        else if (q->left == p)
            q->left = tempNode;
        else
            q->right = tempNode;
    }
    
    clear(p);

    while(isEmpty(stack) != 1){
        q = pop(stack);
        q->height = Height(q);
    }
    
    free(stack);
    inOrder(*node);
}

int main()
{
    Node* root = NULL;
    int key;
    char strTemp[20];
    
    while (fgets(strTemp, sizeof(strTemp), stdin) != NULL) {
        key = 0;
        int j = 2;
        while (strTemp[j] > 47 && strTemp[j] < 58){
            key = key * 10 + (strTemp[j] - '0');
            j++;
        }
        if (strTemp[0] == 'i')
            insertBST(&root, key);
        else if (strTemp[0] == 'd')
            deleteBST(&root, key);
        else
            continue;
        printf("\n");
    }
    clearNode(root);
    return 0;
}
