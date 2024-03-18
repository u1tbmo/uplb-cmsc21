#include <stdio.h>

// Define a pair
typedef struct pair
{
    int max;
    int min;
} pair;

// Returns the minimum and maximum element in an array
pair min_max(int length, int array[]);

int main()
{
    /* CONFIGURATION */

    int array[] = {-9, 16, 3, 27, -5, 42, -18, 8, 33, -2};
    const int length = sizeof(array) / sizeof(array[0]);

    /* CONFIGURATION */

    pair result = min_max(length, array);
    printf("Maximum: %d\nMinimum: %d\n", result.max, result.min);
}

pair min_max(int length, int array[])
{
    // Create a pair with the max and min defaulting to the first element of the array.
    pair result = {.max = array[0], .min = array[0]};

    // Iterate through the array and update the min and max value of the result
    for (int i = 1; i < length; i++)
    {
        if (array[i] > result.max)
        {
            result.max = array[i];
        }
        if (array[i] < result.min)
        {
            result.min = array[i];
        }
    }

    return result;
}
