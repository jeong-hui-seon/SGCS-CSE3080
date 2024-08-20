#include <stdio.h>
#include <stdlib.h>

typedef struct _polyNode *poly_pointer;
typedef struct _polyNode{
    int coef;
    int expon;
    poly_pointer link;
} polyNode;

void attach(int coefficient, int exponent, poly_pointer* ptr){
    poly_pointer temp;
    temp = (poly_pointer)malloc(sizeof(polyNode));
    temp->coef = coefficient;
    temp->expon = exponent;
    temp->link=NULL;
    if((*ptr)==NULL) *ptr=temp;
    else{
        (*ptr)->link = temp;
        (*ptr)=temp;
    }
}

poly_pointer makeP(FILE * f) {
    poly_pointer p=NULL, tail=NULL;
    int coef, expon;
    int num;
    fscanf(f, "%d", &num);
    for(int i=0; i<num; i++){
        fscanf(f, "%d %d", &coef, &expon);
        attach(coef, expon, &tail);
        if(i==0) p=tail;
    }
    tail->link=NULL;
    return p;
}

int COMPARE(int a, int b){
    if(a<b) return -1;
    else if (a==b) return 0;
    else return 1;
}

poly_pointer padd(poly_pointer a, poly_pointer b)
{
/* return a polynomial which is the sum of a and b */
    poly_pointer c, rear, temp;
    int sum;
    rear = (poly_pointer) malloc(sizeof(polyNode));
    c = rear;
    while (a && b){
        switch (COMPARE(a->expon, b->expon)){
        case -1 : /* a->expon < b->expon */
            attach (b->coef, b->expon, &rear);
            b = b->link; break;
        case 0 : /* a->expon = b->expon */
            sum = a->coef + b->coef;
            if (sum) attach(sum, a->expon, &rear);
            a = a->link; b = b->link; break;
        case 1 : /* a->expon > b->expon */
            attach (a->coef, a->expon, &rear);
            a = a->link;
        }
    }
    /* copy rest of list a and then list b */
    for ( ; a; a = a->link) attach (a->coef, a->expon, &rear);
    for ( ; b; b = b->link) attach (b->coef, b->expon, &rear);
    rear->link = NULL;
    /* delete extra initial node */
    temp = c; c = c->link; free(temp);
    return c;
}

poly_pointer pmult(poly_pointer a, poly_pointer b) {
    if(!a || !b) return NULL;
    
    poly_pointer result=NULL;
    while (a!=NULL){
        poly_pointer temp=NULL, cur, temp_cur=temp;
        cur=b;
        while (cur!=NULL){
            attach ((a->coef)*(cur->coef), (a->expon)+(cur->expon), &temp_cur);
            cur = cur->link;
            if(temp==NULL) temp=temp_cur;
        }
        if (result == NULL) {
            result=temp;
        }
        else {
            //print_poly(result); printf("+\n");print_poly(temp); printf("=\n");
            result = padd(result, temp);
            //print_poly(result);
        }
        a=a->link;
    }
    return result;
}

void pwrite(poly_pointer p, FILE * f){
    poly_pointer cur=p;
    int num=0;
    int coef, expon;
    while(1){
        num++;
        if(cur->link==NULL) break;
        cur=cur->link;
    }
    fprintf(f, "%d\n", num);
    cur=p;
    while(cur != NULL){
        fprintf(f, "%d %d\n", cur->coef, cur->expon);
        cur=cur->link;
    }
}

int main(void){
    poly_pointer a,b,d;

    FILE* fileA; FILE* fileB; FILE* fileD;
    fileA = fopen("a.txt", "r");
    fileB = fopen("b.txt", "r");

    a = makeP(fileA); b = makeP(fileB);
    fclose(fileA); fclose(fileB);

    d = pmult(a,b);
    fileD = fopen("d.txt", "w");
    pwrite(d, fileD);
    fclose(fileD);
}