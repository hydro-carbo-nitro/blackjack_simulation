#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//realloc 연습

void output_value(int **arr, int col, int row)
/* 이중 포인터로 생성된 배열을 확인하는 함수 */
{
    for (int i = 0; i < row; i++) 
    {
        printf("%dth row : [ ", i);
        for (int j = 0; j < col; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("]\n");
    }
}

void one_dim_realloc()
{
    int len = 4;
    int len2 = 10;
    int *arr;
    
    arr = (int *)malloc(sizeof(int) * len);
    for (int i = 0; i < len; i++)
    {
        arr[i] = i;
        printf("%d", arr[i]);
    }

    arr = (int *)realloc(arr, sizeof(int) * len2);
    for (int i = 0; i < len2; i++)
    {
        //arr[i] = i;
        printf("%d", arr[i]);
    }

    free(arr);
}

void two_dim_realloc()
{
    int col = 5;
    int row = 10;
    int **arr;
    int *size_arr;

    arr = (int **)malloc(sizeof(int *) * row);
    for (int i = 0; i < row; i++)
    {
        arr[i] = (int *)malloc(sizeof(int) * col);
        for (int j = 0; j < col; j++)
        {
            arr[i][j] = 10*i + j;
        }
    }

    size_arr = (int *)malloc(sizeof(int) * row);
    for (int i = 0; i < row; i++)
    {
        size_arr[i] = col;
    }

    for (int i = 0; i < row; i++) 
    {
        printf("%dth row(%d) : [ ", i, size_arr[i]);
        for (int j = 0; j < size_arr[i]; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("]\n");
        
    }
    printf("\n");


    arr[0] = (int *)realloc(arr[0], sizeof(int) * (col + 2));
    size_arr[0] = col + 2;
    for (int j = 0; j < size_arr[0]; j++)
    {
        //arr[0][j] = j;
    }

    for (int i = 0; i < row; i++) 
    {
        printf("%dth row(%d) : [ ", i, size_arr[i]);
        for (int j = 0; j < size_arr[i]; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("]\n");
        
    }

    for (int i = 0; i < row; i++)
    {
        free(arr[i]);
    }
    free(arr);

}
int main()
{
    //one_dim_realloc();
    two_dim_realloc();
}
    