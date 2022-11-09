#include <stdio.h>
#include <stdlib.h>

// 오름차순으로 정렬할 때 사용하는 비교함수
int static compare (const void* first, const void* second)
{
    if (*(int*)first > *(int*)second)
        return 1;
    else if (*(int*)first < *(int*)second)
        return -1;
    else
        return 0;
}

int main()
{
    int arr[] = {32, 11, 97, 42, 21, 70, 56, 67, 88, 100};
    int array_size = sizeof(arr) / sizeof(int);
    int i;

    // 정렬 전
    for (i = 0; i < array_size; i++) printf("%d ", arr[i]);
    printf("\n");

    qsort(arr, array_size, sizeof(int), compare);

    // 정렬 후
    for (i = 0; i < array_size; i++) printf("%d ", arr[i]);
    printf("\n");

    return 0;
}