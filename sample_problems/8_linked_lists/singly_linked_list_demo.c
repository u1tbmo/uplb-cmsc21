#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;
typedef Node *nptr;

// Returns a pointer to a new node
nptr create_node(int data)
{
    nptr temp;

    // creates a dynamic variable
    temp = (Node *)malloc(sizeof(Node));

    // initialize the components of the Node temp
    *temp = (Node){.data = data, .next = NULL};

    return temp; // return the pointer to that structure
}

void create_list(nptr *head, int size)
{
    nptr temp;

    for (int i = 0; i < size; i++)
    {
        temp = create_node(i);
        temp->next = *head;
        *head = temp;
    }
}

void display_list(nptr head)
{
    nptr temp;
    temp = head;
    while (temp != NULL)
    {
        printf("%i\t", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main()
{
    nptr head = NULL;
    int total_data;

    printf("Enter number of items for your list: ");
    scanf("%d", &total_data);

    create_list(&head, total_data);
    display_list(head);
    return 0;
}