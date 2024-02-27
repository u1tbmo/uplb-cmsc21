#include <stdio.h>

// Function Prototypes
float expo(float base, int power);

// Main function taking command line arguments
int main(int argc, char *argv[])
{
    float b;
    int p;

    // Check for the correct number of arguments from the command line
    if (argc != 3)
    {
        printf("Usage: ./exponent <base> <power>\n");
        // Terminate with error 1
        return 1;
    }

    // Convert to respective data types and assign to variables b and p
    sscanf(argv[1], "%f", &b);
    sscanf(argv[2], "%d", &p);

    // Print the result
    printf("%f\n", expo(b, p));

    // Normal termination
    return 0;
}

// Recursive exponentiation Function taking a base and a power
float expo(float base, int power)
{
    // Base Cases
    if (base == 1)
    {
        return 1;
    }
    if (power < 0)
    {
        return 1 / expo(base, -power);
    }
    else if (power == 0)
    {
        return 1;
    }
    // Recursive Case
    else
    {
        return base * expo(base, power - 1);
    }
}