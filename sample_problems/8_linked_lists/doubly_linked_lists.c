#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    struct Node *prev;
    int value;
    struct Node *next;
} Node;

Node *create_node();
void insert(Node **head, int value);
int delete_node(Node **head, int value);
void search(Node **head, int value);
void print_list(Node *head);
void free_list(Node **head);

int main(void)
{
    Node *head = NULL;
    Node *temp, *p;
    int delete_flag;

    insert(&head, 12);
    insert(&head, 6);
    insert(&head, 2);
    insert(&head, 9);
    insert(&head, 7);
    delete_node(&head, 7);

    print_list(head);
    free_list(&head);
    return 0;
}

Node *create_node()
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    *new_node = (Node){.prev = NULL, .value = 0, .next = NULL};
    return new_node;
}

void insert(Node **head, int value)
{
    Node *temp, *ptr;

    temp = create_node();
    temp->value = value;

    // If the list is empty
    if (*head == NULL)
    {
        *head = temp;
    }
    // If the list is NOT empty
    else
    {
        ptr = *head;

        // Insert at the Head of the List
        if (temp->value < (*head)->value)
        {
            temp->next = *head;
            (*head)->prev = temp;
            *head = temp;
        }
        // Insert at the Middle or Tail
        else
        {
            while (ptr->next != NULL && ptr->next->value < value)
            {
                ptr = ptr->next;
            }

            temp->next = ptr->next;
            temp->prev = ptr;

            if (ptr->next != NULL)
            {
                ptr->next->prev = temp;
            }
            ptr->next = temp;
        }
    }
}

int delete_node(Node **head, int value)
{
    Node *temp, *ptr;

    if (*head != NULL)
    {
        ptr = *head;

        // Delete the Head Node
        if ((*head)->value == value)
        {
            *head = (*head)->next;
            (*head)->prev = NULL;
            free(ptr);

            return 1;
        }
        // Delete at the Middle or Tail
        else
        {
            while (ptr->next != NULL && ptr->next->value != value)
            {
                ptr = ptr->next;
            }
            if (ptr->next != NULL)
            {
                temp = ptr->next;
                ptr->next = temp->next;

                // If ptr is second to last in the list
                if (temp->next != NULL)
                {
                    temp->next->prev = ptr;
                }

                free(temp);

                return 1; // Node found and deleted
            }
            else
            {
                return 0; // Node not found
            }
        }
    }
    return 0;
}

void search(Node **head, int value)
{
    return;
}

void append(Node **head, Node *n)
{
    Node *ptr = *head;

    if (*head == NULL)
    {
        *head = n;
        return;
    }
    while (ptr->next != NULL)
    {
        ptr = ptr->next;
    }
    ptr->next = n;
    n->prev = ptr;
}

void print_list(Node *head)
{
    Node *ptr = head;
    while (ptr != NULL)
    {
        printf("%d ", ptr->value);
        ptr = ptr->next;
    }
    printf("\n");
}

void free_list(Node **head)
{
    Node *curr = *head;

    while (*head != NULL)
    {
        curr = curr->next;
        free(*head);
        *head = curr;
    }
}