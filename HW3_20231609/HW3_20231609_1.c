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
        fprintf(stderr, "Error: No elements to pop from stack\n");
        exit(EXIT_FAILURE); 
    }
    return stack[(*top)--];
}

precedence getToken(char *symbol, int *n, char* input, precedence lastToken) {
    *symbol = input[(*n)++];
    switch (*symbol) {
        case '(': return lparen;
        case ')': return rparen;
        case '+': return plus;
        case '-': 
            if (lastToken == eos || lastToken == lparen || lastToken == plus || lastToken == minus ||
                lastToken == times || lastToken == divide || lastToken == mod || lastToken == unary) {
                return unary;
            } else {
                return minus;
            }
        case '/': return divide;
        case '*': return times;
        case '%': return mod;
        case '#': return unary;
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
        case unary: return '#';
        default: return ' '; 
    }
}

void postfix(char *input, char *postfix_exp) {
    int isp[] = { 0, 19, 12, 12, 13, 13, 13, 0, 15};
    int icp[] = { 20, 19, 12, 12, 13, 13, 13, 0, 14};
    char symbol;
    precedence token, lastToken =eos;
    int n = 0;
    int top = -1;
    int stack[MAX_STACK_SIZE];
    stack[++top] = eos;
    int postfix_index = 0;

    while ((token = getToken(&symbol, &n, input, lastToken)) != eos){
        if (token == operand) {
            postfix_exp[postfix_index++] = symbol;
            if (lastToken == unary) {
                postfix_exp[postfix_index++] = '#'; 
            }
        }
        else if (token == unary) {
            lastToken=unary;
            continue;
        } 
        else if (token == rparen) {
            while (stack[top] != lparen) {
                postfix_exp[postfix_index++] = printToken((precedence)pop(stack, &top));
            }
            pop(stack, &top);
        } else {
            while (isp[stack[top]] >= icp[token]) {
                postfix_exp[postfix_index++] = printToken((precedence)pop(stack, &top));
            }
            push(stack, &top, token);
        }
        lastToken = token;
    }
    while ((token = (precedence)pop(stack, &top)) != eos) {
        postfix_exp[postfix_index++] = printToken(token);
    }
    postfix_exp[postfix_index] = '\0';
}

int eval(char *postfix) {
    int stack[MAX_STACK_SIZE];
    int top = -1;
    int i = 0;
    char symbol;
    int op1, op2;

    while (postfix[i] != '\0') {
        symbol = postfix[i];
        if (isdigit(symbol)) {
            push(stack, &top, symbol - '0');  
        } else if (symbol == '#') {
            op1 = pop(stack, &top);
            push(stack, &top, -op1); 
        } else {
            op2 = pop(stack, &top);
            op1 = pop(stack, &top);
            switch (symbol) {
                case '+': push(stack, &top, op1 + op2); break;
                case '-': push(stack, &top, op1 - op2); break;  
                case '*': push(stack, &top, op1 * op2); break;
                case '/': push(stack, &top, op1 / op2); break;
            }
        }
        i++;
    }
    return pop(stack, &top);  
}


int main(void) {
    char input[100];
    char postfix_exp[100];

    //int stack[MAX_STACK_SIZE];
    //stack[++top] = eos;

    printf("Input: ");
    scanf("%s", input);

    postfix(input, postfix_exp);
    printf("Postfix: %s\n", postfix_exp);
    printf("Result: %d\n", eval(postfix_exp));

    return 0;
}
