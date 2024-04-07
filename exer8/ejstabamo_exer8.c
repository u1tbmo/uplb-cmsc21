/*  Tabamo, Euan Jed S. - ST2L - March 27, 2024
    Exercise 8 - Strings and Multidimensional Arrays
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global Constant
const int MAX_LEN_NAME = 51;

// Function Prototypes
int menu();                                            // Prints a menu and returns an int of the user's choice
char *get_string();                                    // Gets a string from the user
char **crushes_input(char **array, int qty, int init); // Allows the user to add `qty` names of crushes to the `array`
int remaining_chars(char *name1, char *name2);         // Counts the number of characters that are not common between the two strings
void flames(int char_count);                           // Prints the result of FLAMES using the total character count.
void all_flames(char *name, char **array, int qty);    // Prints all FLAMES results
void free_str_array(char **array, int qty);            // Frees all memory inside an array of strings

int main()
{
    // Variables
    char *name, **crushes;
    int crush_qty, running = 1, initialized = 0, input_buffer;

    // Ask the user for a name
    printf("Enter your name: ");
    name = get_string();

    // Ask the user for the number of crushes and validate
    do
    {
        printf("Enter the number of crushes you have: ");
        scanf("%d", &crush_qty);

        // Clear the input buffer
        while ((input_buffer = getchar()) != '\n' && input_buffer != EOF)
        {
        };

        if (crush_qty <= 0)
        {
            printf("Error: You must have at least one crush to play FLAMES.\n\n");
        }
    } while (crush_qty <= 0);

    // Print a menu and keep looping
    while (running)
    {
        // Check the choice of the user returned by menu()
        switch (menu())
        {
        // Allow the user to input crushes and update the global variable
        case 1:
            crushes = crushes_input(crushes, crush_qty, initialized);
            initialized = 1; // Set the flag variable to true
            break;

        // Allow the user to view the results of FLAMES with all their crushes
        case 2:
            if (!initialized)
            {
                printf("Error: The array is still empty.\n\n");
                continue;
            }
            all_flames(name, crushes, crush_qty);
            break;

        // Allow the user to exit the program
        case 0:
            running = 0;
            printf("Goodbye!\n");
            break;

        // Tell the user that the choice is invalid
        default:
            printf("Error: Invalid choice!\n");
            break;
        }
    }

    // Free allocated memory
    free(name);
    if (initialized)
    {
        // Only free the contents of the array and the array itself if it has been initialized
        // Prevents segmentation fault
        free_str_array(crushes, crush_qty);
        free(crushes);
    }
}

int menu()
{
    // Variables
    int choice, input_buffer;

    // Menu
    printf("+-------------------------------------------------------------------------+\n");
    printf("1 | Input Names\n");
    printf("2 | Compute FLAMES results\n");
    printf("0 | Exit\n");
    printf("+-------------------------------------------------------------------------+\n");
    printf("Enter your choice: ");

    // User Input
    scanf("%d", &choice);

    // Clear the input buffer
    while ((input_buffer = getchar()) != '\n' && input_buffer != EOF)
    {
    };

    printf("\n");

    return choice;
}

char *get_string()
{
    // Variable
    int input_buffer;

    // Allocate enough memory for a 50 length string
    char *temp = (char *)malloc(MAX_LEN_NAME * sizeof(char));
    if (temp == NULL) // If malloc failed
    {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    // Read a max of 50 characters.
    if (fgets(temp, MAX_LEN_NAME, stdin) == NULL) // If fgets failed
    {
        printf("Error: Unable to read input.\n");
        free(temp);
        return NULL;
    }

    // Find the new line if it exists and replace it with the null terminator
    char *newline = strchr(temp, '\n'); // strchr returns a pointer to the newline or NULL if it doesn't exist
    if (newline != NULL)
    {
        *newline = '\0';
    }
    else
    {
        // Clear the input buffer
        while ((input_buffer = getchar()) != '\n' && input_buffer != EOF)
        {
        };
    }

    return temp;
}

char **crushes_input(char **array, int qty, int init)
{
    // If the array was initialized, we need to free it first to not leak memory
    if (init)
    {
        free_str_array(array, qty);
        free(array);
    }

    // Allocate memory for a 2d array
    array = (char **)malloc(qty * sizeof(char *));
    if (array == NULL) // If malloc failed
    {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    // Allow the user to input crushes
    for (int i = 0; i < qty; i++)
    {
        printf("Enter name of crush #%d: ", i + 1);
        array[i] = get_string();
    }

    printf("\n");

    return array;
}

int remaining_chars(char *name1, char *name2)
{
    // Variables
    int count, common_chars = 0, i, j;
    char *first = (char *)malloc(sizeof(char) * (strlen(name1) + 1));  // Allocate memory holding name1
    char *second = (char *)malloc(sizeof(char) * (strlen(name2) + 1)); // Allocate memory holding name2

    // Copy only alphabetical letters to the temporary variables, turning them lowercase
    for (i = 0, j = 0; name1[j] != '\0'; j++)
    {
        if (isalpha(name1[j]))
        {
            first[i++] = tolower(name1[j]);
        }
    }
    first[i] = '\0'; // Terminate the string
    for (i = 0, j = 0; name2[j] != '\0'; j++)
    {
        if (isalpha(name2[j]))
        {
            second[i++] = tolower(name2[j]);
        }
    }
    second[i] = '\0'; // Terminate the string

    // Iterate through both strings
    for (i = 0; first[i] != '\0'; i++)
    {
        for (j = 0; second[j] != '\0'; j++)
        {
            if (first[i] == second[j]) // When we encounter a pair, set them to '-' and add to the common_chars counter
            {
                first[i] = '-';
                second[j] = '-';
                common_chars += 2;
                break;
            }
        }
    }

    // Calculate the number of remaining characters
    count = strlen(first) + strlen(second) - common_chars;

    // Free the temporary variables
    free(first);
    free(second);

    return count;
}

void flames(int char_count)
{
    // In cases where a person has the exact same name as the crush (or no characters remain), print "Unknown".
    if (char_count == 0)
    {
        printf("FLAMES Result: Unknown\n");
        return;
    }

    // Variables
    int letters = 6, i, j;

    // FLAMES is a 2D array that contains the array of chars of FLAMES and an array of '1's
    // This will be the way to find "remove" characters from FLAMES.
    char FLAMES[2][6] = {
        {'F', 'L', 'A', 'M', 'E', 'S'},
        {'1', '1', '1', '1', '1', '1'}};

    // While there is still more than 1 letter in FLAMES
    while (letters > 1)
    {
        // Iterate char_count times through FLAMES, wrapping around if needed
        for (i = 0, j = 1; j < char_count; i = (i + 1) % 6)
        {
            // We only count movement if the letter has not yet been removed
            if (FLAMES[1][i] != '0')
            {
                j++;
            }
        }
        // Make sure that we are currently not pointing at a removed letter.
        while (FLAMES[1][i] == '0')
        {
            i = (i + 1) % 6;
        }

        FLAMES[1][i] = '0'; // Remove the letter as a candidate
        letters--;          // Update the number of candidates left
    }

    // Find the remaining letter and print the result
    for (i = 0; i < 6; i++)
    {
        if (FLAMES[1][i] == '1')
        {
            printf("FLAMES Result: ");
            switch (FLAMES[0][i])
            {
            case 'F':
                printf("Friendship\n");
                break;
            case 'L':
                printf("Love\n");
                break;
            case 'A':
                printf("Affection\n");
                break;
            case 'M':
                printf("Marriage\n");
                break;
            case 'E':
                printf("Enemy\n");
                break;
            case 'S':
                printf("Siblings\n");
                break;
            default:
                break;
            }
            break;
        }
    }
}

void all_flames(char *name, char **array, int qty)
{
    // Iterate through every crush in the array and print the results
    for (int i = 0; i < qty; i++)
    {
        int char_count = remaining_chars(name, array[i]);
        printf("Crush #%d: %s\n", i + 1, array[i]);
        printf("Remaining Character Count: %d\n", char_count);
        flames(char_count);
        printf("\n");
    }
}

void free_str_array(char **array, int qty)
{
    // Free the memory inside the array
    for (int i = 0; i < qty; i++)
    {
        free(array[i]);
    }
}