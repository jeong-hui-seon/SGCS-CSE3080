#include <stdio.h>
#include <stdlib.h>
FILE * infile; FILE * outfile;

typedef struct node {
    int key;
    struct node *left, *right;
} Node;

Node* createNode(int key) {
    Node *newNode = malloc(sizeof(Node));
    if (!newNode) return NULL;
    newNode->key = key;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* insert(Node *node, int key) {
    if (node == NULL) return createNode(key);
    if (key < node->key) node->left = insert(node->left, key);
    else node->right = insert(node->right, key);
    return node;
}

Node* constructBST(int pre[], int size) {
    if (size == 0) return NULL;
    Node *root = createNode(pre[0]);  // 루트 노드 생성
    for (int i = 1; i < size; i++) {
        insert(root, pre[i]);  // 나머지 노드 삽입
    }
    return root;
}

void preorderTraversal(Node *node) {
    if (node == NULL) return;
    printf("%d ", node->key);
    preorderTraversal(node->left);
    preorderTraversal(node->right);
}
void inorderTraversal(Node *node) {
    if (node == NULL) return;
    inorderTraversal(node->left);
    fprintf(outfile, "%d ", node->key);
    inorderTraversal(node->right);
}
void postorderTraversal(Node *node) {
    if (node == NULL) return;
    postorderTraversal(node->left);
    postorderTraversal(node->right);
    fprintf(outfile, "%d ", node->key);
}

int main() {
    infile = fopen("input2.txt", "r");
    outfile = fopen("output2.txt", "w");
    int size;
    fscanf(infile, "%d", &size);
    int *pre =(int*)malloc(sizeof(int)*size);
    for(int i=0; i<size; i++){
        fscanf(infile, "%d", &pre[i]);
    }

    int flag=0;
    for(int i=0; i<size; i++){
        for(int j=0; j<i; j++){
            if(pre[i]==pre[j]){
                fprintf(outfile, "cannot construct BST");
                return 0;
            }
        }
    }

    Node *root = constructBST(pre, size);

    fprintf(outfile, "Inorder: ");
    inorderTraversal(root);
    fprintf(outfile, "\n");

    fprintf(outfile, "Postorder: ");
    postorderTraversal(root);
    fprintf(outfile, "\n");

    fclose(infile);
    fclose(outfile);
    return 0;
}
