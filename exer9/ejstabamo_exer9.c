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
    char name[STR_MAX_LENGTH];
    int tickets_bought;
    event tickets[MAX_TICKETS_PER_CUSTOMER];
    float total_cost;

} customer;

// Function Prototypes
int menu();                                                                    // prints the menu and returns the choice of the user
char *get_string();                                                            // gets a string from the user
void clear_buffer();                                                           // clears the input buffer
void add_event(event *e_array, int *e_count);                                  // adds an event to the array
void edit_event(event *e_array, int e_count);                                  // edits an event
void delete_event(event *e_array, int *e_count);                               // deletes an event
void view_events(event *e_array, int e_count);                                 // views all events
void view_events_l(event *e_array, int e_count);                               // views all events in linear format
void save_events(char *e_file, event *e_array, int e_count);                   // saves all events
void load_events(char *e_file, event *e_array, int e_count);                   // loads all events
void buy_ticket(event *e_array, int e_count, customer *c_array, int *c_count); // buys a ticket for a customer
void add_customer(customer *c_array, int *c_count, char *c_name);              // adds a customer to the array
void view_customers(customer *c_array, int c_count);                           // views all customers
void save_customers(char *c_file, customer *c_array, int c_count);             // saves all customers
void load_customers(char *c_file, customer *c_array, int c_count);             // loads all customers
int event_exists(event *e_array, int e_count, int event_id);                   // checks whether the event identifier of `e` already exists in the database
int customer_exists(customer *c_array, int c_count, char *c_name);             // checks whether a customer exists in the database, returns index
int events_in_stock(event *e_array, int e_count);                              // checks if there is at least one event with one stock

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
            if (e_count == 0)
            {
                printf("Error: No events.\n\n");
                break;
            }
            buy_ticket(e_array, e_count, c_array, &c_count);
            break;
        case 3:
            if (e_count == 0)
            {
                printf("Error: No events.\n\n");
                break;
            }
            edit_event(e_array, e_count);
            break;
        case 4:
            if (e_count == 0)
            {
                printf("Error: No events.\n\n");
                break;
            }
            delete_event(e_array, &e_count);
            break;
        case 5:
            if (e_count == 0)
            {
                printf("Error: No events.\n\n");
                break;
            }
            view_events(e_array, e_count);
            break;
        case 6:
            if (c_count == 0)
            {
                printf("Error: No customers.\n\n");
                break;
            }
            view_customers(c_array, c_count);
            break;
        case 0:
            printf("Goodbye!\n");
            running = !running;
            break;
        default:
            printf("Error: Invalid choice.\n\n");
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

void clear_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // clears the input buffer
}

int event_exists(event *e_array, int e_count, int event_id)
{
    for (int i = 0; i < e_count; i++)
    {
        if (event_id == e_array[i].event_id)
        {
            return i;
        }
    }
    return -1;
}

void add_event(event *e_array, int *e_count)
{
    int event_id, stock;
    float ticket_price;

    if (*e_count == MAX_EVENTS)
    {
        printf("Error: Reached maximum number of events.\n");
        return;
    }

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
        if (event_exists(e_array, *e_count, event_id) != -1)
        {
            printf("Error: Event ID already exists in database.\n");
        }
    } while (event_exists(e_array, *e_count, event_id) != -1 || event_id <= 0);
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
    char *temp;
    int c_index, e_index, event_id;

    if (!events_in_stock(e_array, e_count))
    {
        view_events_l(e_array, e_count);
        printf("Error: All events are out of stock.\n\n");
        return;
    }

    // Ask for a name
    printf("Enter Customer Name: ");
    temp = get_string();

    do
    {
        if ((c_index = customer_exists(c_array, *c_count, temp)) == -1)
        {
            if (*c_count == MAX_CUSTOMERS)
            {
                free(temp);
                printf("Error: Reached maximum number of customers.\n");
                return;
            }
            add_customer(c_array, c_count, temp);
        }
    } while (c_index == -1);

    if (c_array[c_index].tickets_bought == MAX_TICKETS_PER_CUSTOMER)
    {
        printf("Error: Customer has bought the maximum number of tickets.\n");
        return;
    }

    // Print events
    view_events_l(e_array, e_count);

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
        else if ((e_index = event_exists(e_array, e_count, event_id)) == -1)
        {
            printf("Error: Event with ID %d does not exist.\n", event_id);
        }
        else if (e_array[e_index].stock == 0)
        {
            printf("Error: Tickets are out of stock.\n");
        }
    } while (event_id <= 0 || e_index == -1);

    // Add the event to the customer's tickets array
    c_array[c_index].tickets[c_array[c_index].tickets_bought] = e_array[e_index];
    c_array[c_index].total_cost += e_array[e_index].ticket_price;
    c_array[c_index].tickets_bought++;
    e_array[e_index].stock--;

    free(temp);
}

void edit_event(event *e_array, int e_count)
{
}

void delete_event(event *e_array, int *e_count)
{
    int event_id, i;

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

    if ((i = event_exists(e_array, *e_count, event_id)) != -1)
    {
        printf("Success: Deleted event %d\n\n", event_id);
        for (int j = i; j < *e_count - 1; j++)
        {
            e_array[j] = e_array[j + 1];
        }
        (*e_count)--;
        return;
    }
    printf("Error: Event with ID %d does not exist.\n\n", event_id);
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
    }
    printf("--------------------------\n\n");
}

void view_events_l(event *e_array, int e_count)
{
    printf("\n---- EVENTS AVAILABLE ----\n");
    for (int i = 0; i < e_count; i++)
    {
        printf("[%d] %s (%s) - %.2f | %d in stock\n",
               e_array[i].event_id,
               e_array[i].event_title,
               e_array[i].artist,
               e_array[i].ticket_price,
               e_array[i].stock);
    }
    printf("\n");
}

void save_events(char *e_file, event *e_array, int e_count)
{
}

void load_events(char *e_file, event *e_array, int e_count)
{
}

int customer_exists(customer *c_array, int c_count, char *c_name)
{
    for (int i = 0; i < c_count; i++)
    {
        if (strcmp(c_name, c_array[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}
void add_customer(customer *c_array, int *c_count, char *c_name)
{
    // Copy the name to the customer name field
    strcpy(c_array[*c_count].name, c_name);

    // Tickets Bought
    c_array[*c_count].tickets_bought = 0;

    // Total Cost
    c_array[*c_count].total_cost = 0;

    // Increment Count
    (*c_count)++;
}

void view_customers(customer *c_array, int c_count)
{
    float cost;

    for (int i = 0; i < c_count; i++)
    {
        cost = 0;
        printf("--------------------------\n");
        printf("Customer Name: %s\n", c_array[i].name);
        printf("Tickets Bought: \n");
        for (int j = 0; j < c_array[i].tickets_bought; j++)
        {
            cost += c_array[i].tickets[j].ticket_price;
            printf("+ [%d] %s @ %s\n",
                   c_array[i].tickets[j].event_id,
                   c_array[i].tickets[j].event_title,
                   c_array[i].tickets[j].date_and_time);
        }
        printf("Total Cost: %.2f\n", cost);
        printf("--------------------------\n\n");
    }
}

void save_customers(char *c_file, customer *c_array, int c_count)
{
}

void load_customers(char *c_file, customer *c_array, int c_count)
{
}

int events_in_stock(event *e_array, int e_count)
{
    for (int i = 0; i < e_count; i++)
    {
        if (e_array[i].stock != 0)
        {
            return 1;
        }
    }
    return 0;
}