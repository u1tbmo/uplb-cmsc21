#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

int get_int();
void clear_buffer();
Node *free_list(Node *head);

void print_list(Node *head);
int count_nodes(Node *head);
Node *append(Node *head, int data);
Node *prepend(Node *head, int data);
Node *insert(Node *head, int data, int position);
Node *delete(Node *head, int position);
Node *destroy(Node *head);
Node *reverse(Node *head);

int main()
{
    Node *head = NULL;
    char choice;
    int data, position, count;

    do
    {
        printf("--------- Linked List --------\n");
        count = count_nodes(head);
        print_list(head);
        printf("------------ Menu ------------\n");
        printf("1 | Append\n");
        printf("2 | Prepend\n");
        printf("3 | Insert\n");
        printf("4 | Delete\n");
        printf("5 | Destroy\n");
        printf("6 | Reverse\n");
        printf("X | Exit\n");
        printf("------------------------------\n");
        printf("Enter choice: ");
        choice = fgetc(stdin);
        clear_buffer();

        printf("\n");
        switch (choice)
        {
        case '1':

            printf("Enter a number: ");
            data = get_int();
            head = append(head, data);
            break;
        case '2':
            printf("Enter a number: ");
            data = get_int();
            head = prepend(head, data);
            break;
        case '3':
            if (count == 0)
            {
                printf("Error: Linked list is empty.\n\n");
                break;
            }
            printf("Enter a position: ");
            position = get_int();
            if (position > count || count < 1)
            {
                printf("Error: Position out of bounds.\n\n");
                break;
            }
            printf("Enter a number: ");
            data = get_int();
            head = insert(head, data, position);
            break;
        case '4':
            if (count == 0)
            {
                printf("Error: Linked list is empty.\n\n");
                break;
            }
            printf("Enter a position: ");
            position = get_int();
            if (position > count || position < 1)
            {
                printf("Error: Position out of bounds.\n\n");
                break;
            }
            head = delete (head, position);
            break;
        case '5':
            head = destroy(head);
            break;
        case '6':
            head = reverse(head);
            break;
        case 'x':
        case 'X':
            printf("Goodbye!\n\n");
            break;
        default:
            printf("Error: Unknown choice!\n\n");
            break;
        }
    } while (choice != 'X' && choice != 'x');

    head = free_list(head);
    return 0;
}

int get_int()
{
    int number;
    scanf("%d", &number);
    clear_buffer();
    return number;
}

void clear_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

Node *free_list(Node *head)
{
    Node *temp = head;
    while (temp != NULL)
    {
        temp = temp->next;
        free(head);
        head = temp;
    }
    return head;
}

void print_list(Node *head)
{
    Node *ptr = head;

    if (head == NULL)
    {
        printf("Linked list is empty.\n");
        return;
    }

    while (ptr != NULL)
    {
        printf("%d ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

int count_nodes(Node *head)
{
    int count = 0;
    Node *ptr = NULL;
    ptr = head;
    while (ptr != NULL)
    {
        count++;
        ptr = ptr->next;
    }
    printf("Number of Nodes: %d\n", count);
    return count;
}

Node *append(Node *head, int data)
{
    Node *ptr, *temp;

    temp = (Node *)malloc(sizeof(Node));
    *temp = (Node){.data = data, .next = NULL};

    if (head == NULL)
    {
        head = temp;
        return head;
    }

    ptr = head;

    while (ptr->next != NULL)
    {
        ptr = ptr->next;
    }
    ptr->next = temp;
    return head;
}

Node *prepend(Node *head, int data)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    *temp = (Node){.data = data, .next = NULL};
    temp->next = head;
    head = temp;

    return head;
}

Node *insert(Node *head, int data, int position)
{
    if (position == 1)
    {
        return prepend(head, data);
    }

    Node *ptr = head;
    Node *temp = (Node *)malloc(sizeof(Node));
    *temp = (Node){.data = data, .next = NULL};

    position--;
    while (position != 1)
    {
        ptr = ptr->next;
        position--;
    }

    temp->next = ptr->next;
    ptr->next = temp;

    return head;
}

Node *delete(Node *head, int position)
{
    Node *previous = head;
    Node *current = head;

    // Delete the beginning
    if (position == 1)
    {
        head = current->next;
        free(current);
        current = NULL;
        return head;
    }
    // Delete in the middle
    while (position != 2)
    {
        previous = current;
        current = current->next;
        position--;
    }
    previous->next = current->next;
    free(current);
    current = NULL;

    return head;
}

Node *destroy(Node *head)
{
    Node *ptr = head;
    while (ptr != NULL)
    {
        ptr = ptr->next;
        free(head);
        head = ptr;
    }

    return head;
}

Node *reverse(Node *head)
{
    Node *prev = NULL;
    Node *next = NULL;

    while (head != NULL)
    {
        next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    head = prev;
    return head;
}