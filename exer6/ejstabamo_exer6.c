/*  Tabamo, Euan Jed S. - ST2L - March 12, 2024
    Exercise 6 - Parameter Passing
*/

#include <stdio.h>

// Function Prototypes
int LCM_version1(int num1, int num2);             // Finds the LCM using the recursive Euclid's Algorithm
void LCM_version2(int *dest, int num1, int num2); // Finds the LCM using iterative version of Euclid's Algorithm
int Euclid_recursive(int num1, int num2);         // Euclid's Algorithm: Recursive function that finds the GCD of two positive ints

int main()
{
    // Initialize Variables
    int LCM_result_v1, LCM_result_v2; // Holds the results of the LCM functions
    int num1, num2;                   // Holds the input of the user

    // Prompt two numbers
    printf("Enter two numbers: ");
    scanf("%i %i", &num1, &num2);

    // Validate input
    if (num1 <= 0 || num2 <= 0)
    {
        printf("Error 1: Integer/s should be positive.\n");
        return 1;
    }

    /* Test Code
    printf("GCD of %i and %i is %i\n", num1, num2, Euclid_recursive(num1, num2));
    */

    // Get the LCM using LCM_version1
    LCM_result_v1 = LCM_version1(num1, num2); // Assign the return value of the function
    printf("LCM of %i and %i is %i\n", num1, num2, LCM_result_v1);

    // Get the LCM using LCM_version2
    LCM_version2(&LCM_result_v2, num1, num2); // LCM_version2 does not return anything, but accepts a destination (like scanf())
    printf("LCM of %i and %i is %i\n", num1, num2, LCM_result_v2);
}

int LCM_version1(int num1, int num2)
{
    // The LCM is given by (num1*num2) / gcd(num1, num2)
    return (num1 * num2) / Euclid_recursive(num1, num2);
}

void LCM_version2(int *dest, int num1, int num2)
{
    // Variables
    int remainder, og_num1, og_num2;

    // Remember the original values of num1 and num2
    og_num1 = num1;
    og_num2 = num2;

    // Iteratively find the GCD using Euclid's algorithm
    while (1)
    {
        // If num1 < num2, swap them.
        if (num1 < num2)
        {
            num2 = (num1 + num2) - (num1 = num2);
        }

        // Get the remainder of num1 / num2
        remainder = num1 % num2;

        // If remainder == 0, num2 is the gcd of num1 and num2
        if (remainder == 0)
        {
            break;
        }
        else
        {
            // Otherwise, replace num1 by num2 and num2 by the remainder, repeat.
            num1 = num2;
            num2 = remainder;
        }
    };

    /* Since num2 is the GCD, LCM is given by the formula (og_num1*og_num2) / num2
        This will be assigned into where the pointer dest is pointing */
    *dest = (og_num1 * og_num2) / num2;

    // This function does not return anything.
    return;
}

int Euclid_recursive(int num1, int num2)
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
        return Euclid_recursive(num2, remainder);
    }
}
