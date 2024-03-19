#include <stdio.h>
#include <stdlib.h>

int *create_int_array(int size);
void print_array(int *array, int length);
int get_int();

int main()
{
    printf("-- Size --\n");
    int size = get_int();
    int *array = create_int_array(size);

    print_array(array, size);
    printf("\n");

    printf("-- Elements --\n");
    for (int i = 0; i < size; i++)
    {
        array[i] = get_int();
    }

    print_array(array, size);
    printf("\n");
    free(array);
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

int get_int()
{
    int temp;
    printf("Enter an integer: ");
    scanf("%d", &temp);
    return temp;
}

void print_array(int *array, int length)
{
    printf("Array: { ");
    for (int i = 0; i < length; i++)
    {
        (i != length - 1) ? printf("%d, ", array[i]) : printf("%d ", array[i]);
    }

    printf("} \n");
}