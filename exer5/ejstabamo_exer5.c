/*  Tabamo, Euan Jed S. - ST2L - March 5, 2024
    Exercise 5 - Pointers
*/

#include <stdio.h>

int main()
{
    int num1, num2 = 3;
    int *nptr1, *nptr2, **nptr3;
    printf("Enter a value: ");
    scanf("%i", &num1);
    //1. display via printf the memory address and content of variable num1
    printf("Memory address of num1 is %p\n", &num1);

    //2-3. nptr1 “points” to num1’s address and display via printf
    nptr1 = &num1;
    printf("Value of num1 is %d\nValue of num1 via nptr1 is %d\n", num1, *nptr1);

    //4. make nptr2 point to what nptr1 is pointing
    nptr2 = nptr1;

    //5. Display via printf the value pointed to by nptr2
    printf("Value at address pointed to by nptr2 is %d\n", *nptr2);

    //6. Display via printf the memory address stored by nptr1 and nptr2
    printf("Memory address in nptr1 is %p\nMemory address in nptr2 is %p\n", nptr1, nptr2);

    //7. Make nptr2 point to the variable num2 and show the new address value of nptr2
    nptr2 = &num2;
    printf("New memory address in nptr2 is %p\n", nptr2);

    //8. Make nptr3 point to nptr2's address
    nptr3 = &nptr2;

    //9. use nptr3 to show via printf num2’s value
    printf("nptr3 points to the address of nptr2 which points to the address of num2 which contains %d\n", **nptr3);

    //10. Display via printf the value of nptr3 and the address of nptr2
    printf("nptr3 points to this address %p\nnptr2 has the memory address %p\n", nptr3, &nptr2);
}