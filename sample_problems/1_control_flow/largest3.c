#include <stdio.h>

int main(int argc, char *argv[])
{
    float num1, num2, num3;

    if (argc != 4)
    {
        printf("Usage: ./largest3 <num1> <num2> <num3>");
        return 1;
    }

    sscanf(argv[1], "%f", &num1);
    sscanf(argv[2], "%f", &num2);
    sscanf(argv[3], "%f", &num3);

    if (num1 > num2)
    {
        if (num1 > num3)
        {
            printf("%.2f\n", num1);
        }
        else
        {
            printf("%.2f\n", num3);
        }
    }
    else
    {
        if (num2 > num3)
        {
            printf("%.2f\n", num2);
        }
        else
        {
            printf("%.2f\n", num3);
        }
    }

    return 0;
}