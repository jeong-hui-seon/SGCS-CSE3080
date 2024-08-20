#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
  int data;
  struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
} Stack;

void init_stk(Stack* stk) {
    stk->top = NULL;
}

/*void print_stk(Stack* stk) {
    Node* current = stk->top;
    while(current != NULL){
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}*/

void push(Stack* stk, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        return;
    }
    newNode->data = data;
    newNode->next = stk->top;
    stk->top = newNode;
}

int pop(Stack* stk) {
    if (stk->top == NULL) {
        return -1;
    }
    int data = stk->top->data;
    Node* temp = stk->top;
    stk->top = stk->top->next;
    free(temp);
    return data;
}

int is_empty(Stack* stk) {
    return stk->top == NULL;
}

void print_stk(Stack* stk) {  
    /*if (stk->top != NULL) {
        int data = pop(stk);
        print_stk(stk);
        printf("%d", data);
        push(stk, data);
    }*/
    int Result=0;
    int i=1;
    while(stk->top != NULL){
        Result+=i*pop(stk);
        i*=10;
    }
    printf("%d",Result);
}

void delete_num(char* num, int k){
    int len = strlen(num);
    Stack stk;
    init_stk(&stk);

    for (int i = 0; i < len; i++) {
        int current_digit = num[i] - '0';
        while (!is_empty(&stk) && stk.top->data > current_digit && k > 0) {
            pop(&stk);
            k--;
        }
        push(&stk, current_digit);
    }
    //printf("Result: ");
    print_stk(&stk);
    printf("\n");
}

int main(void){
    char*num = NULL;
    int k;
    size_t size = 0;

    //printf("Enter num: ");
    getline(&num, &size, stdin);
    num[strcspn(num, "\n")] = '\0';
    
    //printf("Enter k: ");
    scanf("%d", &k);

    delete_num(num, k);
    free(num);
    return 0;
}