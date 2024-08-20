#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SET_SIZE 20

void printSubset(char *subset) {
    printf("{");
    int len = strlen(subset);
    for (int i = 0; i < len; i++) {
        if (i > 0) printf(" ");
        printf("%c", subset[i]);
    }
    printf("} ");
}

void PowerSet(char *set, int n, int index, char *current, int currLen, char **subsets, int *pos) {
    subsets[*pos] = malloc((currLen + 1) * sizeof(char));
    strcpy(subsets[*pos], current);
    (*pos)++;

    for (int i = index; i < n; i++) {
        current[currLen] = set[i]; 
        current[currLen + 1] = '\0'; 
        PowerSet(set, n, i + 1, current, currLen + 1, subsets, pos);
    }
}

int compare(const void *a, const void *b) {
    return strlen(*(const char **)a) - strlen(*(const char **)b);
}

int main() {
    int n;
    printf("Enter the size of set S: ");
    scanf("%d", &n);

    char *set = malloc(n * sizeof(char));
    char *current = malloc(n + 1);
    char **subsets = malloc((1 << n) * sizeof(char *));
    int pos = 0;

    for (int i = 0; i < n; i++) {
        set[i] = 'a' + i;
    }

    PowerSet(set, n, 0, current, 0, subsets, &pos);

    
    qsort(subsets, pos, sizeof(char *), compare);

    printf("The power set P(S) is: \n");
    for (int i = 0; i < pos; i++) {
        printSubset(subsets[i]);
        free(subsets[i]);
    }
    printf("\n");

    free(set);
    free(current);
    free(subsets);
    return 0;
}
