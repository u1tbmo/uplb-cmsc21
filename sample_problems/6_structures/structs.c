#include <stdio.h>
#include <stdlib.h>

typedef struct Person
{
    int brain;
    struct Person *partner;
} Person;

int main()
{
    Person *euan = (Person *)malloc(sizeof(Person));
    euan->brain = 50;
    euan->partner = NULL;

    Person *unknown = (Person *)malloc(sizeof(Person));
    unknown->brain = 30;
    unknown->partner = NULL;

    euan->partner = unknown;
    unknown->partner = euan;

    printf("Euan's Partner's Brain:   %d\n", euan->partner->brain);
    printf("Unknown's Parner's Brain: %d\n", unknown->partner->brain);

    free(euan);
    free(unknown);
}