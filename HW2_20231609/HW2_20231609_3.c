#include <stdio.h>

int check_array(int arr[], int n) {
    if (n == 1) return 1; 

    int min_val = arr[0];
    int max_val = arr[0];
    int sum = 0;
    int expected_sum = 0;

    for (int i = 0; i < n; i++) {
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
        sum += arr[i];
    }

    for (int i = min_val; i <= max_val; i++) {
        expected_sum += i;
    }

    if (max_val - min_val == n - 1 && sum == expected_sum) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    int n;
    scanf("%d", &n);

    int arr[n];
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    printf("%d\n", check_array(arr, n));
    return 0;
}
