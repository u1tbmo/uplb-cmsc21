#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

int menu();
void insert(char mode);
void delete(char mode);
void search(int data);
void view(Node *head);

int main(int argc, char *argv[])
{
    Node *head = NULL;
    Node *temp, *p;

    // Create a Node
    temp = (Node *)malloc(sizeof(Node));
    temp->data = 2;
    temp->next = NULL;

    // If the list is empty
    if (head == NULL)
    {
        head = temp;
    }
    else
    {
        p = head;
        while (p->next != NULL) // traverse until the tail of the list
        {
            p = p->next;
        }
        p->next = temp;
    }

    free(temp);
}

void view(Node *head)
{
    Node *temp = head;
    printf("[");
    while (temp != NULL)
    {
        printf("%d", temp->data);
        temp = temp->next;
        if (temp != NULL)
        {
            printf(" ");
        }
    }
    printf("]\n");
}

/* Notes

Linked Lists
- a data structure consisting of dynamic variables
  linked together to form a chain-like structure
            Head Node        Node       Tail Node
  [Head]-->[Data|Next]-->[Data|Next]-->[Data|Next]-->NULL
- Alternative to arrays
- During execution, linked lists can either grow or shrink following the user's needs.

Nodes
- a self-referential structure
- a structure that has a member that is a pointer to itself

Reminders
- The head pointer must always point to the next node in the list.
- The next node's next pointer should always point to the next node in the list.
- The tail pointer's next pointer should always point to NULL.
- Adding a node is done via malloc()
- Deleting a node is done via free()

Types of Linked Lists
1. Singly Linked Lists
2. Doubly Linked Lists
3. Circular Linked Lists
4. Linked Lists with Dummies

Linked List Operations
1. Insertion (Add Nodes)
2. Deletion (Delete Nodes)
3. Searching (Search Nodes)
4. Viewing (Print Nodes)

Insertion
[Head]-->[2|Next]-->[6|Next]-->[9|Next]-->NULL

Insert Head

Create a new node
[Head]-->[2|Next]-->[6|Next]-->[9|Next]-->NULL
[Temp]-->[1|Next]

Both Head and Temp are now pointing to the Head Node
[Head]/[Temp]-->[1|Next]-->[2|Next]-->[6|Next]-->[9|Next]-->NULL

Free Temp
[Head]-->[1|Next]-->[2|Next]-->[6|Next]-->[9|Next]-->NULL

Insert Tail

Insert Middle

*/