#include <stdio.h>
#include <string.h>
#include <stdlib.h>
FILE * infile; FILE * outfile;
typedef int bool;
#define true 1
#define false 0

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
bool keyExists(Node *root, int key) {
    while(true){
        if (!root) return false;
        if(key == root->key) return true;
        else if (key > root->key) root=root->right;
        else root=root->left;
    }
}

Node* push(Node *node, int key) {
    if (keyExists(node, key)) {
        fprintf(outfile, "Exist number\n");
        return node;  // 중복된 키에 대한 처리
    }

    if (node == NULL){
        Node* newNode = createNode(key);
        fprintf(outfile, "Push %d\n", key);
        return newNode;
    }
    if (key < node->key) node->left = push(node->left, key);
    else node->right = push(node->right, key);
    return node;
}

void top(Node *node){
    if (node == NULL){
        fprintf(outfile, "The queue is empty\n");
        return;
    }
    while (node->right){
        node = node->right;
    }
    fprintf(outfile, "The top is %d\n", node->key);
}
void pop(Node **root)
{   
    if (*root == NULL){
        fprintf(outfile, "The queue is empty\n");
        return;
    }
    Node *current = *root;
    Node *parent = NULL;

    while (current->right){
        parent=current;
        current = current->right;
    }
    if(parent != NULL) parent->right = NULL;
    else *root = current->left;
    fprintf(outfile,"Pop %d\n", current->key);
    free(current);
    return;
}

int main() {
    infile = fopen("input3.txt", "r");
    outfile = fopen("output3.txt", "w");
    char str[100];
    int key;
    Node * root=NULL;

    while(1){
        fscanf(infile, "%s", str);
        if(strcmp(str,"q")==0) break;
        else if(strcmp(str,"push")==0){
            fscanf(infile, "%d", &key);
            root = push(root, key);
        }
        else if(strcmp(str,"top")==0){
            top(root);
        }
        else if(strcmp(str,"pop")==0){
            pop(&root);
        }
    }
    fclose(infile);
    fclose(outfile);
    return 0;
}