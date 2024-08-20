#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 100

typedef enum { lparen, rparen, plus, minus, times, divide, mod, eos, operand, unary } precedence;

void push(int stack[], int *top, int item) {
    stack[++(*top)] = item;
}

int pop(int stack[], int *top) {
    if (*top == -1) {
        fprintf(stderr, "Error: Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    return stack[(*top)--];
}

precedence getToken(char *symbol, int *n, char* input) {
    *symbol = input[(*n)--];
    switch (*symbol) {
        case '(': return rparen; //
        case ')': return lparen; //
        case '+': return plus;
        case '-': return minus;
        case '/': return divide;
        case '*': return times;
        case '%': return mod;
        case '\0': return eos;
        default: return operand;
    }
}

char printToken(precedence token) {
    switch (token) {
        case plus: return '+';
        case minus: return '-';
        case times: return '*';
        case divide: return '/';
        case mod: return '%';
        default: return ' '; 
    }
}

void prefix(char *input, char *prefix_exp) {
    int isp[] = {0, 19, 12, 12, 13, 13, 13, 0};
    int icp[] = {20, 19, 12, 12, 13, 13, 13, 0};
    char symbol;
    precedence token;
    int n = strlen(input) - 1;
    int top = -1;
    int stack[MAX_STACK_SIZE];
    stack[++top] = eos;
    char output[100];
    int outputIndex = 0;

    while (n >= 0) {
        token = getToken(&symbol, &n, input);
        if (token == operand) {
            output[outputIndex++] = symbol;
        } else if (token == lparen) {  // 이 부분이 ')'로 처리됨
            while (top != -1 && stack[top] != rparen) {  // ')'를 찾을 때까지 pop
                output[outputIndex++] = printToken((precedence)pop(stack, &top));
            }
            if (top == -1) {
                fprintf(stderr, "Error: Unbalanced parentheses\n");
                exit(EXIT_FAILURE);
            }
            pop(stack, &top);  // ')' 제거
        } else {
            while (top != -1 && isp[stack[top]] >= icp[token]) {
                output[outputIndex++] = printToken((precedence)pop(stack, &top));
            }
            push(stack, &top, token);
        }
    }

    while (top != -1) {
        output[outputIndex++] = printToken((precedence)pop(stack, &top));
    }
    output[outputIndex] = '\0';

    for (int i = 0; i < outputIndex; i++) {
        prefix_exp[i] = output[outputIndex - i - 1];
    }
    prefix_exp[outputIndex] = '\0';
}


int main(void) {
    char input[100];
    char prefix_exp[100];

    //int stack[MAX_STACK_SIZE];
    //stack[++top] = eos;

    printf("Infix: ");
    scanf("%s", input);
    /*fgets(input, 100, stdin);  //
    input[strcspn(input, "\n")] = 0;  //
    */
    prefix(input, prefix_exp);
    printf("Prefix: %s\n", prefix_exp);
    return 0;
}