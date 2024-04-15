#include <stdio.h>
#include <stdlib.h>

void display(int A[10])
{
    int i;
    for (i = 0; i <= 10; i += 1)
    {
        printf("%d\n", *(A + i));
    }
}

int main()
{
    // int list[10], i;
    // for (i = 0; i < 10; i += 1)
    // {
    //     list[i] = i;
    // }

    // display(list);

    float *grades;
    int i, size = 3;

    grades = (float *)malloc(sizeof(float) * size);

    for (i = 0; i < size; i++)
    {
        printf("Enter grade in the subject %d: ", i + 1);
        scanf("%f", &grades[i]);
    }
    for (i = 0; i < size; i++)
    {
        printf("Your grade in subject %d is: %f\n", i + 1, grades[i]);
    }
}