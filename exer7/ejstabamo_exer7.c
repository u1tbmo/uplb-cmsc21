/*  Tabamo, Euan Jed S. - ST2L - March 19, 2024
    Exercise 7 - Arrays
*/

#include <stdio.h>
#include <stdlib.h>

int print_menu(int *array, int array_size, int initialized);       // 1. Print Menu
void initialize_array(int *array, int array_size);                 // 2. Input Number
void array_element_factors_prompt(int *array, int array_size);     // 3. Factors of an element in an array (prompts the user for an index)
void array_element_factors(int *array, int array_size, int index); // 3. Factors of an element in an array (index is used as a formal parameter)
void array_factors(int *array, int array_size);                    // 4. Factors of all elements in the array
int get_integer();                                                 // Helper function
void print_array(int *array, int array_size);                      // Increases quality of life

int main()
{
    // Variables
    int array_size, *array, is_running = 1, initialized = 0, choice;

    // Ask the user for an array size
    do
    {
        printf("Enter the array size: ");
        array_size = get_integer();
        if (array_size <= 0)
        {
            printf("Error: Invalid array size!\n\n");
        }
    } while (array_size <= 0);

    // Create a dynamic array of length array_size
    array = (int *)malloc(sizeof(int) * array_size);

    // Menu Loop
    while (is_running)
    {
        choice = print_menu(array, array_size, initialized);
        printf("\n");

        switch (choice)
        {
        case 0:
            // Allow the user to add values to the array
            initialize_array(array, array_size);
            printf("\n");

            // Set the initialized flag to true
            if (!initialized)
            {
                initialized = 1;
            }

            break;

        case 1:
            // Check if the array is initialized first
            if (!initialized)
            {
                printf("Error: The array is still empty!\n\n");
                continue;
            }
            // Call array_element_factors_prompt()
            array_element_factors_prompt(array, array_size);
            printf("\n");

            break;

        case 2:
            // Check if the array is initialized first
            if (!initialized)
            {
                printf("Error: The array is still empty!\n\n");
                continue;
            }

            // Call array_factors()
            array_factors(array, array_size);
            printf("\n");

            break;

        case 3:
            printf("Goodbye!\n");
            free(array);    // Free the array before exiting the program
            is_running = 0; // Set the is_running flag to false to stop the while loop
            break;

        default:
            printf("Error: Invalid choice!\n\n");
            break;
        }
    }
}

// Prints a menu, asks the user for a choice, then returns the choice.
int print_menu(int *array, int array_size, int initialized)
{
    // Variable
    int choice;

    printf("-------------------------------------------------------------\n");
    (initialized) ? print_array(array, array_size) : printf("Array: Uninitialized!\n"); // If the array is initialized, display its elements.
    printf("-------------------------------------------------------------\n");
    printf("0 | Input Numbers\n");
    printf("1 | Factors of an Element in the Array\n");
    printf("2 | Factors of all Elements in the Array\n");
    printf("3 | Exit\n");
    printf("-------------------------------------------------------------\n");

    printf("Enter choice: ");
    choice = get_integer();

    return choice;
}

// Asks the user for an integer and returns that integer.
int get_integer()
{
    int temp;
    scanf("%d", &temp);
    return temp;
}

// Displays the elements of an array.
void print_array(int *array, int array_size)
{
    printf("Array: { ");
    for (int i = 0; i < array_size; i++)
    {
        (i < array_size - 1) ? printf("%d, ", array[i]) : printf("%d ", array[i]); // Only print commas if it isn't the end of the array.
    }
    printf("}\n");
}

// Allows the user to input values for the array.
void initialize_array(int *array, int array_size)
{
    // Ask for array_size number of positive integers.
    for (int i = 0; i < array_size; i++)
    {
        do
        {
            printf("Enter array[%d]: ", i);
            array[i] = get_integer();

            if (array[i] <= 0)
            {
                printf("Error: Value must be positive!\n\n");
            }
        } while (array[i] <= 0);
    }
}

// Prompts the user for an index, then displays the factors of that element which are present in that array.
void array_element_factors_prompt(int *array, int array_size)
{
    int index;

    // Ask the user for a valid index
    do
    {
        // If the array size is 1, there is only one valid index (0). Otherwise, the range is [0, array_size - 1].
        (array_size == 1) ? printf("Valid Range: [0]\n") : printf("Valid Range: [0, %d]\n", array_size - 1);
        printf("Enter the index of an element: ");
        index = get_integer();

        if (index < 0 || index >= array_size)
        {
            printf("Error: Index out of range!\n\n");
        }
    } while (index < 0 || index >= array_size);

    array_element_factors(array, array_size, index);
}

// Displays the factors of the array element at a given index which are present in that array.
void array_element_factors(int *array, int array_size, int index)
{
    int factors_qty = 0;

    printf("The factors of %d in the array are: ", array[index]);

    // Check every element in the array (except the element at the index itself) if it is a factor
    // If it is, print it.
    for (int i = 0; i < array_size; i++)
    {
        if (index == i)
        {
            continue;
        }
        if (array[index] % array[i] == 0)
        {
            printf("%d ", array[i]);
            factors_qty++;
        }
    }

    // Print None if there are no factors in the array for that element.
    (factors_qty == 0) ? printf("None\n") : printf("\n");
}

// Displays all factors of every element (excluding itself) which are in the same array.
void array_factors(int *array, int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        array_element_factors(array, array_size, i);
    }
}
