#include <stdio.h>
#include <stdlib.h>

int strcmp_custom(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}

size_t strlen_custom(const char *str) {
    const char *s;
    for (s = str; *s; ++s) {}
    return s - str;
}

void sort_names(char **names, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (strcmp_custom(names[j], names[j + 1]) > 0) {
                char *temp = names[j];
                names[j] = names[j + 1];
                names[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n;
    scanf("%d\n", &n);  // 입력을 stdin에서 읽음

    char **names = (char **)malloc(n * sizeof(char *));
    if (names == NULL) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++) {
        names[i] = (char *)malloc(100 * sizeof(char));
        if (names[i] == NULL) {
            perror("Memory allocation failed");
            for (int j = 0; j < i; j++) {
                free(names[j]);
            }
            free(names);
            return EXIT_FAILURE;
        }
        fgets(names[i], 100, stdin);
        names[i][strlen_custom(names[i]) - 1] = '\0';  // 개행 문자 제거
    }

    sort_names(names, n);

    for (int i = 0; i < n; i++) {
        printf("%s\n", names[i]);
        free(names[i]);
    }
    free(names);

    return 0;
}
