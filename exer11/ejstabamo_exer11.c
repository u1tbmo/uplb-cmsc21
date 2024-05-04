/*  Tabamo, Euan Jed S. - ST2L - April 30, 2024
    Exercise 11 - Singly Linked Lists
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_LENGTH 5
#define STR_LENGTH 50 // increased to 50 to allow for longer strings
#define EVENT_FILE "events.txt"
#define CUSTOMER_FILE "customers.txt"

// Structures
// Used to create an instance of an event
typedef struct event_t
{
    char event_id[ID_LENGTH];
    char event_title[STR_LENGTH];
    char artist[STR_LENGTH];
    char datetime[STR_LENGTH];
    float ticket_price;
    int stock;
    int sold_count;
    struct event_t *next_event;
} Event;

// Used to create a booking linked list for each customer.
typedef struct bookings_t
{
    struct event_t *event_booked;
    struct bookings_t *next_booking;
} Booking;

// Used to create an instance of a customer.
typedef struct customer_t
{
    char name[STR_LENGTH];
    float total_cost;
    struct bookings_t *tickets_bought;
    struct customer_t *next_customer;
} Customer;

// Utility

char menu();                                       // Print the menu and return the char of the user's choice
void clear_buffer();                               // Clears the input buffer
char *get_string(int length);                      // Gets a string from the user
int event_position(Event *head, char *event_id);   // Returns the position of an event in an LL, -1 if it doesn't exist
int customer_position(Customer *head, char *name); // Returns the position of a customer in an LL, -1 if it doesn't exist

// Linked List Functions
Event *create_event();                                // Creates an Event Node with default values
void events_insert(Event **head, Event *e);           // Insert an Event node to a LL (alphabetical)
void events_delete(Event **head, char *event_id);     // Delete an Event node from a LL
void events_destroy(Event **head);                    // Free/destroy an Event LL
int events_count(Event *head);                        // Count the number of Events in an LL
Customer *create_customer();                          // Creates a Customer Node with default values
void customers_append(Customer **head, Customer *c);  // Append a Customer node to a LL
void customers_delete(Customer **head, char *name);   // Delete a Customer node from a LL
void customers_destroy(Customer **head);              // Free/destroy a Customer LL
int customers_count(Customer *head);                  // Count the number of Customers in an LL
Booking *create_booking();                            // Creates a Booking Node with default values
void bookings_append(Booking **head, Booking *b);     // Append a Booking node to a LL
void bookings_delete(Booking **head, char *event_id); // Delete a Booking node from a LL
void bookings_destroy(Booking **head);                // Free/destroy a Booking LL
int bookings_count(Booking *head);                    // Count the number of Bookings in an LL

// Program Logic
void add_event(Event **head);                      // Adds an Event to the database
void edit_event(Event *e_head, Customer *c_head);  // Edits an Event from the database
void delete_event(Event **e_head);                 // Deletes an Event from the database
void view_events(Event *head);                     // Views all Events in the database
void view_events_l(Event *head);                   // Views all Events in the database in linear format
void view_customers(Customer *head);               // Views all Customers in the database
void buy_ticket(Event *e_head, Customer **c_head); // Buys a ticket, adds a Customer and a Booking
void view_bookings(Booking *head);                 // Views all Bookings for a Customer

// File Functions
void save_events(Event *head);                         // Save all Events to a file
void save_customers(Customer *head);                   // Save all Customers to a file
void load_events(Event **head);                        // Load all Events from a file
void load_customers(Event *e_head, Customer **c_head); // Load all Customers from a file

int main()
{
    // Linked Lists
    Event *e_head = NULL;
    Customer *c_head = NULL;

    // Load Events and Customers
    load_events(&e_head);
    load_customers(e_head, &c_head);

    // Main Program Loop
    char choice;
    do
    {
        choice = menu();
        printf("\n");
        switch (choice)
        {
        case '1': // Add Event
            add_event(&e_head);
            break;
        case '2': // Buy a Ticker
            if (e_head == NULL)
            {
                printf("Error: No events.\n\n");
                break;
            }
            buy_ticket(e_head, &c_head);
            break;
        case '3': // Edit Event
            if (e_head == NULL)
            {
                printf("Error: No events.\n\n");
                break;
            }
            edit_event(e_head, c_head);
            break;
        case '4': // Delete Event
            if (e_head == NULL)
            {
                printf("Error: No events.\n\n");
                break;
            }
            delete_event(&e_head);
            break;
        case '5': // View all Events
            if (e_head == NULL)
            {
                printf("Error: No events.\n\n");
                break;
            }
            view_events(e_head);
            break;
        case '6': // View all Customers
            if (c_head == NULL)
            {
                printf("Error: No customers.\n\n");
                break;
            }
            view_customers(c_head);
            break;
        case '0': // Quit Program
            save_events(e_head);
            save_customers(c_head);
            printf("Goodbye!\n\n");
            break;
        default: // Invalid Choice
            printf("Error: Unknown choice\n\n");
            break;
        }
    } while (choice != '0');

    // Destroy Linked Lists (Free allocated memory)
    events_destroy(&e_head);
    customers_destroy(&c_head);
    return 0;
}

char menu()
{
    char choice;
    printf("---------------- MENU ----------------\n");
    printf("1 | Add Event Details\n");
    printf("2 | Buy Ticket\n");
    printf("3 | Edit Event Details\n");
    printf("4 | Delete Event\n");
    printf("5 | View All Events\n");
    printf("6 | View All Customers\n");
    printf("0 | Exit\n");
    printf("--------------------------------------\n");
    printf("Enter choice: ");
    choice = fgetc(stdin);
    clear_buffer();
    return choice;
}
void clear_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // clear the input buffer
}
char *get_string(int length)
{
    // Allocate enough memory for a string with length `length`
    char *temp = (char *)malloc(length * sizeof(char));
    if (temp == NULL) // If malloc failed
    {
        printf("Error: Memory allocation failed.\n\n");
        return NULL;
    }

    // Read a max of `length - 1` characters.
    if (fgets(temp, length, stdin) == NULL) // If fgets failed
    {
        printf("Error: Unable to read input.\n\n");
        free(temp);
        return NULL;
    }

    // Find the new line if it exists and replace it with the null terminator
    char *newline = strchr(temp, '\n'); // strchr returns a pointer to the newline or NULL if it doesn't exist
    if (newline != NULL)
    {
        *newline = '\0';
    }
    else // If there is no newline, ensure that extra characters are cleared from the buffer
    {
        clear_buffer();
    }

    return temp;
}
int event_position(Event *head, char *event_id)
{
    Event *temp = head;
    int pos = 0;
    while (temp != NULL)
    {
        pos++;
        if (strcmp(temp->event_id, event_id) == 0)
        {
            return pos; // return the position of the Event in the LL
        }
        temp = temp->next_event; // Traverse
    }
    return -1;
}
int customer_position(Customer *head, char *name)
{
    Customer *temp = head;
    int pos = 0;
    while (temp != NULL)
    {
        pos++;
        if (strcmp(temp->name, name) == 0)
        {
            return pos; // return the position of the Customer in the LL
        }
        temp = temp->next_customer; // Traverse
    }
    return -1;
}

Event *create_event()
{
    // Allocate memory for an Event
    Event *e = (Event *)malloc(sizeof(Event));

    // Initialize to defaults
    strcpy(e->event_id, "");
    strcpy(e->event_title, "");
    strcpy(e->artist, "");
    strcpy(e->datetime, "");
    e->ticket_price = 0.0f;
    e->stock = 0;
    e->sold_count = 0;
    e->next_event = NULL;

    return e;
}
void events_insert(Event **head, Event *e)
{
    Event *curr = *head;

    // If LL is empty or name is alphabetically before the head event's name
    if (curr == NULL || strcmp(e->event_title, curr->event_title) < 0)
    {
        e->next_event = curr;
        *head = e;
    }
    else
    {
        // Traverse to the correct spot (to sort alphabetically)
        while (curr->next_event != NULL && strcmp(e->event_title, curr->next_event->event_title) >= 0)
        {
            curr = curr->next_event;
        }
        e->next_event = curr->next_event;
        curr->next_event = e;
    }
}
void events_delete(Event **head, char *event_id)
{
    Event *prev = *head;
    Event *curr = *head;

    if (event_position(*head, event_id) == -1) // Ensure Event exists
    {
        printf("Error: Event does not exist.\n\n");
        return;
    }

    if (strcmp(curr->event_id, event_id) == 0) // Delete from the beginning
    {
        *head = curr->next_event;
        free(curr);
        curr = NULL;
        return;
    }
    while (strcmp(curr->event_id, event_id) != 0) // Delete from the middle or end
    {
        prev = curr;
        curr = curr->next_event;
    }
    prev->next_event = curr->next_event;
    free(curr);
    curr = NULL;
}
void events_destroy(Event **head)
{
    Event *ptr = *head;
    while (ptr != NULL)
    {
        ptr = ptr->next_event; // Traverse
        free(*head);           // Free the head node
        *head = ptr;           // Set the head node
    }
}
int events_count(Event *head)
{
    int count = 0;
    Event *ptr = head;
    while (ptr != NULL)
    {
        count++;               // Count for every Node in the LL
        ptr = ptr->next_event; // Traverse
    }
    return count;
}

Customer *create_customer()
{
    // Allocate memory for a Customer
    Customer *c = (Customer *)malloc(sizeof(Customer));

    // Initialize to defaults
    strcpy(c->name, "");
    c->total_cost = 0.0f;
    c->tickets_bought = NULL;
    c->next_customer = NULL;

    return c;
}
void customers_append(Customer **head, Customer *c)
{
    Customer *ptr = *head;

    if (ptr == NULL) // If LL is empty
    {
        *head = c; // The c node is now the head node of the LL
        return;
    }
    while (ptr->next_customer != NULL) // Traverse to the end of the LL
    {
        ptr = ptr->next_customer;
    }
    ptr->next_customer = c;
}
void customers_delete(Customer **head, char *name) // * Currently unused
{
    Customer *prev = *head;
    Customer *curr = *head;

    if (customer_position(*head, name) == -1) // Ensure customer exists
    {
        printf("Error: Customer does not exist.\n\n");
        return;
    }

    if (strcmp(curr->name, name) == 0) // Delete from the beginning
    {
        *head = curr->next_customer;
        free(curr);
        curr = NULL;
        return;
    }
    while (strcmp(curr->name, name) != 0) // Delete from the middle or end
    {
        prev = curr;
        curr = curr->next_customer;
    }
    prev->next_customer = curr->next_customer;
    free(curr);
    curr = NULL;
}
void customers_destroy(Customer **head)
{
    Customer *ptr = *head;
    while (ptr != NULL)
    {
        bookings_destroy(&(ptr->tickets_bought));
        ptr = ptr->next_customer;
        free(*head);
        *head = ptr;
    }
}
int customers_count(Customer *head)
{
    int count = 0;
    Customer *ptr = head;
    while (ptr != NULL)
    {
        count++;
        ptr = ptr->next_customer;
    }
    return count;
}

Booking *create_booking()
{
    // Allocate memory for a booking
    Booking *b = (Booking *)malloc(sizeof(Booking));

    // Initialize to defaults
    b->event_booked = NULL;
    b->next_booking = NULL;

    return b;
}
void bookings_append(Booking **head, Booking *b)
{
    Booking *ptr = *head;
    if (ptr == NULL) // If LL is empty
    {
        *head = b;
        return;
    }
    while (ptr->next_booking != NULL) // Traverse to the end of the LL
    {
        ptr = ptr->next_booking;
    }
    ptr->next_booking = b;
}
void bookings_delete(Booking **head, char *event_id) // * Currently unused
{
    Booking *prev = *head;
    Booking *curr = *head;

    if (strcmp(curr->event_booked->event_id, event_id) == 0) // Delete from the beginning
    {
        *head = curr->next_booking;
        free(curr);
        curr = NULL;
        return;
    }
    while (strcmp(curr->event_booked->event_id, event_id) != 0) // Delete from the middle or end
    {
        prev = curr;
        curr = curr->next_booking;
    }
    prev->next_booking = curr->next_booking;
    free(curr);
    curr = NULL;
}
void bookings_destroy(Booking **head)
{
    Booking *ptr = *head;
    while (ptr != NULL)
    {
        ptr = ptr->next_booking;
        free(*head);
        *head = ptr;
    }
}
int bookings_count(Booking *head)
{
    int count = 0;
    Booking *ptr = head;
    while (ptr != NULL)
    {
        count++;
        ptr = ptr->next_booking;
    }
    return count;
}

void add_event(Event **head)
{
    Event *e = create_event(); // Event node to append to the LL
    char *string_temp;         // Temporary string variable

    // Event ID
    printf("Event ID: ");
    string_temp = get_string(ID_LENGTH);
    if (event_position(*head, string_temp) != -1)
    {
        printf("Error: Event ID already exists.\n\n");
        free(string_temp);
        free(e);
        return;
    }
    strcpy(e->event_id, string_temp);
    free(string_temp);

    // Event Title
    printf("Event Title: ");
    string_temp = get_string(STR_LENGTH);
    strcpy(e->event_title, string_temp);
    free(string_temp);

    // Artist
    printf("Artist: ");
    string_temp = get_string(STR_LENGTH);
    strcpy(e->artist, string_temp);
    free(string_temp);

    // Date and Time
    printf("Date and Time: ");
    string_temp = get_string(STR_LENGTH);
    strcpy(e->datetime, string_temp);
    free(string_temp);

    // Ticket Price
    do
    {
        printf("Ticket Price: ");
        scanf("%f", &e->ticket_price);
        clear_buffer();
        if (e->ticket_price < 0.0f)
        {
            printf("Error: Ticket price cannot be negative.\n\n");
        }
    } while (e->ticket_price < 0.0f);

    // Stock
    do
    {
        printf("Stock: ");
        scanf("%d", &e->stock);
        clear_buffer();
        if (e->stock < 0)
        {
            printf("Error: Stock cannot be negative.\n\n");
        }
    } while (e->stock < 0);

    events_insert(&(*head), e);

    printf("Success: Event added.\n\n");
}
void edit_event(Event *e_head, Customer *c_head)
{
    Event *e_ptr = e_head;
    Customer *c_ptr = c_head;
    Booking *b_ptr = NULL;
    char *event_id, *string_temp;

    view_events_l(e_head);

    printf("Enter the event ID to edit: ");
    event_id = get_string(ID_LENGTH);

    if (event_position(e_head, event_id) == -1)
    {
        printf("Error: Event does not exist.\n\n");
        free(event_id);
        return;
    }

    while (strcmp(e_ptr->event_id, event_id) != 0)
    {
        e_ptr = e_ptr->next_event;
    }

    // Date and Time
    printf("Date and Time: ");
    string_temp = get_string(STR_LENGTH);
    strcpy(e_ptr->datetime, string_temp);
    free(string_temp);

    // Subtract all costs of ticket price from customers
    while (c_ptr != NULL)
    {
        b_ptr = c_ptr->tickets_bought;
        while (b_ptr != NULL)
        {
            if (strcmp(b_ptr->event_booked->event_id, event_id) == 0)
            {
                c_ptr->total_cost -= b_ptr->event_booked->ticket_price;
            }
            b_ptr = b_ptr->next_booking; // Traverse
        }
        c_ptr = c_ptr->next_customer; // Traverse
    }

    // Ticket Price
    do
    {
        printf("Ticket Price: ");
        scanf("%f", &e_ptr->ticket_price);
        clear_buffer();
        if (e_ptr->ticket_price < 0.0f)
        {
            printf("Error: Ticket price cannot be negative.\n\n");
        }
    } while (e_ptr->ticket_price < 0.0f);

    // Add all costs of ticket to customers
    c_ptr = c_head;
    while (c_ptr != NULL)
    {
        b_ptr = c_ptr->tickets_bought;
        while (b_ptr != NULL)
        {
            if (strcmp(b_ptr->event_booked->event_id, event_id) == 0)
            {
                c_ptr->total_cost += b_ptr->event_booked->ticket_price;
            }
            b_ptr = b_ptr->next_booking; // Traverse
        }
        c_ptr = c_ptr->next_customer; // Traverse
    }
    // Stock
    do
    {
        printf("Stock: ");
        scanf("%d", &e_ptr->stock);
        clear_buffer();
        if (e_ptr->stock < 0)
        {
            printf("Error: Stock cannot be negative.\n\n");
        }
    } while (e_ptr->stock < 0);

    free(event_id);

    printf("Success: Event edited.\n\n");
}
void delete_event(Event **e_head)
{
    Event *e; // Event node

    char *event_id;

    view_events_l(*e_head); // Print all events

    printf("Enter the event ID to delete: ");
    event_id = get_string(ID_LENGTH);

    if (event_position(*e_head, event_id) == -1) // If the event does not exist, we cannot delete it
    {
        printf("Error: Event does not exist.\n\n");
        free(event_id);
        return;
    }

    e = *e_head;
    while (e != NULL)
    {
        if (strcmp(e->event_id, event_id) == 0)
        {
            if (e->sold_count != 0) // If there is at least one sold ticket, we cannot delete it
            {
                printf("Error: Ticket has been bought by a customer.\n\n");
                free(event_id);
                return;
            }
        }
        e = e->next_event;
    }

    events_delete(&(*e_head), event_id);
    free(event_id);

    printf("Success: Event deleted.\n\n");
}
void view_events(Event *head)
{
    Event *ptr = head;
    printf("\n");
    while (ptr != NULL)
    {
        printf("--------------------------------------\n");
        printf("Event ID: %s\n", ptr->event_id);
        printf("Event Title: %s\n", ptr->event_title);
        printf("Artist: %s\n", ptr->artist);
        printf("Date and Time: %s\n", ptr->datetime);
        printf("Ticket Price: %.2f\n", ptr->ticket_price);
        printf("Stock: %d\n", ptr->stock);
        // printf("Sold Count: %d\n", ptr->sold_count); // Hide the sold count from the user
        ptr = ptr->next_event;
    }
    printf("--------------------------------------\n\n");
}
void view_events_l(Event *head)
{
    Event *ptr = head;
    printf("--------------- EVENTS ---------------\n");
    while (ptr != NULL)
    {
        printf("%s | %s (%s) - %.2f | %d in stock\n",
               ptr->event_id,
               ptr->event_title,
               ptr->artist,
               ptr->ticket_price,
               ptr->stock);
        ptr = ptr->next_event;
    }
    printf("\n");
}
void view_customers(Customer *head)
{
    Customer *ptr = head;
    printf("\n");
    while (ptr != NULL)
    {
        printf("--------------------------------------\n");
        printf("Name: %s\n", ptr->name);
        printf("Tickets Bought: %d\n", bookings_count(ptr->tickets_bought));
        view_bookings(ptr->tickets_bought);
        printf("Total Cost: %.2f\n", ptr->total_cost);
        ptr = ptr->next_customer;
    }
    printf("--------------------------------------\n\n");
}
void buy_ticket(Event *e_head, Customer **c_head)
{
    Customer *c;    // Customer node
    Event *e;       // Event node
    Booking *b;     // Booking node
    int is_new = 0; // Boolean flag indicating if the customer is new or not

    char *string_temp; // Temporary string variable

    // Customer Name
    printf("Name: ");
    string_temp = get_string(STR_LENGTH);
    if (customer_position(*c_head, string_temp) == -1) // If the customer does not exist
    {
        is_new = 1;
        c = create_customer();
        strcpy(c->name, string_temp);
    }
    else // If the customer exists
    {
        c = *c_head;
        while (strcmp(c->name, string_temp) != 0)
        {
            c = c->next_customer;
        }
    }
    free(string_temp);

    view_events_l(e_head);

    // Event ID
    printf("Enter the event ID to buy a ticket: ");
    string_temp = get_string(ID_LENGTH);
    if (event_position(e_head, string_temp) == -1) // If the event does not exist
    {
        printf("Error: Event does not exist.\n\n");
        free(string_temp);
        free(c);
        return;
    }
    else // If the event exists
    {
        e = e_head;
        while (strcmp(e->event_id, string_temp) != 0)
        {
            e = e->next_event;
        }
    }
    free(string_temp);

    if (e->stock == 0)
    {
        printf("Error: Event is out of stock.\n\n");
        if (is_new)
        {
            free(c);
        }
        return;
    }

    // Booking (Tickets)
    b = create_booking();
    b->event_booked = e;
    b->next_booking = NULL;
    bookings_append(&(c->tickets_bought), b);

    // Update Customer Total Cost
    c->total_cost += e->ticket_price;

    // Update Event Details
    e->sold_count++;
    e->stock--;

    // Update Customer LL
    if (is_new)
    {
        customers_append(&(*c_head), c);
    }

    printf("Success: %s bought ticket %s (%s) - %s.\n\n",
           c->name,
           e->event_id,
           e->event_title,
           e->datetime);
}
void view_bookings(Booking *head)
{
    Booking *ptr = head;
    while (ptr != NULL)
    {
        printf("- %s | %s at %s\n",
               ptr->event_booked->event_id,
               ptr->event_booked->event_title,
               ptr->event_booked->datetime);
        ptr = ptr->next_booking;
    }
}

void save_events(Event *head)
{
    FILE *fp = fopen(EVENT_FILE, "w"); // Open the file
    int count = events_count(head);    // Obtain the event quantity
    fprintf(fp, "%d\n", count);        // Print to the file the event quantity

    Event *ptr = head;  // Pointer that reads every node in the Event LL
    while (ptr != NULL) // Print every Event node to the file
    {
        fprintf(fp, "%s\n", ptr->event_id);
        fprintf(fp, "%s\n", ptr->event_title);
        fprintf(fp, "%s\n", ptr->artist);
        fprintf(fp, "%s\n", ptr->datetime);
        fprintf(fp, "%.2f\n", ptr->ticket_price);
        fprintf(fp, "%d\n", ptr->stock);
        fprintf(fp, "%d\n", ptr->sold_count);

        ptr = ptr->next_event; // Traverse
    }

    fclose(fp); // Close the File
}
void save_customers(Customer *head)
{
    FILE *fp = fopen(CUSTOMER_FILE, "w"); // Open the file
    int c_count = customers_count(head);  // Obtain the customer quantity
    fprintf(fp, "%d\n", c_count);         // Print to the file the customer quantity

    Customer *cptr = head; // Pointer that reads every node in the Customer LL
    while (cptr != NULL)   // Print every Customer node to the file
    {
        fprintf(fp, "%s\n", cptr->name);
        fprintf(fp, "%d\n", bookings_count(cptr->tickets_bought));
        Booking *bptr = cptr->tickets_bought; // Pointer that reads every node in the Booking LL
        while (bptr != NULL)                  // Print every Booking node to the file
        {
            fprintf(fp, "%s\t%s\t%s\n",
                    bptr->event_booked->event_id,
                    bptr->event_booked->event_title,
                    bptr->event_booked->datetime);
            bptr = bptr->next_booking; // Traverse
        }
        fprintf(fp, "%.2f\n", cptr->total_cost);
        cptr = cptr->next_customer; // Traverse
    }

    fclose(fp); // Close the file
}
void load_events(Event **head)
{
    Event *temp = NULL;               // Temporary Event node
    char string_temp[STR_LENGTH + 1]; // Temporary string variable
    int count = 0;                    // Hold the event quantity

    FILE *fp = fopen(EVENT_FILE, "r"); // Open the file
    if (fp == NULL)                    // If the file does not exist, create the file
    {
        printf("File events.txt not found\nCreating events.txt...\n");
        fp = fopen(EVENT_FILE, "w");
        fclose(fp);
        return;
    }

    fscanf(fp, "%d\n", &count); // Scan the event quantity (and consume newline/whitespace characters)

    if (count == 0) // There is nothing to read
    {
        fclose(fp);
        return;
    }

    for (int i = 0; i < count; i++)
    {
        temp = create_event(); // Allocate memory for the Event

        // Event ID
        fgets(string_temp, ID_LENGTH + 1, fp); // + 1 to account for the '\n' in the file
        string_temp[strcspn(string_temp, "\n")] = '\0';
        strcpy(temp->event_id, string_temp);

        // Event Title
        fgets(string_temp, STR_LENGTH + 1, fp);
        string_temp[strcspn(string_temp, "\n")] = '\0';
        strcpy(temp->event_title, string_temp);

        // Artist
        fgets(string_temp, STR_LENGTH + 1, fp);
        string_temp[strcspn(string_temp, "\n")] = '\0';
        strcpy(temp->artist, string_temp);

        // Date and Time
        fgets(string_temp, STR_LENGTH + 1, fp);
        string_temp[strcspn(string_temp, "\n")] = '\0';
        strcpy(temp->datetime, string_temp);

        // Ticket Price
        fscanf(fp, "%f\n", &temp->ticket_price);

        // Stock
        fscanf(fp, "%d\n", &temp->stock);

        // Sold Count
        fscanf(fp, "%d\n", &temp->sold_count);

        // Insert to Linked List
        events_insert(&(*head), temp);
    }
}
void load_customers(Event *e_head, Customer **c_head)
{
    Customer *c_temp = NULL;      // Temporary Customer node (to hold data)
    Booking *b_temp = NULL;       // Temporary Booking node (to hold data)
    Event *e_ptr = NULL;          // Temporary Event node (to be assigned)
    char string_temp[STR_LENGTH]; // Temporary string variable
    int c_count = 0;              // Hold the customer quantity
    int b_count = 0;              // Hold the booking quantity (per customer)
    float total_cost = 0.0f;      // Hold the total cost

    FILE *fp = fopen(CUSTOMER_FILE, "r"); // Open the file
    if (fp == NULL)                       // If the file does not exist, create the file
    {
        printf("File customers.txt not found\nCreating customers.txt...\n");
        fp = fopen(CUSTOMER_FILE, "w");
        fclose(fp);
        return;
    }

    fscanf(fp, "%d\n", &c_count); // Scan the customer quantity (and consume newline/whitespace characters)

    if (c_count == 0) // There is nothing to read
    {
        fclose(fp);
        return;
    }

    for (int i = 0; i < c_count; i++)
    {
        c_temp = create_customer(); // Allocate memory for Customer

        // Customer Name
        fgets(string_temp, STR_LENGTH, fp);
        string_temp[strcspn(string_temp, "\n")] = '\0';
        strcpy(c_temp->name, string_temp);

        // Ticket Count
        fscanf(fp, "%d\n", &b_count);
        for (int j = 0; j < b_count; j++)
        {
            b_temp = create_booking();             // Allocate memory for Booking
            b_temp->event_booked = create_event(); // Allocate memory for an Event in Booking
            // Bookings
            fscanf(fp, "%[^\t\n]\t%[^\t\n]\t%[^\t\n]\n",
                   b_temp->event_booked->event_id,
                   b_temp->event_booked->event_title,
                   b_temp->event_booked->datetime);
            e_ptr = e_head;
            while (strcmp(e_ptr->event_id, b_temp->event_booked->event_id) != 0)
            {
                e_ptr = e_ptr->next_event;
            }
            free(b_temp->event_booked); // To remove the
            b_temp->event_booked = e_ptr;

            // Append to Linked List (Bookings)
            bookings_append(&c_temp->tickets_bought, b_temp);
        }

        // Total Cost
        fscanf(fp, "%f\n", &c_temp->total_cost);

        // Append to Linked List
        customers_append(&(*c_head), c_temp);
    }
}