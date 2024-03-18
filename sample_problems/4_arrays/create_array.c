#include <stdio.h>
#include <stdlib.h>

// Function Prototype
int to_int(char *string);
void print_array(int *array, int length);

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Usage: ./create_array <num1> {num2} ...\n");
        return 1;
    }

    int *array = malloc(sizeof(int) * (argc - 1));
    for (int i = 1; i < argc; i++)
    {
        array[i - 1] = to_int(argv[i]);
    }

    print_array(array, argc - 1);
}

int to_int(char *string)
{
    int temp;
    sscanf(string, "%d", &temp);
    return temp;
}

void print_array(int *array, int length)
{
    printf("Output: { ");
    for (int i = 0; i < length; i++)
    {
        (i != length - 1) ? printf("%d, ", array[i]) : printf("%d ", array[i]);
    }
    printf("} \n");
}