#include <stdio.h>
#include <string.h>

// Global Variables
const int MAX_LINE_LEN = 101;

int main(int argc, char *argv[])
{
    // Check arguments
    if (argc != 2)
    {
        printf("Usage: ./meow <filename>\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r"); // Read from file
    if (fp == NULL)
    {
        printf("Error: File does not exist.\n");
        return 2;
    }

    char temp[MAX_LINE_LEN];
    int line_number = 1;
    while (fgets(temp, MAX_LINE_LEN, fp) != NULL)
    {
        printf("%d\t%s", line_number++, temp);
    }
    printf("\n--- END OF FILE ---\n");

    // char ch;
    // while ((ch = fgetc(fp)) != EOF)
    // {
    //     putchar(ch);
    // }
    // printf("\n--- END OF FILE ---\n");

    fclose(fp); // Close the file
    return 0;
}