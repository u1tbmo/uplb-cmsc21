/*  Tabamo, Euan Jed S. - ST2L - March 27, 2024
    Exercise 9 - Structures and Files
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global Variables
#define STR_MAX_LENGTH 51
#define MAX_EVENTS 10
#define MAX_CUSTOMERS 10
#define MAX_TICKETS_PER_CUSTOMER 5
#define CUSTOMERS_FILE "customers.txt"
#define EVENTS_FILE "events.txt"

// Structures
typedef struct event
{
    int event_id;                       // the event identifier
    char event_title[STR_MAX_LENGTH];   // the event title
    char artist[STR_MAX_LENGTH];        // the artist of the event
    char date_and_time[STR_MAX_LENGTH]; // the date and time of the event
    float ticket_price;                 // the price of the ticket for the event
    int stock;                          // the number of tickers remaining
} event;

typedef struct customer
{
    char *name;
    int tickets_bought;
    event tickets[MAX_TICKETS_PER_CUSTOMER];
    float total_cost;

} customer;

// Function Prototypes
int menu();                                                                    // prints the menu and returns the choice of the user
char *get_string();                                                            // gets a string from the user
int is_duplicate(event *e_array, int e_count, int event_id);                   // checks whether the event identifier of `e` already exists
void clear_buffer();                                                           // clears the input buffer
void add_event(event *e_array, int *e_count);                                  // adds an event to the array
void buy_ticket(event *e_array, int e_count, customer *c_array, int *c_count); // buys a ticket for a customer
void edit_event(event *e_array, int e_count);                                  // edits an event
void delete_event(event *e_array, int *e_count);                               // deletes an event
void view_events(event *e_array, int e_count);                                 // views all events
void view_customers(customer *c_array, int c_count);                           // views all customers
void save_events(char *e_file, event *e_array, int e_count);                   // saves all events
void save_customers(char *c_file, customer *c_array, int c_count);             // saves all customers
void load_events(char *e_file, event *e_array, int e_count);                   // loads all events
void load_customers(char *c_file, customer *c_array, int c_count);             // loads all customers

int main()
{
    int e_count = 0, c_count = 0, running = 1;

    // Arrays
    customer *c_array = (customer *)malloc(sizeof(customer) * MAX_CUSTOMERS);
    event *e_array = (event *)malloc(sizeof(event) * MAX_EVENTS);

    // Load
    // TODO: load(EVENTS_FILE, CUSTOMERS_FILE, e_array, c_array);

    while (running)
    {
        switch (menu())
        {
        case 1:
            add_event(e_array, &e_count);
            break;
        case 2:
            if (e_count == 0 && c_count != 0)
            {
                printf("Error: No events.\n");
                break;
            }
            else if (e_count != 0 && c_count == 0)
            {
                printf("Error: No customers.\n");
                break;
            }
            else if (e_count == 0 && c_count == 0)
            {
                printf("Error: No events and customers.\n");
                break;
            }
            buy_ticket(e_array, e_count, c_array, c_count);
            break;
        case 3:
            if (e_count == 0)
            {
                printf("Error: No events.\n");
                break;
            }
            edit_event(e_array, e_count);
            break;
        case 4:
            if (e_count == 0)
            {
                printf("Error: No events.\n");
                break;
            }
            delete_event(e_array, &e_count);
            break;
        case 5:
            if (e_count == 0)
            {
                printf("Error: No events.\n");
                break;
            }
            view_events(e_array, e_count);
            break;
        case 6:
            if (c_count == 0)
            {
                printf("Error: No customers.\n");
                break;
            }
            view_customers(c_array, c_count);
            break;
        case 0:
            printf("Goodbye!\n");
            running = !running;
            break;
        default:
            printf("Error: Invalid choice.\n");
            break;
        }
    }

    // Free allocated memory
    free(c_array);
    free(e_array);
}

int menu()
{
    int choice;
    printf("---------- MENU ----------\n");
    printf("1 | Add Event Details\n");
    printf("2 | Buy Ticket\n");
    printf("3 | Edit Event Details\n");
    printf("4 | Delete Event\n");
    printf("5 | View All Events\n");
    printf("6 | View All Customers\n");
    printf("0 | Exit\n");
    printf("--------------------------\n");
    printf("Enter Choice: ");
    scanf("%d", &choice);
    clear_buffer();

    printf("\n");

    return choice;
}

char *get_string()
{
    // Allocate enough memory for a  string with length STR_MAX_LENGTH
    char *temp = (char *)malloc(STR_MAX_LENGTH * sizeof(char));
    if (temp == NULL) // If malloc failed
    {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }

    // Read a max of 50 characters.
    if (fgets(temp, STR_MAX_LENGTH, stdin) == NULL) // If fgets failed
    {
        printf("Error: Unable to read input.\n");
        free(temp);
        return NULL;
    }

    // Find the new line if it exists and replace it with the null terminator
    char *newline = strchr(temp, '\n'); // strchr returns a pointer to the newline or NULL if it doesn't exist
    if (newline != NULL)
    {
        *newline = '\0';
    }
    else
    {
        clear_buffer();
    }

    return temp;
}

int is_duplicate(event *e_array, int e_count, int event_id)
{
    for (int i = 0; i < e_count; i++)
    {
        if (event_id == e_array[i].event_id)
        {
            return 1; // the id is a duplicate
        }
    }
    return 0; // the id is not a duplicate
}

void clear_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // clears the input buffer
}

void add_event(event *e_array, int *e_count)
{
    int event_id, stock;
    float ticket_price;

    // Ask for an event id
    do
    {
        printf("Event ID: ");
        scanf("%d", &event_id);
        clear_buffer();
        if (event_id <= 0)
        {
            printf("Error: Invalid event ID.\n");
        }
        if (is_duplicate(e_array, *e_count, event_id))
        {
            printf("Error: Event ID already exists in database.\n");
        }
    } while (is_duplicate(e_array, *e_count, event_id) || event_id <= 0);
    e_array[*e_count].event_id = event_id;

    char *temp;

    // Event Title
    printf("Event Title: ");
    temp = get_string();
    strcpy(e_array[*e_count].event_title, temp);
    free(temp);

    // Artist
    printf("Artist: ");
    temp = get_string();
    strcpy(e_array[*e_count].artist, temp);
    free(temp);

    // Date and Time
    printf("Date and Time: ");
    temp = get_string();
    strcpy(e_array[*e_count].date_and_time, temp);
    free(temp);

    // Ticket Price
    do
    {
        printf("Ticket Price: ");
        scanf("%f", &ticket_price);
        if (ticket_price < 0)
        {
            printf("Error: Ticket price must be at least 0 (free).\n");
        }
    } while (ticket_price < 0);
    e_array[*e_count].ticket_price = ticket_price;

    // Stock
    do
    {
        printf("Stock: ");
        scanf("%d", &stock);
        if (stock < 0)
        {
            printf("Error: Stock must be at least 0.\n");
        }
    } while (stock < 0);
    e_array[*e_count].stock = stock;

    // Increment Count
    (*e_count)++;
}

void buy_ticket(event *e_array, int e_count, customer *c_array, int *c_count)
{
}
void edit_event(event *e_array, int e_count)
{
}
void delete_event(event *e_array, int *e_count)
{
    int event_id;

    // Ask for an event id
    do
    {
        printf("Event ID: ");
        scanf("%d", &event_id);
        clear_buffer();
        if (event_id <= 0)
        {
            printf("Error: Invalid event ID.\n");
        }
    } while (event_id <= 0);

    for (int i = 0; i < *e_count; i++)
    {
        if (e_array[i].event_id == event_id)
        {
            printf("Success: Deleted event %d\n\n", event_id);
            for (int j = i; j < *e_count - 1; j++)
            {
                e_array[j] = e_array[j + 1];
            }
            (*e_count)--;
            return;
        }
    }
}
void view_events(event *e_array, int e_count)
{
    for (int i = 0; i < e_count; i++)
    {
        printf("--------------------------\n");
        printf("Event ID: %d\n", e_array[i].event_id);
        printf("Event Title: %s\n", e_array[i].event_title);
        printf("Artist: %s\n", e_array[i].artist);
        printf("Date and Time: %s\n", e_array[i].date_and_time);
        printf("Ticket Price: %.2f\n", e_array[i].ticket_price);
        printf("Stock: %d\n", e_array[i].stock);
        printf("--------------------------\n");
    }
}
void view_customers(customer *c_array, int c_count)
{
    int cost;

    for (int i = 0; i < c_count; i++)
    {
        cost = 0;
        printf("--------------------------\n");
        printf("Customer Name: %s\n", c_array[i].name);
        printf("Tickets Bought: \n");
        for (int j = 0; j < c_array[i].tickets_bought; j++)
        {
            cost += c_array[i].tickets[j].ticket_price;
            printf("[%d] %s %s", c_array[i].tickets[j].event_id, c_array[i].tickets[j].event_title, c_array[i].tickets[j].date_and_time);
        }
        printf("Total Cost: %.2f\n", cost);
        printf("--------------------------\n");
    }
}

void save_events(char *e_file, event *e_array, int e_count)
{
}
void save_customers(char *c_file, customer *c_array, int c_count)
{
}
void load_events(char *e_file, event *e_array, int e_count)
{
}
void load_customers(char *c_file, customer *c_array, int c_count)
{
}