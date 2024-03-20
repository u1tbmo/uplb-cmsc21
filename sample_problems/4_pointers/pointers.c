#include <stdio.h>

int main()
{
    int x = 2, y = 3, z = 4;
    int *a, *c;
    int **p, **d;

    p = &c;
    c = &x;
    d = p;
    a = &y;

    **p = 7;
    *p = a;
    **d = *c + **p;

    printf("%d\n", **d);
}