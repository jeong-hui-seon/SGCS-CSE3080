#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 50 /* size of largest matrix */
typedef enum {head, entry} tagfield;
typedef struct matrix_node *matrix_pointer;

typedef struct entry_node {
int row;
int col;
int value;
};

typedef struct _node * node_ptr;
typedef struct _node{
    int row;
    int col;
    int val;
    node_ptr nxt;
} Node;

typedef struct matrix_node {
matrix_pointer down;
matrix_pointer right;
tagfield tag;
union {
matrix_pointer next;
struct entry_node entry;
} u;
};

matrix_pointer hdnode[MAX_SIZE];
//matrix_pointer hdnode_t[MAX_SIZE];

matrix_pointer new_node(void){
    matrix_pointer temp = (matrix_pointer)malloc(sizeof(struct matrix_node));
    temp->down = NULL;
    temp->right = NULL;
    return temp;
}

matrix_pointer mread(FILE * file)
{ /* read in a matrix and set up its linked representation. An auxiliary global array hdnode is used */
    int num_rows, num_cols,num_terms, num_heads, i;
    int row, col, value, current_row;
    matrix_pointer temp, last, node;
    
    fscanf(file, "%d %d %d", &num_rows, &num_cols, &num_terms);
    num_heads = (num_cols > num_rows) ? num_cols : num_rows;
/* set up header node for the list of header nodes */
    node = new_node(); node->tag = entry;
    node->u.entry.row = num_rows;
    node->u.entry.col = num_cols;
    node->u.entry.value = num_terms;

    if (!num_heads) node->right = node;
    else { /* initialize the header nodes */
        for (i=0; i<num_heads; i++) {
            temp = new_node();
            hdnode[i] = temp; hdnode[i]->tag = head;
            hdnode[i]->right = temp; hdnode[i]->u.next=temp;
        }
        current_row = 0;
        last = hdnode[0]; /* last node in current row */
        for (i=0; i<num_terms; i++) {
            fscanf(file, "%d %d %d", &row, &col, &value);
            if (row > current_row) { /* close current row */
                last->right = hdnode[current_row];
                current_row = row; last = hdnode[row];
            }
            temp = new_node(); temp->tag = entry;
            temp->u.entry.row = row; temp->u.entry.col = col;
            temp->u.entry.value = value;
            last->right = temp; /* link into row list */
            last = temp;
            hdnode[col]->u.next->down = temp; /* link into column list */
            hdnode[col]->u.next = temp;
        }
        /* close last row */
        last->right = hdnode[current_row];
        /* close all column lists */
        for (i=0; i<num_cols; i++)
            hdnode[i]->u.next->down = hdnode[i];
        /* link all header nodes together */
        for (i=0; i<num_heads-1; i++)
            hdnode[i]->u.next = hdnode[i+1];
        hdnode[num_heads-1]->u.next = node;
        node->right = hdnode[0];
    }
    return node;
}

void mwrite(matrix_pointer node, FILE * file)
{ /* print out the matrix in row major form */
    int i;
    matrix_pointer temp, head=node->right;
    /* matrix dimensions */
    fprintf(file, "%d %d %d\n", node->u.entry.row, node->u.entry.col, node->u.entry.value);
    for(i=0; i<node->u.entry.row; i++) {
    /* print out the entries in each row */
        for(temp=head->right; temp!=head; temp=temp->right)
            fprintf(file, "%d %d %d\n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
        head = head->u.next; /* next row */
    }
}

void merase(matrix_pointer *node)
{ /* erase the matrix, return the nodes to the heap */
    matrix_pointer x,y, head = (*node)->right;
    int i;
    /* free the entry and header nodes by row */
    for (i=0; i<(*node)->u.entry.row; i++) {
        y = head->right;
        while (y != head) {
            x = y; y = y->right; free(x);
        }
        x = head; head = head->u.next; free(x);
    }
    /* free remaining head nodes */
    y = head;
    while (y != *node) {
        x = y; y = y->u.next; free(x);
    }
    free(*node); *node = NULL;
}

matrix_pointer mtranspose(matrix_pointer matrix){
    int n_row, n_col, n_nonz;
    int row, col, value;

    FILE * tempFile;
    FILE * transFile;
    tempFile = fopen("temp.txt", "w");
    mwrite(matrix, tempFile);
    fclose(tempFile);

    tempFile = fopen("temp.txt", "r");
    fscanf(tempFile, "%d %d %d", &n_col, &n_row, &n_nonz);
    node_ptr temp, head, cur, prev;
    head = NULL;
    for (int i=0; i<n_nonz; i++) {
        fscanf(tempFile, "%d %d %d", &col, &row, &value);
        temp = (node_ptr)malloc(sizeof(Node));
        temp->row = row; temp->col = col; temp->val = value; temp->nxt=NULL;
        prev = NULL;
        cur = head;   
        while (cur!=NULL && (temp->row>cur->row || (temp->row==cur->row && temp->col>cur->col))){
            prev=cur;
            cur=cur->nxt;
        }
        if(prev==NULL){
            temp->nxt = head;
            head = temp;
        }
        else{
            temp->nxt = cur;
            prev->nxt = temp;
        }
    }
    fclose(tempFile);
    remove("temp.txt");
    transFile = fopen("trans.txt", "w");
    fprintf(transFile, "%d %d %d\n", n_row, n_col, n_nonz);
    for (cur = head; cur != NULL; cur = cur->nxt) {
        fprintf(transFile, "%d %d %d\n", cur->row, cur->col, cur->val);
    }
    fclose(transFile);
    while (head != NULL) {
        temp = head;
        head = head->nxt;
        free(temp);
    }
    transFile = fopen("trans.txt", "r");
    matrix = mread(transFile);
    fclose(transFile);
    remove("trans.txt");
    return matrix;
}

int main() {
    matrix_pointer matrix = NULL;

    FILE * inputFile;
    inputFile = fopen("input.txt", "r");
    matrix = mread(inputFile);
    fclose(inputFile);
    matrix = mtranspose(matrix);
    
    FILE * outputFile;
    outputFile = fopen("output.txt", "w");
    mwrite(matrix, outputFile);
    fclose(outputFile);
    merase(&matrix);
    return 0;
}