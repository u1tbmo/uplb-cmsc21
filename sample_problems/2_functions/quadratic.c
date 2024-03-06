#include <stdio.h>
#include <math.h>

void roots(int a, int b, int c);

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: ./quadratic <a> <b> <c>");
        return 1;
    }

    int num1, num2, num3;

    sscanf(argv[1], "%d", &num1);
    sscanf(argv[2], "%d", &num2);
    sscanf(argv[3], "%d", &num3);

    roots(num1, num2, num3);

    return 0;
}
void roots(int a, int b, int c)
{
    if (a == 0)
    {
        printf("Error: Not a quadratic equation.");
        return;
    }

    float discriminant = (b * b) - (4 * a * c);

    if (discriminant > 0)
    {
        printf("x_1 = %.2f\n", (-b - sqrt(discriminant)) / (2 * a));
        printf("x_2 = %.2f\n", (-b + sqrt(discriminant)) / (2 * a));
    }
    else if (discriminant == 0)
    {
        printf("x = %.2f\n", (double)-b / (2 * a));
    }
    else
    {
        printf("No real solutions.\n");
    }
}