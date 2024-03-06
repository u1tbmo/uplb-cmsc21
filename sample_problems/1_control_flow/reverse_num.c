#include <stdio.h>

int main(int argc, char *argv[])
{
    int num;

    if (argc != 2)
    {
        printf("Usage: ./reverse_num <num>");
        return 1;
    }

    sscanf(argv[1], "%d", &num);

    int rev_num = 0;
    while (num > 0)
    {
        rev_num = (rev_num * 10) + (num % 10);
        num /= 10;
    }

    printf("Reversed Number: %d", rev_num);

    return 0;
}