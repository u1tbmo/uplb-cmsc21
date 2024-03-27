#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *create_str(int chars);
void read_str(char *str, int chars);
int count_vowels(char *s);
int count_a(char *s);

int main()
{
    printf("%c\n", 'A' + 32);

    char *name;
    int size;

    printf("Enter the number of characters in your name: ");
    scanf("%d", &size);

    name = create_str(size);
    read_str(name, size);

    printf("%s: %d vowels\n", name, count_vowels(name));
    printf("%s: %d As\n", name, count_a(name));

    free(name);

    return 0;
}

char *create_str(int chars)
{
    char *new_string = (char *)malloc((chars + 1) * sizeof(char));
    return new_string;
}

void read_str(char *str, int chars)
{
    char *p = str;

    getchar();

    printf("Enter your name: ");
    fgets(str, chars + 1, stdin);

    // Post-Increment (p++)
    while (*p++ != '\0')
    {
        if (*p == '\n')
        {
            *p = '\0';
        }
    }
}

int count_vowels(char *s)
{
    int count = 0;

    while ((*s) != '\0')
    {
        if (*s == 'a' || *s == 'e' || *s == 'i' || *s == 'o' || *s == 'u')
            count++;
        s++;
    }

    return count;
}

int count_a(char *s)
{
    int count = 0;

    while ((*s) != '\0')
    {
        if (*s == 'a' || *s == 'A')
            count++;
        s++;
    }

    return count;
}