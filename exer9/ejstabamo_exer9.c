/*  Tabamo, Euan Jed S. - ST2L - April 23, 2024
    Exercise 9 - Structures and Files
*/

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
typedef struct Event
{
    int event_id;                       // the Event identifier
    char event_title[STR_MAX_LENGTH];   // the Event title
    char artist[STR_MAX_LENGTH];        // the artist of the Event
    char date_and_time[STR_MAX_LENGTH]; // the date and time of the Event
    float ticket_price;                 // the price of the ticket for the Event
    int stock;                          // the number of tickers remaining
} Event;

typedef struct Ticket // Ticket is a subset of Event
{
    int event_id;                       // the Ticket's event Id
    char event_title[STR_MAX_LENGTH];   // the Ticket's event title
    char date_and_time[STR_MAX_LENGTH]; // the Ticket's date and time
    float ticket_price;                 // the Ticket's price
} Ticket;

typedef struct Customer
{
    char name[STR_MAX_LENGTH];                // the name of the Customer
    int tickets_bought;                       // the number of Tickets bought by the Customer
    Ticket tickets[MAX_TICKETS_PER_CUSTOMER]; // the Tickets bought by the Customer
    float total_cost;                         // the total cost of all Tickets

} Customer;

// Utility functions
int menu();                                                        // prints the menu and returns the choice of the user
char *get_string();                                                // gets a string from the user
void clear_buffer();                                               // clears the input buffer
int events_in_stock(Event *e_array, int e_count);                  // checks if all events have at least 1 stock
int event_exists(Event *e_array, int e_count, int event_id);       // checks whether the Event already exists in the database
int customer_exists(Customer *c_array, int c_count, char *c_name); // checks whether a Customer exists in the database, returns index
int ticket_exists(Ticket *t_array, int t_count, int event_id);     // checks whether the Ticket already exists in the database
void initialize_events(Event *e_array);                            // initializes the Event array to default values
void initialize_customers(Customer *c_array);                      // initializes the Customer array to default values

// Event functions
void add_event(Event *e_array, int *e_count);                                     // adds an Event to the array
void edit_event(Event *e_array, int e_count, Customer *c_array, int c_count);     // edits an Event
void delete_event(Event *e_array, int *e_count, Customer *c_array, int *c_count); // deletes an Event
void view_events(Event *e_array, int e_count);                                    // views all Events
void view_events_l(Event *e_array, int e_count);                                  // views all Events in linear format
void save_events(char *e_file, Event *e_array, int e_count);                      // saves all Events
void load_events(char *e_file, Event *e_array, int *e_count);                     // loads all Events

// Customer functions
void add_customer(Customer *c_array, int *c_count, char *c_name);   // adds a Customer to the array
void view_customers(Customer *c_array, int c_count);                // views all Customers
void save_customers(char *c_file, Customer *c_array, int c_count);  // saves all Customers
void load_customers(char *c_file, Customer *c_array, int *c_count); // loads all Customers
void delete_empty_customers(Customer *c_array, int *c_count);       // deletes Customers with 0 tickets bought

// Ticket function
void buy_ticket(Event *e_array, int e_count, Customer *c_array, int *c_count); // buys a Ticket for a Customer

// Main Function
int main()
{
    // Initialize event and customer counts, and the user's meny choice
    int e_count = 0, c_count = 0, choice;

    // Customer and Event Arrays
    Event e_array[MAX_EVENTS];
    Customer c_array[MAX_CUSTOMERS];

    // Initialize events and customers to default values
    initialize_events(e_array);
    initialize_customers(c_array);

    // Load events and customers from files
    load_events(EVENTS_FILE, e_array, &e_count);
    load_customers(CUSTOMERS_FILE, c_array, &c_count);

    // Main program loop
    do
    {
        // Display meny and get user's choice
        switch (choice = menu())
        {
        case 1: // Add a new event
            add_event(e_array, &e_count);
            break;
        case 2: // Buy a ticket, if there are any events
            if (e_count == 0)
            {
                printf("Error: No events.\n\n");
                break;
            }
            buy_ticket(e_array, e_count, c_array, &c_count);
            break;
        case 3: // Edit an event, if there are any events
            if (e_count == 0)
            {
                printf("Error: No events.\n\n");
                break;
            }
            edit_event(e_array, e_count, c_array, c_count);
            break;
        case 4: // Delete an event, if there are any events
            if (e_count == 0)
            {
                printf("Error: No events.\n\n");
                break;
            }
            delete_event(e_array, &e_count, c_array, &c_count);
            break;
        case 5: // View all events, if there are any events
            if (e_count == 0)
            {
                printf("Error: No events.\n\n");
                break;
            }
            view_events(e_array, e_count);
            break;
        case 6: // View all customers, if there are any customers
            if (c_count == 0)
            {
                printf("Error: No customers.\n\n");
                break;
            }
            view_customers(c_array, c_count);
            break;
        case 0: // Exit the program
            // Save events and customers to files
            save_events(EVENTS_FILE, e_array, e_count);
            save_customers(CUSTOMERS_FILE, c_array, c_count);

            printf("Goodbye!\n");
            break;
        default: // Handle invalid choices
            printf("Error: Invalid choice.\n\n");
            break;
        }
        delete_empty_customers(c_array, &c_count); // Delete any Customers with 0 tickets
    } while (choice != 0);
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
        printf("Error: Memory allocation failed.\n\n");
        return NULL;
    }

    // Read a max of STR_MAX_LENGTH characters.
    if (fgets(temp, STR_MAX_LENGTH, stdin) == NULL) // If fgets failed
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

int event_exists(Event *e_array, int e_count, int event_id)
{
    for (int i = 0; i < e_count; i++)
    {
        if (event_id == e_array[i].event_id)
        {
            return i; // return the index of the Event in the array
        }
    }
    return -1;
}

int customer_exists(Customer *c_array, int c_count, char *c_name)
{
    for (int i = 0; i < c_count; i++)
    {
        if (strcmp(c_name, c_array[i].name) == 0)
        {
            return i; // return the index of the Customer in the array
        }
    }
    return -1;
}

int ticket_exists(Ticket *t_array, int t_count, int event_id)
{
    for (int i = 0; i < t_count; i++)
    {
        if (event_id == t_array[i].event_id)
        {
            return i; // return the index of the Ticket in the array
        }
    }
    return -1;
}

void initialize_events(Event *e_array)
{
    // Prevents conditional jump or move errors from valgrind
    // which occurs when accessing fields that are uninitialized
    for (int i = 0; i < MAX_EVENTS; i++)
    {
        e_array[i].event_id = 0;
        strcpy(e_array[i].event_title, "");
        strcpy(e_array[i].artist, "");
        strcpy(e_array[i].date_and_time, "");
        e_array[i].ticket_price = 0;
        e_array[i].stock = 0;
    }
}

void initialize_customers(Customer *c_array)
{
    // Prevents conditional jump or move errors from valgrind
    // which occurs when accessing fields that are uninitialized
    for (int i = 0; i < MAX_CUSTOMERS; i++)
    {
        strcpy(c_array[i].name, "");
        c_array[i].tickets_bought = 0;
        c_array[i].total_cost = 0;
        for (int j = 0; j < MAX_TICKETS_PER_CUSTOMER; j++)
        {
            c_array[i].tickets[j].event_id = 0;
            strcpy(c_array[i].tickets[j].event_title, "");
            strcpy(c_array[i].tickets[j].date_and_time, "");
            c_array[i].tickets[j].ticket_price = 0;
        }
    }
}

void add_event(Event *e_array, int *e_count)
{
    // Variables
    int event_id, stock;
    float ticket_price;
    char *temp;

    // Don't allow adding more events when we reach the limit
    if (*e_count == MAX_EVENTS)
    {
        printf("Error: Reached maximum number of events.\n\n");
        return;
    }

    // Event ID
    do
    {
        printf("Event ID: ");
        scanf("%d", &event_id);
        clear_buffer();
        if (event_id <= 0)
        {
            printf("Error: Invalid Event ID.\n\n");
        }
        if (event_exists(e_array, *e_count, event_id) != -1)
        {
            printf("Error: Event ID already exists in database.\n\n");
            return;
        }
    } while (event_exists(e_array, *e_count, event_id) != -1 || event_id <= 0);
    e_array[*e_count].event_id = event_id;

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
        clear_buffer();
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
        clear_buffer();
        if (stock < 0)
        {
            printf("Error: Stock must be at least 0.\n");
        }
    } while (stock < 0);
    e_array[*e_count].stock = stock;

    // Increment Count
    (*e_count)++;

    // Print message
    printf("Success: Added event!\n\n");
}

void edit_event(Event *e_array, int e_count, Customer *c_array, int c_count)
{
    // Variables
    int event_id, e_index, stock;
    float ticket_price;
    char *temp;

    // Print events
    view_events_l(e_array, e_count);

    // Event ID
    do
    {
        printf("Enter Event ID: ");
        scanf("%d", &event_id);
        clear_buffer();
        if (event_id <= 0)
        {
            printf("Error: Invalid Event ID.\n");
        }
        else if ((e_index = event_exists(e_array, e_count, event_id)) == -1)
        {
            printf("Error: Event with ID %d does not exist.\n\n", event_id);
            return;
        }
    } while (event_id <= 0 || e_index == -1);
    printf("\n");

    // Ask for new details

    // Date and Time
    printf("Enter new Date and Time: ");
    temp = get_string();
    strcpy(e_array[e_index].date_and_time, temp);

    // ! UNCLEAR SPECIFICATIONS: Should editing the Date and Time of the Event
    // ! also edit the Date and Time of the Tickets bought by the customers?
    // ! Opted to not edit the date and time of the customers, since
    // ! edit_event() can be used to "re-stock" tickets for a new Date and Time
    // for (int i = 0; i < c_count; i++) // Edit the date and time of all Tickets with the same event ID
    // {
    //     for (int j = 0; j < c_array[i].tickets_bought; j++)
    //     {
    //         if (c_array[i].tickets[j].event_id == e_array[e_index].event_id)
    //         {
    //             strcpy(c_array[i].tickets[j].date_and_time, temp);
    //         }
    //     }
    // }
    free(temp);

    // Ticket Price
    do
    {
        printf("Enter new Ticket Price: ");
        scanf("%f", &ticket_price);
        clear_buffer();
        if (ticket_price < 0)
        {
            printf("Error: Ticket price must be at least 0 (free).\n");
        }
    } while (ticket_price < 0);
    e_array[e_index].ticket_price = ticket_price;

    // Stock
    do
    {
        printf("Enter new Stock: ");
        scanf("%d", &stock);
        clear_buffer();
        if (stock < 0)
        {
            printf("Error: Stock must be at least 0.\n");
        }
    } while (stock < 0);
    e_array[e_index].stock = stock;
    printf("Success: Edited event!\n\n");
}

void delete_event(Event *e_array, int *e_count, Customer *c_array, int *c_count)
{
    // Variables
    int event_id, i;

    // Print events
    view_events_l(e_array, *e_count);

    // Ask for an Event id
    do
    {
        printf("Event ID: ");
        scanf("%d", &event_id);
        clear_buffer();
        if (event_id <= 0)
        {
            printf("Error: Invalid Event ID.\n");
        }
    } while (event_id <= 0);

    if ((i = event_exists(e_array, *e_count, event_id)) != -1)
    {
        printf("Success: Deleted Event %d\n\n", event_id);
        for (int j = i; j < *e_count - 1; j++)
        {
            e_array[j] = e_array[j + 1];
        }
        (*e_count)--;
    }
    else
    {
        printf("Error: Event with ID %d does not exist.\n\n", event_id);
    }

    // ! UNCLEAR SPECIFICATIONS: Should deleting an event delete tickets
    // ! with the same event ID from customers?
    // ! Opted to delete tickets to prevent tickets with conflicting event IDs.
    for (int h = 0; h < *c_count; h++)
    {
        for (int k = 0; k < c_array[h].tickets_bought;)
        {
            if ((i = ticket_exists(c_array[h].tickets, c_array[h].tickets_bought, event_id)) != -1)
            {
                printf("Success: Deleted Ticket %d from Customer %s\n", event_id, c_array[h].name);
                c_array[h].total_cost -= c_array[h].tickets[i].ticket_price; // Deduct from total cost
                for (int l = i; l < c_array[h].tickets_bought - 1; l++)
                {
                    c_array[h].tickets[l] = c_array[h].tickets[l + 1]; // Shift tickets to the left, essentially deleting the ticket
                }
                c_array[h].tickets_bought--;
            }
            else
            {
                k++;
            }
        }
    }
    printf("\n");
}

void view_events(Event *e_array, int e_count)
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

void view_events_l(Event *e_array, int e_count)
{
    printf("\n--------- EVENTS ---------\n");
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

void save_events(char *e_file, Event *e_array, int e_count)
{

    FILE *fp = fopen(e_file, "w");

    // Write the count
    fprintf(fp, "%d\n", e_count);

    if (e_count == 0)
    {
        printf("No events were saved.");
        return;
    }

    for (int i = 0; i < e_count; i++)
    {
        fprintf(fp, "%d\n", e_array[i].event_id);
        fprintf(fp, "%s\n", e_array[i].event_title);
        fprintf(fp, "%s\n", e_array[i].artist);
        fprintf(fp, "%s\n", e_array[i].date_and_time);
        fprintf(fp, "%.2f\n", e_array[i].ticket_price);
        fprintf(fp, "%d\n", e_array[i].stock);
    }
    fclose(fp);
}

void load_events(char *e_file, Event *e_array, int *e_count)
{
    // Variable
    char temp[STR_MAX_LENGTH];

    FILE *fp = fopen(e_file, "r");
    if (fp == NULL)
    {
        printf("File events.txt not found!\nCreating events.txt file...\n\n");
        fp = fopen(e_file, "w");
        fclose(fp);
        return; // since there is nothing to read
    }

    // Read the count
    fscanf(fp, "%d\n", e_count);

    // If *e_count is 0, there is nothing to read.
    if (*e_count == 0)
    {
        return;
    }

    int i = 0;
    int status = 0; // 0 for ID, 1 for Title, 2 for Artist, 3 for Date and Time, 4 for Ticket Price, 5 for Stock
    while (fgets(temp, STR_MAX_LENGTH, fp) != NULL)
    {
        switch (status)
        {
        case 0: // ID
            sscanf(temp, "%d", &e_array[i].event_id);
            status++;
            break;
        case 1: // TITLE
            if (temp[strlen(temp) - 1] == '\n')
            {
                temp[strlen(temp) - 1] = '\0';
            }
            strcpy(e_array[i].event_title, temp);
            status++;
            break;
        case 2: // ARTIST
            if (temp[strlen(temp) - 1] == '\n')
            {
                temp[strlen(temp) - 1] = '\0';
            }
            strcpy(e_array[i].artist, temp);
            status++;
            break;
        case 3: // DATE AND TIME
            if (temp[strlen(temp) - 1] == '\n')
            {
                temp[strlen(temp) - 1] = '\0';
            }
            strcpy(e_array[i].date_and_time, temp);
            status++;
            break;
        case 4: // TICKET PRICE
            sscanf(temp, "%f", &e_array[i].ticket_price);
            status++;
            break;
        case 5: // STOCK
            sscanf(temp, "%d", &e_array[i].stock);
            i++;
            status = 0;
            break;

        default:
            break;
        }
    }
    fclose(fp);
}

void add_customer(Customer *c_array, int *c_count, char *c_name)
{
    strcpy(c_array[*c_count].name, c_name); // Customer Name
    c_array[*c_count].tickets_bought = 0;   // Tickets Bought
    c_array[*c_count].total_cost = 0;       // Total Cost
    (*c_count)++;                           // Increment Count
}

void view_customers(Customer *c_array, int c_count)
{
    // Variable
    float cost;

    for (int i = 0; i < c_count; i++)
    {
        printf("--------------------------\n");
        printf("Customer Name: %s\n", c_array[i].name);
        printf("Tickets Bought: \n");
        for (int j = 0; j < c_array[i].tickets_bought; j++)
        {
            printf("- %d | %s at %s | Bought for P%.2f\n",
                   c_array[i].tickets[j].event_id,
                   c_array[i].tickets[j].event_title,
                   c_array[i].tickets[j].date_and_time,
                   c_array[i].tickets[j].ticket_price);
        }
        printf("Total Cost: %.2f\n", c_array[i].total_cost);
    }
    printf("--------------------------\n\n");
}

void save_customers(char *c_file, Customer *c_array, int c_count)
{
    FILE *fp = fopen(c_file, "w");

    // Write the count
    fprintf(fp, "%d\n", c_count);

    if (c_count)
    {
        printf("No customers were saved.\n");
        return;
    }

    for (int i = 0; i < c_count; i++)
    {
        fprintf(fp, "%s\n", c_array[i].name);
        fprintf(fp, "%d\n", c_array[i].tickets_bought);
        for (int j = 0; j < c_array[i].tickets_bought; j++)
        {
            fprintf(fp, "%d\t%s\t%s\t%.2f\n",
                    c_array[i].tickets[j].event_id,
                    c_array[i].tickets[j].event_title,
                    c_array[i].tickets[j].date_and_time,
                    c_array[i].tickets[j].ticket_price);
        }
        fprintf(fp, "%.2f\n", c_array[i].total_cost);
    }
    fclose(fp);
}

void load_customers(char *c_file, Customer *c_array, int *c_count)
{
    // Variable
    char temp[STR_MAX_LENGTH];

    FILE *fp = fopen(c_file, "r");
    if (fp == NULL)
    {
        printf("File customers.txt not found!\nCreating customers.txt file...\n\n");
        fp = fopen(c_file, "w");
        fclose(fp);
        return; // since there is nothing to read
    }

    // Read the count
    fscanf(fp, "%d\n", c_count);

    // If *c_count is 0, there is nothing to read.
    if (*c_count == 0)
    {
        return;
    }

    for (int i = 0; i < *c_count; i++)
    {
        for (int status = 0; status < 4; status++)
        {
            fgets(temp, STR_MAX_LENGTH, fp);
            switch (status)
            {
            case 0: // NAME
                if (temp[strlen(temp) - 1] == '\n')
                {
                    temp[strlen(temp) - 1] = '\0';
                }
                strcpy(c_array[i].name, temp);
                break;
            case 1: // TICKET COUNT
                sscanf(temp, "%d", &c_array[i].tickets_bought);
                break;
            case 2: // TICKETS
                for (int j = 0; j < c_array[i].tickets_bought; j++)
                {
                    if (j != 0)
                    {
                        fgets(temp, STR_MAX_LENGTH, fp); // ensure we actually read a new line
                    }
                    sscanf(temp, "%d\t%[^\t\n]\t%[^\t\n]\t%f", // [^\t\n] is a scanset, stopping when encountering a newline or tab
                           &c_array[i].tickets[j].event_id,
                           c_array[i].tickets[j].event_title,
                           c_array[i].tickets[j].date_and_time,
                           &c_array[i].tickets[j].ticket_price);
                }
                break;
            case 3: // TOTAL COST
                sscanf(temp, "%f", &c_array[i].total_cost);
                break;
            default:
                break;
            }
        }
    }
    fclose(fp);
}

void delete_empty_customers(Customer *c_array, int *c_count)
{
    // Essentially removes Customers from the array if
    // they do not have any tickets bought
    for (int i = 0; i < *c_count;)
    {
        if (c_array[i].tickets_bought == 0)
        {
            for (int j = i; j < *c_count - 1; j++)
            {
                c_array[j] = c_array[j + 1];
            }
            (*c_count)--;
        }
        else
        {
            i++;
        }
    }
}

int events_in_stock(Event *e_array, int e_count)
{
    for (int i = 0; i < e_count; i++)
    {
        if (e_array[i].stock != 0)
        {
            return 1; // returns true if there is at least one event with stock
        }
    }
    return 0;
}

void buy_ticket(Event *e_array, int e_count, Customer *c_array, int *c_count)
{
    // Variables
    char *temp;
    int c_index, e_index, event_id;

    // If no event is in stock, there is nothing to buy.
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
                printf("Error: Reached maximum number of customers.\n\n");
                return;
            }
            add_customer(c_array, c_count, temp);
        }
    } while (c_index == -1);

    if (c_array[c_index].tickets_bought == MAX_TICKETS_PER_CUSTOMER)
    {
        printf("Error: Customer has bought the maximum number of tickets.\n\n");
        free(temp);
        return;
    }

    // Print events
    view_events_l(e_array, e_count);

    // Event ID
    do
    {
        printf("Event ID: ");
        scanf("%d", &event_id);
        clear_buffer();
        if (event_id <= 0)
        {
            printf("Error: Invalid Event ID.\n");
        }
        else if ((e_index = event_exists(e_array, e_count, event_id)) == -1)
        {
            printf("Error: Event with ID %d does not exist.\n\n", event_id);
            free(temp);
            return;
        }
        else if (e_array[e_index].stock == 0)
        {
            printf("Error: Tickets are out of stock.\n\n");
            free(temp);
            return;
        }
    } while (event_id <= 0 || e_index == -1);

    // Copy the Event details to the Customer's Tickets array and decrease Event stock
    c_array[c_index].tickets[c_array[c_index].tickets_bought].event_id = e_array[e_index].event_id;
    strcpy(c_array[c_index].tickets[c_array[c_index].tickets_bought].event_title, e_array[e_index].event_title);
    strcpy(c_array[c_index].tickets[c_array[c_index].tickets_bought].date_and_time, e_array[e_index].date_and_time);
    c_array[c_index].tickets[c_array[c_index].tickets_bought].ticket_price = e_array[e_index].ticket_price;
    c_array[c_index].total_cost += e_array[e_index].ticket_price;
    c_array[c_index].tickets_bought++;
    e_array[e_index].stock--;

    // Print message
    printf("Success: Ticket %d %s %s bought for P%.2f!\n\n",
           e_array[e_index].event_id,
           e_array[e_index].event_title,
           e_array[e_index].date_and_time,
           e_array[e_index].ticket_price);

    free(temp);
}