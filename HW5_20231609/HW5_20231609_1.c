#include <stdio.h>
#include <stdlib.h>
typedef int bool;
#define true 1
#define false 0

FILE * infile; FILE * outfile;

typedef struct node *treePointer;
typedef struct node {
    int key;
    treePointer parent;
    treePointer leftChild, rightChild;
} Node;

void swap(Node *a, Node *b) {
    int temp = a->key;
    a->key = b->key;
    b->key = temp;
}

Node* findInsertPosition(Node *root){
    if (!root) return NULL;
    Node *temp;
    Node **queue = malloc(sizeof(Node*) * 10000); // Sufficiently large queue
    int front = 0, rear = 0;
    queue[rear++] = root;
    while (front < rear) {
        temp = queue[front++];
        if (!temp->leftChild || !temp->rightChild) {
            free(queue);
            return temp;
        }
        if (temp->leftChild) queue[rear++] = temp->leftChild;
        if (temp->rightChild) queue[rear++] = temp->rightChild;
        free(queue);
        return NULL;
    }
    
}

Node* findLastNode(Node *root) {
    if (!root) return NULL;  // 빈 트리인 경우 NULL 반환

    Node **queue = malloc(sizeof(Node*) * 10000);  // 큰 큐를 생성
    int front = 0, rear = 0;  // 큐 초기화
    queue[rear++] = root;  // 루트 노드를 큐에 삽입

    Node *lastNode = NULL;
    while (front < rear) {
        lastNode = queue[front++];  // 큐에서 노드를 하나 꺼냄

        if (lastNode->leftChild)  // 왼쪽 자식이 있으면 큐에 추가
            queue[rear++] = lastNode->leftChild;
        if (lastNode->rightChild)  // 오른쪽 자식이 있으면 큐에 추가
            queue[rear++] = lastNode->rightChild;
    }

    free(queue);  // 큐 사용 후 메모리 해제
    return lastNode;  // 마지막으로 큐에서 꺼낸 노드 반환
}

void heapifyUp(Node *node){
    while (node->parent && node->key > node->parent->key) {
        int temp = node->key;
        node->key = node->parent->key;
        node->parent->key = temp;
        node = node->parent;
    }
}
void heapifyDown(Node* node){
    while(node){
        Node *largest=node;
        Node *left = node->leftChild;
        Node *right = node->rightChild;
        if (left && left->key > largest->key) {
            largest = left;
        }
        if (right && right->key > largest->key) {
            largest = right;
        }
        if (largest != node) {
            swap(node, largest);
            node = largest;
        } else {
            break;
        }
    }
}

bool keyExists(Node *root, int key) {
    if (!root) return false;
    Node **queue = malloc(sizeof(Node*) * 10000); // 가정: 충분히 큰 큐
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        Node *current = queue[front++];
        if (current->key == key) {
            free(queue);
            return true;  // 키를 찾았을 경우
        }
        if (current->leftChild) queue[rear++] = current->leftChild;
        if (current->rightChild) queue[rear++] = current->rightChild;
    }

    free(queue);
    return false;  // 키를 찾지 못했을 경우
}

Node* insert(Node *root, int key)
{   
    if (keyExists(root, key)) {
        fprintf(outfile, "Existing number\n");
        return root;  // 중복된 키에 대한 처리
    }
    Node *newNode=malloc(sizeof(Node));
    newNode->key = key;
    newNode->parent = NULL;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;

    if (!root) {
        root=newNode;
        fprintf(outfile, "Insert %d\n", key);
        return root;
    }
    Node *parent = findInsertPosition(root);
    if (!parent->leftChild) {
        parent->leftChild = newNode;
    } else {
        parent->rightChild = newNode;
    }
    newNode->parent = parent;

    heapifyUp(newNode);
    fprintf(outfile, "Insert %d\n", key);
    return root;
}
Node* pop(Node **root)
{   
    if (!*root){
        fprintf(outfile,"The heap is empty\n");
        return NULL;
    }
    Node *rootNode = *root;
    if(!rootNode->leftChild && !rootNode->rightChild){
        *root = NULL;
        fprintf(outfile,"Delete %d\n", rootNode->key);
        free(rootNode);
        return NULL;
    }
    Node *last=findLastNode(*root);
    fprintf(outfile, "Delete %d\n", rootNode->key);
    swap(rootNode, last);
    if (last->parent) {
        if (last->parent->leftChild == last)
            last->parent->leftChild = NULL;
        else
            last->parent->rightChild = NULL;
    }
    free(last);
    heapifyDown(*root);

    return *root;
}

int main(void){
    Node *root=NULL;
    char ch;
    int key;
    infile = fopen("input1.txt", "r");
    outfile = fopen("output1.txt", "w");

    while((ch=fgetc(infile))!=EOF){
        if (ch=='q') break;
        else if(ch=='i'){
            fscanf(infile, "%d", &key);
            root=insert(root, key);
        }
        else if(ch=='d'){
            root=pop(&root);
        }
    }
    fclose(infile);
    fclose(outfile);
    return 0;
}