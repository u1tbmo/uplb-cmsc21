#include <stdio.h>

int main()
{
    int array[10];
    char str[10];

    for (int i = 0; i < 10; i++)
        printf("%p\n", &array[i]);

    printf("\n");

    for (int i = 0; i < 10; i++)
        printf("%p: %d = %c\n", &str[i], str[i], str[i]);

    // Since a character is only one byte size, C needs the null terminator to know the end of string
    // This is because since C can assume every address in the array as a byte.
    // This is not the same as an int since an int is 4 bytes long and the next address may not be an int.
}