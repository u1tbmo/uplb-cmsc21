#include <stdio.h>

int main(int argc, char *argv[])
{
    int num, sum = 0;

    if (argc != 2)
    {
        printf("Usage: ./summation <number>");
        return 1;
    }

    sscanf(argv[1], "%d", &num);

    for (int i = 0; i <= num; i++)
    {
        sum += i;
    }

    printf("Sum: %d\n", sum);

    return 0;
}