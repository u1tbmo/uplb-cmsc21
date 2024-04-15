#include <stdio.h>
#include <stdlib.h>

int *create_int_array(int size);
int **create_matrix(int row, int col);
void init_matrix(int **matrix, int row, int col);
void print_matrix(int **matrix, int row, int col);

int main()
{
    int rows, cols;
    printf("Enter number of rows: ");
    scanf("%d", &rows);
    printf("Enter number of cols: ");
    scanf("%d", &cols);
    int **matrix = create_matrix(rows, cols);
    printf("\n-- Matrix --\n");
    print_matrix(matrix, rows, cols);
    printf("\n");
    init_matrix(matrix, rows, cols);
    printf("\n-- Matrix --\n");
    print_matrix(matrix, rows, cols);

    for (int i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

int *create_int_array(int size)
{
    int *array = (int *)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
    {
        array[i] = 0;
    }

    return array;
}

int **create_matrix(int row, int col)
{
    int **temp = (int **)malloc(sizeof(int *) * row);
    for (int i = 0; i < row; i++)
    {
        temp[i] = (int *)malloc(sizeof(int) * col);
    }
    return temp;
}

void init_matrix(int **matrix, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("[%d][%d] | Enter an integer: ", i, j);
            scanf("%d", &matrix[i][j]);
        }
    }
}

void print_matrix(int **matrix, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}