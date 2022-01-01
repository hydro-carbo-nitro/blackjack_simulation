#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//함수에서 malloc 연습

int *create_1d_malloc(int size_of_array)
{
    int *array;
    array = (int *)malloc(sizeof(int) * size_of_array);

    for (int i = 0; i < size_of_array; i++)
    {
        array[i] = i;     // 초기값
    }

    return array;
}

int **create_2d_malloc(int row, int col)
{
    int **array;
    array = (int **)malloc(sizeof(int *) * row);
    for (int i = 0; i < row; i++)
    {
        array[i] = (int *)malloc(sizeof(int) * col);
        for (int j = 0; j < col; j++)
        {
            array[i][j] = 10*i + j;     // 초기값
        }
    }

    return array;
}


int main()
{
    int N = 10;
    int *money = create_1d_malloc(N);
    int **player = create_2d_malloc(N, 2);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j <2; j++)
        {
            printf("%d\n", player[i][j]);
        }
    }

    for (int i = 0; i < N; i++)
    {
        free(player[i]);
    }
    free(player);
}