#include <stdio.h>

/* Uses the Euclidean algorithm to find the GCD of two numbers. */
int euclid(int num1, int num2);

int main(int argc, char *argv[])
{
    int num1, num2, gcd;

    if (argc != 3)
    {
        printf("Usage: ./euclid <num1> <num2>\n");
        return 1;
    }

    sscanf(argv[1], "%d", &num1);
    sscanf(argv[2], "%d", &num2);

    if (num1 == 0 || num2 == 0)
    {
        printf("Error: 0 is not allowed.\n");
        return 2;
    }

    gcd = euclid(num1, num2);

    printf("The GCD of %d and %d is %d.\n", num1, num2, gcd);
    return 0;
}

int euclid(int num1, int num2)
{
    // Variable
    int remainder;

    // If num1 < num2, swap them.
    if (num1 < num2)
    {
        num2 = (num1 + num2) - (num1 = num2);
    }

    // Get the remainder of num1 / num2
    remainder = num1 % num2;

    // Base Case: If remainder == 0, return num2 as the gcd of num1 and num2
    if (remainder == 0)
    {
        return num2;
    }
    // Recursive Case: Else, replace num1 by num2 and num2 by the remainder, repeat.
    else
    {
        return euclid(num2, remainder);
    }
}