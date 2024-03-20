#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
} node;

void free_list(node *list);
void print_list(node *list);

int main()
{
    // Variables

    int qty;

    // Initialize a list node for the linked list
    node *list = NULL;

    printf("Enter quantity: ");
    scanf("%d", &qty);

    // Prepend
    for (int i = 0; i < qty; i++)
    {
        node *temp = malloc(sizeof(node));
        if (temp == NULL)
        {
            free_list(list);
            return 2;
        }

        printf("Enter data: ");
        scanf("%d", &(temp->data));

        temp->next = list;
        list = temp;
    }

    print_list(list);
    free_list(list);
}

void free_list(node *list)
{
    node *temp;
    while (list != NULL)
    {
        temp = list;
        list = list->next;
        free(temp);
    }
}

void print_list(node *list)
{
    node *temp = list;
    while (temp != NULL)
    {
        printf("%d\n", temp->data);
        temp = temp->next;
    }
}