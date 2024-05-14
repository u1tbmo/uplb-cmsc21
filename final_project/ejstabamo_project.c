#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global Constants

#define FLIGHT_ID_LEN 6
#define PASSPORT_NUM_LEN 9
#define MONTH_STR_LEN 10
#define INITIAL_BUFFER_LEN 16
#define MIN_PASSENGERS 1
#define FLIGHTS_FILE "flights.txt"
#define PASSENGERS_FILE "passengers.txt"

const char *MONTHS[12] =
    {"January", "February", "March", "April",
     "May", "June", "July", "August",
     "September", "October", "November", "December"};

// Structures

typedef struct Date // A structure for a Date with a day, month, and year.
{
    int day;
    char month[MONTH_STR_LEN];
    int year;
} Date;

typedef struct Time // A structure for a Time with the hours and minutes components.
{
    int hours;
    int minutes;
} Time;

typedef struct DateTime // A structure for a DateTime with a Date and Time.
{
    struct Date date;
    struct Time time;
} DateTime;

typedef struct Flight // A structure for a Flight with Flight details.
{
    char *flight_id;
    char *destination;
    char *origin;
    struct DateTime departure;
    struct DateTime arrival;
    int passenger_qty;
    int passenger_max;
    int bonus_miles;
    struct Flight *next;
} Flight;

typedef struct Reservation // A structure for a Reservation with a Flight.
{
    struct Flight *flight;
    struct Reservation *next;
} Reservation;

typedef struct Passenger // A structure for a Passenger with passenger details.
{
    char *first_name;
    char *last_name;
    struct Date birthdate;
    char *passport_number;
    int miles;
    int reservation_qty;
    struct Reservation *reservations;
    struct Passenger *next;
} Passenger;

// General Helper Functions

int main_menu();       // Prints a menu and returns an integer
bool confirm_delete(); // Asks the user if they are sure of deleting data
void clean_exit();     // Cleanly exits the program when there is no more memory left

// Comparison Helper Functions

int flight_compare(Flight *a, Flight *b);          // Compares two flights
int passenger_compare(Passenger *a, Passenger *b); // Compares two passengers

// Valid/Validation Helper Functions

bool valid_day(int day, char *month, int year); // Checks if a day is valid
bool valid_month(char *input);                  // Checks if a month is valid
bool valid_string(char *string);                // Checks if a string is valid
bool validate_id(char *id);                     // Checks if an ID is valid, normalizes string to uppercase
bool validate_passport(char *passport_numnber); // Checks if a passport is valid, normalizes string to uppercase

// DateTime Helper Functions

int month_to_int(char *month);              // Converts a month to its number
long long datetime_to_minutes(DateTime dt); // Converts a DateTime to minutes

// DateTime Check Helper Functions

bool is_conflicting(DateTime departure1, DateTime arrival1, DateTime departure2, DateTime arrival2); // Checks if a DateTime is in the bounds of another DateTime
bool is_future(DateTime old, DateTime new);                                                          // Checks if a date is in the future (from another date)
bool is_leap(int year);                                                                              // Checks if a year is a leap year

// Input Functions

char *get_string(FILE *stream);                              // Gets a string from a file stream (with automatic dynamic memory allocation)
int get_int(char *prompt);                                   // Gets an integer from the user
Date get_date();                                             // Gets a date from the user
DateTime get_datetime(bool is_arrival, DateTime *departure); // Gets DateTime from the user

// Memory Functions

void free_flight_node(Flight *node);       // Frees memory allocated for a Flight node
void free_passenger_node(Passenger *node); // Frees memory allocated for a Passenger node
void free_flights(Flight *head);           // Frees all memory allocated for the flights linked list
void free_passengers(Passenger *head);     // Frees all memory allocated for the passengers linked list
void free_reservations(Reservation *head); // Frees all memory allocated for the reservations linked list

// Flight Linked List Functions

Flight *create_flight_node();                              // Creates a Flight node
Flight *search_flight_node(Flight *head, char *flight_id); // Searches for a Flight in the linked list and returns a pointer to it
void insert_flight_node(Flight **head, Flight *node);      // Insert a Flight to the linked list considering order
void delete_flight_node(Flight **head, char *flight_id);   // Deletes a Flight from the linked list
int count_flights(Flight *head);                           // Counts the number of Flights in the linked list

// Passenger Linked List Functions

Passenger *create_passenger_node();                                       // Creates a Passenger Node
Passenger *search_passenger_node(Passenger *head, char *passport_number); // Searches for a Passenger in the linked list and returns a pointer to it
void insert_passenger_node(Passenger **head, Passenger *node);            // Insert a Passenger to the linked list considering order
int count_passengers(Passenger *head);                                    // Counts the number of Passengers in the linked list

// Reservation Linked List Functions

Reservation *create_reservation_node(Flight *flight);                    // Creates a Reservation Node
Reservation *search_reservation_node(Reservation *head, Flight *flight); // Searches for a Reservation in the linked list and returns a pointer to it
void insert_reservation_node(Reservation **head, Reservation *node);     // Insert a Reservation to the linked list considering order
void delete_reservation_node(Reservation **head, Flight *flight);        // Deletes a Reservation from the linked list
int count_reservations(Reservation *head);                               // Counts the number of Reservations in the linked list

// Program Functionality

void add_flight(Flight **head);                             // Adds a Flight to the database
void edit_flight(Flight *head);                             // Edits a Flight in the database
int view_flights_menu();                                    // Prints the menu for viewing flights and returns an integer
void view_flights(Flight *head, int mode);                  // Views flights in the database
bool view_flights_linear(Flight *head, int mode);           // Views flights in the database (linear format), returns false if no flights are empty for mode 2
void view_passengers_linear(Passenger *head);               // Views passengers in the database (linear format)
void view_reservations_linear(Reservation *head);           // Views reservations for a passenger (linear format)
void delete_flight(Flight **head);                          // Deletes a Flight from the database
void add_passenger(Passenger **head);                       // Adds a passenger to the database
void edit_passenger(Passenger *head);                       // Edits a passenger in the database
void book_reservation(Flight *f_head, Passenger *p_head);   // Books a Flight reservation for a passenger
void remove_reservation(Flight *f_head, Passenger *p_head); // Removes a Flight reservation from a passenger
void view_reservations(Passenger *p_head);                  // Views all reservations of a passenger

// Save and Load Functions
void load(Flight **f_head, Passenger **p_head); // Load flights and passengers
void save(Flight *f_head, Passenger *p_head);   // Save flights and passengers

int main()
{
    // Variables
    int choice, view_choice;

    // Linked Lists
    Flight *flights = NULL;
    Passenger *passengers = NULL;

    // Load Flights and Passengers from files
    load(&flights, &passengers);

    // Main Program Loop
    do
    {
        choice = main_menu(); // Ask the user for a choice from the menu
        printf("\n");
        switch (choice)
        {
        case 1: // Add Flight
            add_flight(&flights);
            break;
        case 2: // Edit Flight
            if (flights == NULL)
            {
                printf("[Error] No flights.\n\n");
                break;
            }
            edit_flight(flights);
            break;
        case 3: // View Flights
            if (flights == NULL)
            {
                printf("[Error] No flights.\n\n");
                break;
            }
            do
            {
                view_choice = view_flights_menu();
                printf("\n");
                switch (view_choice)
                {
                case 1:
                case 2:
                case 3:
                case 4:
                    view_flights(flights, view_choice);
                    break;
                case 0:
                    break;
                default:
                    printf("[Error] Invalid choice.\n\n");
                    break;
                }
            } while (view_choice != 0);
            break;
        case 4: // Delete Flight
            if (flights == NULL)
            {
                printf("[Error] No flights.\n\n");
                break;
            }
            delete_flight(&flights);
            break;
        case 5: // Add Passenger
            add_passenger(&passengers);
            break;
        case 6:
            if (passengers == NULL)
            {
                printf("[Error] No passengers.\n\n");
                break;
            }
            edit_passenger(passengers);
            break;
        case 7: // Book Reservation
            if (flights == NULL && passengers == NULL)
            {
                printf("[Error] No flights and passengers.\n\n");
                break;
            }
            if (flights == NULL)
            {
                printf("[Error] No flights.\n\n");
                break;
            }
            if (passengers == NULL)
            {
                printf("[Error] No passengers.\n\n");
                break;
            }
            book_reservation(flights, passengers);
            break;
        case 8: // Remove Reservation
            if (passengers == NULL)
            {
                printf("[Error] No passengers.\n\n");
                break;
            }
            remove_reservation(flights, passengers);
            break;
        case 9: // View Reservation
            if (passengers == NULL)
            {
                printf("[Error] No passengers.\n\n");
                break;
            }
            view_reservations(passengers);
            break;
        case 0:
            printf("Goodbye!\n\n");
            break;

        default:
            break;
        }

        // Save Flights and Passengers to Files
        save(flights, passengers);
    } while (choice != 0);

    // Free allocated memory
    free_flights(flights);
    free_passengers(passengers);
}

int main_menu()
{
    int choice;

    printf("================== Menu ===================\n");
    printf("1 | Add Flight\n");
    printf("2 | Edit Flight\n");
    printf("3 | View Flights\n");
    printf("4 | Delete Flight\n");
    printf("5 | Add Passenger\n");
    printf("6 | Edit Passenger Details\n");
    printf("7 | Book Flight Reservation\n");
    printf("8 | Remove Flight Reservation\n");
    printf("9 | View Reservations\n");
    printf("0 | Exit\n\n");
    choice = get_int("Enter choice: ");

    return choice;
}

bool confirm_delete()
{
    // Variable to hold user's input
    char *choice;

    printf("Enter [Y] to confirm deletion: ");
    choice = get_string(stdin);

    // Check if the user only entered "Y"
    if (strcmp(choice, "Y") == 0)
    {
        free(choice);
        return true; // Return true if input matches "Y"
    }
    else
    {
        printf("[Info] '%s' does not match 'Y'. Cancelling.\n\n", choice);
        free(choice);
        return false; // Return false otherwise
    }
}

void clean_exit()
{
    // Free allocated memory?
    // Still thinking of a way to do this
    // On modern OSes, exit() allows the OS to reclaim memory from the process
    // It is still good practice to free allocated memory

    // Exit with failure status
    exit(EXIT_FAILURE);
}

int flight_compare(Flight *a, Flight *b)
{
    // Convert DateTime to minutes
    long long a_departure_in_minutes = datetime_to_minutes(a->departure);
    long long b_departure_in_minutes = datetime_to_minutes(b->departure);

    // Sort by Departure
    if (a_departure_in_minutes < b_departure_in_minutes)
    {
        return -1;
    }
    else if (a_departure_in_minutes > b_departure_in_minutes)
    {
        return 1;
    }

    // Sort by Origin
    int origin_cmp = strcmp(a->origin, b->origin);
    if (origin_cmp != 0)
    {
        return origin_cmp; // Return the result of strcmp()
    }

    // Sort by Destination
    int destination_cmp = strcmp(a->destination, b->destination);
    if (destination_cmp != 0)
    {
        return destination_cmp; // Return the result of strcmp()
    }

    // If all else fails, the flights are equal
    return 0;
}

int passenger_compare(Passenger *a, Passenger *b)
{
    // Sort by First Names
    int first_name_cmp = strcmp(a->first_name, b->first_name);
    if (first_name_cmp != 0)
    {
        return first_name_cmp; // Return the result of strcmp()
    }

    // Sort by Last Names
    int last_name_cmp = strcmp(a->last_name, b->last_name);
    if (last_name_cmp != 0)
    {
        return last_name_cmp; // Return the result of strcmp()
    }

    // If all else fails, the passengers are equal
    return 0;
}

bool valid_day(int day, char *month, int year)
{
    // Check if the month is February
    if (strcmp(month, "February") == 0)
    {
        // If the year is a leap year
        if (is_leap(year))
        {
            return day >= 1 && day <= 29; // Check if the day is in the range [1,29]
        }
        else
        {
            return day >= 1 && day <= 28; // Check if the day is in the range [1,28]
        }
    }
    // Check if the month is April, June, September, or November
    else if (strcmp(month, "April") == 0 || strcmp(month, "June") == 0 ||
             strcmp(month, "September") == 0 || strcmp(month, "November") == 0)
    {
        return day >= 1 && day <= 30; // Check if the day is in the range [1,30]
    }
    // Check if the month is a 31-day month
    else
    {
        return day >= 1 && day <= 31; // Check if the day is in the range [1,31]
    }
}

bool valid_month(char *input)
{
    // Iterate over the MONTHS array
    for (int i = 0; i < 12; i++)
    {
        // Check if the input matches any month in the array
        if (strcmp(input, MONTHS[i]) == 0)
        {
            return true; // Return true if input is in the MONTHS array
        }
    }
    return false; // Return false otherwise
}

bool valid_string(char *string)
{
    if (strlen(string) == 0)
    {
        return false; // Return false if the string is empty
    }
    return true; // Return true otherwise
}

bool validate_id(char *id)
{
    // Check if the id is empty
    if (strlen(id) == 0)
    {
        return false;
    }
    // Check if the id exceeds the maximum length FLIGHT_ID_LEN
    if (strlen(id) > FLIGHT_ID_LEN)
    {
        return false;
    }
    // Iterate over the string
    for (int i = 0; id[i] != '\0'; i++)
    {
        // Check for non alphanumeric letters
        if (!isalnum(id[i]))
        {
            return false;
        }
        // Check for lowercase letters, turn them uppercase
        else if (islower(id[i]))
        {
            id[i] = toupper(id[i]);
        }
    }
    return true;
}

bool validate_passport(char *passport_number)
{
    // Check if the length of the passport_number is 9
    if (strlen(passport_number) != PASSPORT_NUM_LEN)
    {
        return false;
    }
    // Iterate over the string
    for (int i = 0; passport_number[i] != '\0'; i++)
    {
        // Check for non alphanumeric letters
        if (!isalnum(passport_number[i]))
        {
            return false;
        }
        // Check for lowercase letters, turn them uppercase
        else if (islower(passport_number[i]))
        {
            passport_number[i] = toupper(passport_number[i]);
        }
    }
    return true;
}

int month_to_int(char *month)
{
    // Iterate over the MONTHS array
    for (int i = 0; i < 12; i++)
    {
        // Check if the input matches any month in the array
        if (strcmp(month, MONTHS[i]) == 0)
        {
            return i + 1; // Returns the month number of a month string
        }
    }
    return -1;
}

long long datetime_to_minutes(DateTime dt)
{
    // Convert each DateTime component to minutes, add, then return the result
    return dt.date.year * 525600 +
           month_to_int(dt.date.month) * 43800 +
           dt.date.day * 1440 +
           dt.time.hours * 60 +
           dt.time.minutes;
}

bool is_conflicting(DateTime departure1, DateTime arrival1, DateTime departure2, DateTime arrival2)
{
    // Convert DateTime to minutes
    long long departure1_in_minutes = datetime_to_minutes(departure1);
    long long arrival1_in_minutes = datetime_to_minutes(arrival1);
    long long departure2_in_minutes = datetime_to_minutes(departure2);
    long long arrival2_in_minutes = datetime_to_minutes(arrival2);

    // Check if the two time periods overlap
    if ((departure1_in_minutes <= arrival2_in_minutes) && (arrival1_in_minutes >= departure2_in_minutes))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_future(DateTime old, DateTime new)
{
    // Convert DateTime to minutes since some point, for easy comparison
    long long old_in_minutes = datetime_to_minutes(old);
    long long new_in_minutes = datetime_to_minutes(new);

    // Check if the new DateTime is in the future compared to the old DateTime
    return new_in_minutes > old_in_minutes;
}

bool is_leap(int year)
{
    if (year % 4 != 0) // A leap year must be disivible by 4...
    {
        return false;
    }
    if (year % 100 != 0) // ...but must not be divisible by 100...
    {
        return true;
    }
    if (year % 400 == 0) // ...unless it is divisble by 400
    {
        return true;
    }
    return false;
}

char *get_string(FILE *stream)
{
    // Variables
    int buffer_length = INITIAL_BUFFER_LEN, c, i = 0;
    char *temp;

    // Allocate memory for a string
    char *buffer = (char *)malloc(sizeof(char) * buffer_length);
    if (buffer == NULL) // If malloc failed
    {
        printf("[Error] Memory allocation failed.\n\n");
        clean_exit();
    }

    while ((c = fgetc(stream)) != '\n' && c != EOF) // Read characters from the file stream one by one
    {
        // Fill the buffer with the characters from the file stream
        buffer[i++] = c;

        // If we've reached the end of the buffer, reallocate more memory
        if (i == buffer_length)
        {
            buffer_length *= 2;                                             // Increase size of buffer
            buffer = (char *)realloc(buffer, sizeof(char) * buffer_length); // Reallocate memory
            if (buffer == NULL)                                             // If realloc failed.
            {
                printf("[Error] Memory allocation failed.\n\n");
                clean_exit();
            }
        }
    }

    // Null-terminate the string
    buffer[i] = '\0';

    // Shrink the buffer to the actual size
    temp = (char *)realloc(buffer, sizeof(char) * (i + 1));
    if (temp == NULL) // If realloc fails
    {
        printf("[Error] Memory allocation failed.\n\n");
        free(buffer);
        clean_exit();
    }
    buffer = temp; // The buffer is resized to the optimal size

    return buffer; // Return the string
}

int get_int(char *prompt)
{
    int number;          // Holds the number that is converted from the string
    char *input = NULL;  // Holds the string from the user
    char *endptr = NULL; // Stores the endptr from strtol

    // Repeatedly ask for an integer
    do
    {
        // Free previous input from the previous loop
        if (input != NULL)
        {
            free(input);
        }
        // Ask the user for a number in string form
        printf("%s", prompt);
        input = get_string(stdin);

        // Convert the string to an integer
        number = strtol(input, &endptr, 0);

        // Check for invalid invalid input
        if (*endptr != '\0' || input[0] == '\n' || endptr == input)
        {
            printf("[Error] Please enter a valid positive integer.\n");
        }
    } while (*endptr != '\0' || input[0] == '\n' || endptr == input);
    free(input); // Free the string

    return number; // Return the integer
}

Date get_date()
{
    Date new_date;                                     // Holds the new Date
    char *input, *endptr;                              // String input, endptr for strtol
    bool month_is_valid = false, day_is_valid = false; // Bools for validating inputs

    // Ask for a year
    do
    {
        new_date.year = get_int("Year   : ");
        if (new_date.year < 1909) // First recorded use of an airport :)
        {
            printf("[Error] Invalid year. Please type a valid year.\n");
        }
    } while (new_date.year < 1909);

    // Ask for a month
    do
    {
        printf("Month  : ");
        input = get_string(stdin);
        month_is_valid = valid_month(input);
        if (!month_is_valid)
        {
            printf("[Error] Invalid month. Please type a valid month name.\n");
        }
        else
        {
            strncpy(new_date.month, input, MONTH_STR_LEN);
        }

        free(input);
    } while (!month_is_valid);

    // Ask for a day
    do
    {
        new_date.day = get_int("Day    : ");
        day_is_valid = valid_day(new_date.day, new_date.month, new_date.year);
        if (!day_is_valid)
        {
            printf("[Error] Invalid day. Please type a valid day.\n");
        }
    } while (!day_is_valid);

    return new_date;
}

DateTime get_datetime(bool is_arrival, DateTime *departure)
{
    Date new_date;                                         // Holds the new Date
    Time new_time;                                         // Holds the new Time
    DateTime new_datetime;                                 // Holds the new DateTime
    char *input, *endptr;                                  // String Input, endptr for strtol
    bool month_is_valid = false, arrival_is_future = true; // Bools for validating inputs

    do
    {
        // Ask for a Date (Year, Month, Day)
        new_date = get_date();

        // Ask for hours
        do
        {
            new_time.hours = get_int("Hour   : ");
            if (new_time.hours < 0 || new_time.hours > 23)
            {
                printf("[Error] Hour is not in range (0-23).\n");
            }
        } while (new_time.hours < 0 || new_time.hours > 23);

        // Ask for minutes
        do
        {
            new_time.minutes = get_int("Minutes: ");
            if (new_time.minutes < 0 || new_time.minutes > 59)
            {
                printf("[Error] Minutes are not in range (0-59).\n");
            }

        } while (new_time.minutes < 0 || new_time.minutes > 59);

        // Set the DateTime components using the acquired Date and Time
        new_datetime = (DateTime){.date = new_date, .time = new_time}; // Compound Literal

        // If this input is for an arrival DateTime, make sure it is to the future of the departure
        if (is_arrival)
        {
            arrival_is_future = is_future(*departure, new_datetime);
            if (!arrival_is_future)
            {
                printf("[Error] Your arrival must be after the departure.\n");
            }
        }
    } while (!arrival_is_future);

    return new_datetime; // Return the new DateTime
}

void free_flight_node(Flight *node)
{
    // Only free the node if it is NOT NULL
    if (node != NULL)
    {
        // Free strings in the node
        if (node->flight_id != NULL)
        {
            free(node->flight_id);
        }
        if (node->destination != NULL)
        {
            free(node->destination);
        }
        if (node->origin != NULL)
        {
            free(node->origin);
        }

        // Free the node
        free(node);
    }
}

void free_passenger_node(Passenger *node)
{
    // Only free the node if it is NOT NULL
    if (node != NULL)
    {
        // Free strings in the node
        if (node->first_name != NULL)
        {
            free(node->first_name);
        }
        if (node->last_name != NULL)
        {
            free(node->last_name);
        }
        if (node->passport_number != NULL)
        {
            free(node->passport_number);
        }

        // Free reservations
        free_reservations(node->reservations);

        // Free the node
        free(node);
    }
}

void free_flights(Flight *head)
{
    // Free every node in the linked list
    while (head != NULL)
    {
        Flight *temp = head;
        head = head->next;
        free_flight_node(temp);
    }
}

void free_passengers(Passenger *head)
{
    // Free every node in the linked list
    while (head != NULL)
    {
        Passenger *temp = head;
        head = head->next;
        free_passenger_node(temp);
    }
}

void free_reservations(Reservation *head)
{
    // Free every node in the linked list
    while (head != NULL)
    {
        Reservation *temp = head;
        head = head->next;
        free(temp);
    }
}

Flight *create_flight_node()
{
    // Allocate memory for a new Flight node
    Flight *new_flight = (Flight *)malloc(sizeof(Flight));
    if (new_flight == NULL) // If malloc failed
    {
        printf("[Error] Memory allocation failed.\n");
        clean_exit();
    }

    // Set fields to defaults
    *new_flight = (Flight){
        .flight_id = NULL,
        .destination = NULL,
        .origin = NULL,
        .departure.date = (Date){.day = 0, .month = "", .year = 0},
        .arrival.date = (Date){.day = 0, .month = "", .year = 0},
        .departure.time = (Time){.hours = 0, .minutes = 0},
        .arrival.time = (Time){.hours = 0, .minutes = 0},
        .passenger_qty = 0,
        .bonus_miles = 0,
        .next = NULL,
    }; // Compound Literal

    return new_flight;
}

Flight *search_flight_node(Flight *head, char *flight_id)
{
    // Start from the head
    Flight *curr = head;

    // Traverse the linked list
    while (curr != NULL)
    {
        if (strcmp(curr->flight_id, flight_id) == 0)
        {
            return curr; // return the pointer to the node if found
        }
        curr = curr->next;
    }
    return NULL; // return NULL if not found
}

void insert_flight_node(Flight **head, Flight *node)
{
    // Start from the head
    Flight *curr = *head;

    // If the list is empty OR the new node is before the head in order
    if (*head == NULL || flight_compare(node, *head) < 0)
    {
        node->next = *head;
        *head = node;
    }
    // Traverse the list until we find the correct position
    else
    {
        while (curr->next != NULL && flight_compare(node, curr->next) >= 0)
        {
            curr = curr->next;
        }
        node->next = curr->next;
        curr->next = node;
    }
}

void delete_flight_node(Flight **head, char *flight_id)
{
    // Start from the head
    Flight *prev = *head;
    Flight *curr = *head;

    // Stores the node that will be deleted
    Flight *temp;

    // If the Flight does not exist
    if (search_flight_node(*head, flight_id) == NULL)
    {
        printf("[Error] Flight does not exist.\n\n");
        return;
    }

    // If the node to delete is the head
    if (strcmp((*head)->flight_id, flight_id) == 0)
    {
        temp = *head;
        *head = (*head)->next;

        free_flight_node(temp);
        return;
    }

    // Find the node to delete
    while (strcmp(curr->flight_id, flight_id) != 0)
    {
        prev = curr;
        curr = curr->next;
    }
    prev->next = curr->next; // Remove the node from the list
    free_flight_node(curr);  // Free the deleted node
}

int count_flights(Flight *head)
{
    int count = 0;

    // Start from the head
    Flight *ptr = head;

    // Traverse the linked list, incrementing per node
    while (ptr != NULL)
    {
        count++;
        ptr = ptr->next;
    }

    return count; // Return the count
}

Passenger *create_passenger_node()
{
    // Allocate memory for a Passenger node
    Passenger *new_passenger = (Passenger *)malloc(sizeof(Passenger));
    if (new_passenger == NULL) // If malloc failed
    {
        printf("[Error] Memory allocation failed.\n\n");
        clean_exit();
    }

    // Initialize fields to defaults
    *new_passenger = (Passenger){
        .first_name = NULL,
        .last_name = NULL,
        .birthdate = (Date){.day = 0, .month = "", .year = 0},
        .passport_number = NULL,
        .reservation_qty = 0,
        .miles = 0}; // Compound Literal

    return new_passenger; // Return the Passenger
}

Passenger *search_passenger_node(Passenger *head, char *passport_number)
{
    // Start from the head
    Passenger *curr = head;

    // Traverse the linked list
    while (curr != NULL)
    {
        if (strcmp(curr->passport_number, passport_number) == 0)
        {
            return curr; // return the pointer to the node
        }
        curr = curr->next;
    }

    return NULL; // return NULL if node does not exist
}

void insert_passenger_node(Passenger **head, Passenger *node)
{
    // Start from the head
    Passenger *curr = *head;

    // If the list is empty OR the new node is before the head in order
    if (*head == NULL || passenger_compare(node, *head) < 0)
    {
        node->next = *head;
        *head = node;
    }
    // Traverse the list until we find the correct position
    else
    {
        while (curr->next != NULL && passenger_compare(node, curr->next) >= 0)
        {
            curr = curr->next;
        }
        node->next = curr->next;
        curr->next = node;
    }
}

int count_passengers(Passenger *head)
{
    int count = 0;

    // Start from the head
    Passenger *ptr = head;

    // Traverse the linked list, incrementing per node
    while (ptr != NULL)
    {
        count++;
        ptr = ptr->next;
    }

    return count; // Return the count
}

Reservation *create_reservation_node(Flight *flight)
{
    // Allocate memory for a Reservation node
    Reservation *new_reservation = (Reservation *)malloc(sizeof(Reservation));
    if (new_reservation == NULL) // If malloc failed
    {
        printf("[Error] Memory allocation failed.\n\n");
        clean_exit();
    }

    // Initialize field with the Flight
    *new_reservation = (Reservation){
        .flight = flight, .next = NULL}; // Compound Literal

    return new_reservation; // Return the Reservation
}

Reservation *search_reservation_node(Reservation *head, Flight *flight)
{
    // Start from the head
    Reservation *curr = head;

    // Traverse the linked list
    while (curr != NULL)
    {
        if (strcmp(curr->flight->flight_id, flight->flight_id) == 0)
        {
            return curr; // return the pointer to the node
        }
        curr = curr->next;
    }
    return NULL; // return NULL if node does not exist
}

void insert_reservation_node(Reservation **head, Reservation *node)
{
    // Start from the head
    Reservation *curr = *head;

    // If the list is empty OR the new node is before the *head in order
    if (*head == NULL || flight_compare(node->flight, (*head)->flight) < 0)
    {
        node->next = *head;
        *head = node;
    }
    // Traverse the list until we find the correct position
    else
    {
        while (curr->next != NULL && flight_compare(node->flight, (*head)->flight) >= 0)
        {
            curr = curr->next;
        }
        node->next = curr->next;
        curr->next = node;
    }
}

void delete_reservation_node(Reservation **head, Flight *flight)
{
    // Start from the head
    Reservation *prev = *head;
    Reservation *curr = *head;

    // Stores the node that will be deleted
    Reservation *temp;

    if (search_reservation_node(*head, flight) == NULL) // If the reservation does not exist
    {
        printf("[Error] Reservation does not exist.\n\n");
        return;
    }

    if (strcmp((*head)->flight->flight_id, flight->flight_id) == 0) // If the node to delete is the head
    {
        temp = *head;
        *head = (*head)->next;

        free(temp);
        return;
    }

    while (strcmp(curr->flight->flight_id, flight->flight_id) != 0) // Find the node to delete
    {
        prev = curr;
        curr = curr->next;
    }
    prev->next = curr->next; // Remove the node from the linked list
    free(curr);              // Free the node
}

int count_reservations(Reservation *head)
{
    int count = 0;

    // Start from the head
    Reservation *ptr = head;

    // Traverse the list, incrementing per node
    while (ptr != NULL)
    {
        count++;
        ptr = ptr->next;
    }

    return count; // Return the count
}

void add_flight(Flight **head)
{
    char *flight_id = NULL;                              // Holds the input flight_id
    bool string_is_valid = false, dest_is_origin = true; // Bools for validation

    printf("== Add Flight =============================\n\n");

    // Ask for Flight ID and validate
    printf("Flight ID  : ");
    flight_id = get_string(stdin);
    if (!validate_id(flight_id))
    {
        printf("[Error] A valid Flight ID has at least 1 and at most 6 uppercase letters and/or digits only.\n\n");
        free(flight_id);
        return;
    }
    if (search_flight_node(*head, flight_id) != NULL)
    {
        printf("[Error] That Flight already exists.\n\n");
        free(flight_id);
        return;
    }

    // Create the Flight node and set its flight_id
    Flight *new_flight = create_flight_node();
    new_flight->flight_id = flight_id;

    // Ask for Origin
    do
    {
        // Free previous input from the previous loop
        if (new_flight->origin != NULL)
        {
            free(new_flight->origin);
        }
        printf("Origin     : ");
        new_flight->origin = get_string(stdin);
        string_is_valid = valid_string(new_flight->origin);
        if (!string_is_valid)
        {
            printf("[Error] Origin cannot be empty.\n");
        }
    } while (!string_is_valid);

    // Ask for Destination
    do
    {
        // Free previous input from the previous loop
        if (new_flight->destination != NULL)
        {
            free(new_flight->destination);
        }
        printf("Destination: ");
        new_flight->destination = get_string(stdin);
        string_is_valid = valid_string(new_flight->destination);
        if (!string_is_valid)
        {
            printf("[Error] Destination cannot be empty.\n");
        }
        if ((dest_is_origin = strcmp(new_flight->destination, new_flight->origin) == 0))
        {
            printf("[Error] Destination cannot be the same as the origin.\n");
        }
    } while (!string_is_valid || dest_is_origin);

    // Ask for Departure DateTime
    printf("\n-- Departure -------------------------\n\n");
    new_flight->departure = get_datetime(false, NULL);

    // Ask for Arrival DateTime
    printf("\n-- Arrival ---------------------------\n\n");
    new_flight->arrival = get_datetime(true, &new_flight->departure);

    // Number of Booked Passengers is already set to 0

    // Ask for Max Count of Passengers (Seats)
    printf("\n--------------------------------------\n");
    do
    {
        new_flight->passenger_max = get_int("Max Seats  : ");
        if (new_flight->passenger_max < MIN_PASSENGERS)
        {
            printf("[Error] The maximum number of passengers must be at least %d.\n", MIN_PASSENGERS);
        }
    } while (new_flight->passenger_max < MIN_PASSENGERS);

    // Ask for Bonus Miles
    do
    {
        new_flight->bonus_miles = get_int("Bonus Miles: ");
        if (new_flight->bonus_miles < 0)
        {
            printf("[Error] The bonus miles must be at least 0.\n");
        }
    } while (new_flight->bonus_miles < 0);

    // Insert the Flight to the linked list
    insert_flight_node(&(*head), new_flight);

    // Print a success message
    printf("\n[Success] Added Flight %s.\n\n", new_flight->flight_id);
}

void edit_flight(Flight *head)
{
    char *flight_id;      // Holds the input flight_id
    Flight *f_ptr = NULL; // A pointer to the Flight to edit

    printf("== Edit Flight ============================\n\n");

    // Print flights (in linear form)
    view_flights_linear(head, 3);

    // Ask for a Flight ID and validate
    printf("Flight ID: ");
    flight_id = get_string(stdin);
    printf("\n");
    if (!validate_id(flight_id))
    {
        printf("[Error] A valid Flight ID has at least 1 and at most 6 uppercase letters and/or digits only.\n\n");
        return;
    }
    if ((f_ptr = search_flight_node(head, flight_id)) == NULL)
    {
        printf("[Error] That Flight does not exist.\n\n");
        return;
    }
    free(flight_id);

    // New Departure DateTime
    printf("-- Departure -------------------------\n\n");
    f_ptr->departure = get_datetime(false, NULL);

    // New Arrival DateTime
    printf("\n-- Arrival ---------------------------\n\n");
    f_ptr->arrival = get_datetime(true, &f_ptr->departure);

    // New Max Passengers (Seats)
    printf("\n--------------------------------------\n\n");
    do
    {
        f_ptr->passenger_max = get_int("Max Seats: ");
        if (f_ptr->passenger_max < MIN_PASSENGERS)
        {
            printf("[Error] The maximum number of passengers must be at least %d.\n", MIN_PASSENGERS);
        }
        else if (f_ptr->passenger_max < f_ptr->passenger_qty)
        {
            printf("[Error] You cannot decrease maximum passengers below number of reserved passengers.\n");
        }
    } while (f_ptr->passenger_max < MIN_PASSENGERS || f_ptr->passenger_max < f_ptr->passenger_qty);

    printf("\n[Success] Edited Flight %s.\n\n", f_ptr->flight_id);
}

int view_flights_menu()
{
    int choice;

    printf("============== View Flights ===============\n");
    printf("1 | View Specific Flight\n");
    printf("2 | View All Available Flights\n");
    printf("3 | View All Fully-booked Flights\n");
    printf("4 | View All Flights\n");
    printf("0 | Back\n\n");
    choice = get_int("Enter choice: ");

    return choice;
}

void view_flights(Flight *head, int mode)
{
    int count = 0;
    Flight *ptr = head; // A pointer to the Flight to view
    char *flight_id;    // Input string

    switch (mode)
    {
    case 1: // Mode 1: View Specific Flight
        printf("-- View Flights > Specific -----------\n\n");

        // Print flights (in linear form)
        view_flights_linear(head, 3);

        // Ask for a Flight ID and Validate
        printf("Flight ID: ");
        flight_id = get_string(stdin);
        printf("\n");
        if (!validate_id(flight_id))
        {
            printf("[Error] A valid Flight ID has at least 1 and at most 6 uppercase letters and/or digits only.\n\n");
            free(flight_id);
            return;
        }
        if ((ptr = search_flight_node(head, flight_id)) == NULL)
        {
            printf("[Error] Flight does not exist.\n\n");
            free(flight_id);
            return;
        }
        free(flight_id);

        printf("Flight ID: %s\n", ptr->flight_id);
        printf("Flight   : %s to %s\n", ptr->origin, ptr->destination);
        printf("- Departure: %d %s %d - %02d:%02d\n",
               ptr->departure.date.day, ptr->departure.date.month, ptr->departure.date.year,
               ptr->departure.time.hours, ptr->departure.time.minutes);
        printf("- Arrival  : %d %s %d - %02d:%02d\n",
               ptr->arrival.date.day, ptr->arrival.date.month, ptr->arrival.date.year,
               ptr->arrival.time.hours, ptr->arrival.time.minutes);
        printf("Passengers : %d\n", ptr->passenger_qty);
        printf("Max Seats  : %d\n", ptr->passenger_max);
        printf("Bonus Miles: %d\n", ptr->bonus_miles);
        printf("\n");

        break;
    case 2: // Mode 2: View Available Flights
        printf("-- View Flights > Available ----------\n\n");
        // Traverse the linked list
        while (ptr != NULL)
        {
            // Print the flight if the flight can still be booked
            if (ptr->passenger_qty < ptr->passenger_max)
            {
                printf("Flight ID: %s\n", ptr->flight_id);
                printf("Flight   : %s to %s\n", ptr->origin, ptr->destination);
                printf("- Departure: %d %s %d - %02d:%02d\n",
                       ptr->departure.date.day, ptr->departure.date.month, ptr->departure.date.year,
                       ptr->departure.time.hours, ptr->departure.time.minutes);
                printf("- Arrival  : %d %s %d - %02d:%02d\n",
                       ptr->arrival.date.day, ptr->arrival.date.month, ptr->arrival.date.year,
                       ptr->arrival.time.hours, ptr->arrival.time.minutes);
                printf("Passengers : %d\n", ptr->passenger_qty);
                printf("Max Seats  : %d\n", ptr->passenger_max);
                printf("Bonus Miles: %d\n", ptr->bonus_miles);
                printf("\n");
                count++;
            }
            ptr = ptr->next;
        }

        // If all flights are booked
        if (count == 0)
        {
            printf("[Info] No flights fit the criteria.\n\n");
        }
        break;
    case 3: // Mode 3: View Full Flights
        printf("-- View Flights > Fully-Booked -------\n\n");
        // Traverse the linked list
        while (ptr != NULL)
        {
            // Print the flight if fully booked
            if (ptr->passenger_qty == ptr->passenger_max)
            {
                printf("Flight ID: %s\n", ptr->flight_id);
                printf("Flight   : %s to %s\n", ptr->origin, ptr->destination);
                printf("- Departure: %d %s %d - %02d:%02d\n",
                       ptr->departure.date.day, ptr->departure.date.month, ptr->departure.date.year,
                       ptr->departure.time.hours, ptr->departure.time.minutes);
                printf("- Arrival  : %d %s %d - %02d:%02d\n",
                       ptr->arrival.date.day, ptr->arrival.date.month, ptr->arrival.date.year,
                       ptr->arrival.time.hours, ptr->arrival.time.minutes);
                printf("Passengers : %d\n", ptr->passenger_qty);
                printf("Max Seats  : %d\n", ptr->passenger_max);
                printf("Bonus Miles: %d\n", ptr->bonus_miles);
                printf("\n");
                count++;
            }
            ptr = ptr->next;
        }

        // If all flights can still be reserved
        if (count == 0)
        {
            printf("[Info] No flights fit the criteria.\n\n");
        }
        break;
    case 4: // Mode 4: View All Flights
        printf("-- View Flights > All ----------------\n\n");
        // Traverse the linked list and print each flight
        while (ptr != NULL)
        {
            printf("Flight ID: %s\n", ptr->flight_id);
            printf("Flight   : %s to %s\n", ptr->origin, ptr->destination);
            printf("- Departure: %d %s %d - %02d:%02d\n",
                   ptr->departure.date.day, ptr->departure.date.month, ptr->departure.date.year,
                   ptr->departure.time.hours, ptr->departure.time.minutes);
            printf("- Arrival  : %d %s %d - %02d:%02d\n",
                   ptr->arrival.date.day, ptr->arrival.date.month, ptr->arrival.date.year,
                   ptr->arrival.time.hours, ptr->arrival.time.minutes);
            printf("Passengers : %d\n", ptr->passenger_qty);
            printf("Max Seats  : %d\n", ptr->passenger_max);
            printf("Bonus Miles: %d\n", ptr->bonus_miles);
            printf("\n");
            ptr = ptr->next;
        }
        break;
    default:
        printf("\n[Error] Invalid choice.\n\n");
        break;
    }
}

bool view_flights_linear(Flight *head, int mode)
{
    int count = 0;
    Flight *ptr = head; // Start from the head

    switch (mode)
    {
    case 1: // Mode 1: Available Flights (for Booking)
        printf("-- Available Flights -----------------\n\n");
        while (ptr != NULL)
        {
            if (ptr->passenger_qty < ptr->passenger_max)
            {
                printf("%6s | %s to %s | %d %s %d %02d:%02d - %d %s %d %02d:%02d\n",
                       ptr->flight_id, ptr->origin, ptr->destination,
                       ptr->departure.date.day, ptr->departure.date.month, ptr->departure.date.year,
                       ptr->departure.time.hours, ptr->departure.time.minutes,
                       ptr->arrival.date.day, ptr->arrival.date.month, ptr->arrival.date.year,
                       ptr->arrival.time.hours, ptr->arrival.time.minutes);
            }
            ptr = ptr->next;
        }
        printf("\n");
        break;
    case 2: // Mode 2: Empty Flights (for Deleting)
        printf("-- Empty Flights ---------------------\n\n");
        while (ptr != NULL)
        {
            if (ptr->passenger_qty == 0)
            {
                printf("%6s | %s to %s | %d %s %d %02d:%02d - %d %s %d %02d:%02d\n",
                       ptr->flight_id, ptr->origin, ptr->destination,
                       ptr->departure.date.day, ptr->departure.date.month, ptr->departure.date.year,
                       ptr->departure.time.hours, ptr->departure.time.minutes,
                       ptr->arrival.date.day, ptr->arrival.date.month, ptr->arrival.date.year,
                       ptr->arrival.time.hours, ptr->arrival.time.minutes);
                count++;
            }
            ptr = ptr->next;
        }
        if (count == 0)
        {
            printf("[Info] No flights are empty.\n\n");
            return false;
        }
        printf("\n");
        break;
    case 3: // Mode 3: All Flights (for Viewing)
        printf("-- All Flights -----------------------\n\n");
        while (ptr != NULL)
        {
            printf("%6s | %s to %s | %d %s %d %02d:%02d - %d %s %d %02d:%02d\n",
                   ptr->flight_id, ptr->origin, ptr->destination,
                   ptr->departure.date.day, ptr->departure.date.month, ptr->departure.date.year,
                   ptr->departure.time.hours, ptr->departure.time.minutes,
                   ptr->arrival.date.day, ptr->arrival.date.month, ptr->arrival.date.year,
                   ptr->arrival.time.hours, ptr->arrival.time.minutes);

            ptr = ptr->next;
        }
        printf("\n");
        break;
    default:
        break;
    }
    return true;
}

void view_passengers_linear(Passenger *head)
{
    // Start from the head
    Passenger *ptr = head;

    printf("-- Passengers ------------------------\n\n");

    // Traverse the linked list and print each Passenger
    while (ptr != NULL)
    {
        printf("%9s | %s, %s | %d flights reserved\n",
               ptr->passport_number, ptr->last_name,
               ptr->first_name, ptr->reservation_qty);
        ptr = ptr->next;
    }
    printf("\n");
}

void view_reservations_linear(Reservation *head)
{
    // Start from the head
    Reservation *ptr = head;
    Flight *f_ptr = NULL; // Pointer to the Reservation's Flight

    printf("-- Reservations ----------------------\n\n");
    // Traverse the linked list and print each Reservation
    while (ptr != NULL)
    {
        f_ptr = ptr->flight;
        printf("%6s | %s to %s | %d %s %d %02d:%02d - %d %s %d %02d:%02d\n",
               f_ptr->flight_id, f_ptr->origin, f_ptr->destination,
               f_ptr->departure.date.day, f_ptr->departure.date.month, f_ptr->departure.date.year,
               f_ptr->departure.time.hours, f_ptr->departure.time.minutes,
               f_ptr->arrival.date.day, f_ptr->arrival.date.month, f_ptr->arrival.date.year,
               f_ptr->arrival.time.hours, f_ptr->arrival.time.minutes);
        ptr = ptr->next;
    }
    printf("\n");
}

void delete_flight(Flight **head)
{
    Flight *ptr; // Pointer to the Flight to delete
    bool empty_flights_exists;

    printf("== Delete Flight ==========================\n\n");

    // Print flights (in linear form)
    empty_flights_exists = view_flights_linear(*head, 2);
    if (!empty_flights_exists)
    {
        return;
    }

    // Ask for a Flight ID and validate
    printf("Flight ID: ");
    char *flight_id = get_string(stdin);
    printf("\n");
    if (!validate_id(flight_id))
    {
        printf("[Error] A valid Flight ID has at least 1 and at most 6 uppercase letters and/or digits only.\n\n");
        return;
    }
    if ((ptr = search_flight_node(*head, flight_id)) == NULL)
    {
        printf("[Error] That Flight does not exist.\n\n");
        return;
    }
    if (ptr->passenger_qty != 0)
    {
        printf("[Error] Passengers have already booked this Flight.\n\n");
        return;
    }
    free(flight_id);

    // Confirm deletion of Flight
    if (confirm_delete())
    {
        printf("[Success] Deleted Flight %s.\n\n", ptr->flight_id);
        delete_flight_node(&(*head), ptr->flight_id);
    }
    else
    {
        printf("[Info] Did not delete Flight.\n\n");
    }
}

void add_passenger(Passenger **head)
{
    printf("== Add Passenger ==========================\n\n");

    char *first_name = NULL;                                 // Input string
    char *last_name = NULL;                                  // Input string
    Date birthdate;                                          // Input string
    char *passport_number = NULL;                            // Input string
    bool passport_is_valid = false, string_is_valid = false; // Bools for validation

    // Ask for Name
    do
    {
        // Free previous input from the previous loop
        if (first_name != NULL)
        {
            free(first_name);
        }
        printf("First Name: ");
        first_name = get_string(stdin);
        string_is_valid = valid_string(first_name);
        if (!string_is_valid)
        {
            printf("[Error] First name cannot be empty.\n");
        }
    } while (!string_is_valid);
    do
    {
        // Free previous input from the previous loop
        if (last_name != NULL)
        {
            free(last_name);
        }
        printf("Last Name : ");
        last_name = get_string(stdin);
        string_is_valid = valid_string(last_name);
        if (!string_is_valid)
        {
            printf("[Error] Last name cannot be empty.\n");
        }
    } while (!string_is_valid);

    // Ask for Birthdate
    printf("\n-- Date of Birth ---------------------\n\n");
    birthdate = get_date();
    printf("\n--------------------------------------\n\n");

    // Ask for Passport Number and validate
    do
    {
        // Free previous input from the previous loop
        if (passport_number != NULL)
        {
            free(passport_number);
        }
        printf("Passport Number: ");
        passport_number = get_string(stdin);
        printf("\n");
        if (!(passport_is_valid = validate_passport(passport_number)))
        {
            printf("[Error] A valid passport number has 9 uppercase letters and/or digits.\n");
        }
        if (search_passenger_node(*head, passport_number) != NULL)
        {
            printf("[Error] Passenger with that passport number already exists.\n\n");
            free(first_name);
            free(last_name);
            free(passport_number);
            return;
        }
    } while (!passport_is_valid);

    // Create Passenger node and initialize fields
    Passenger *new_passenger = create_passenger_node();
    new_passenger->first_name = first_name;
    new_passenger->last_name = last_name;
    new_passenger->birthdate = birthdate;
    new_passenger->passport_number = passport_number;

    // Ask for Number of Miles
    do
    {
        new_passenger->miles = get_int("Number of Miles: ");
        if (new_passenger->miles < 0)
        {
            printf("[Error] The number of miles must be at least 0.\n");
        }
    } while (new_passenger->miles < 0);

    // Insert the Passenger to the linked list
    insert_passenger_node(*(&head), new_passenger);

    // Print success message
    printf("\n[Success] Added Passenger %s.\n\n", new_passenger->first_name);
}

void edit_passenger(Passenger *head)
{
    bool string_is_valid = false; // Bool for validation

    printf("== Edit Passenger =========================\n\n");

    // Print passengers (in linear form)
    view_passengers_linear(head);

    char *passport_number;   // Input string
    Passenger *p_ptr = NULL; // Pointer to the Passenger to edit

    // Ask for a Passport Number and validate
    printf("Passport Number: ");
    passport_number = get_string(stdin);
    printf("\n");
    if (!validate_passport(passport_number))
    {
        printf("[Error] A valid Passport Number has 9 uppercase letters and/or digits.\n\n");
        free(passport_number);
        return;
    }
    if ((p_ptr = search_passenger_node(head, passport_number)) == NULL)
    {
        printf("[Error] That passenger does not exist.\n\n");
        free(passport_number);
        return;
    }
    free(passport_number);

    // New Last Name
    do
    {
        if (p_ptr->last_name != NULL)
        {
            free(p_ptr->last_name);
        }
        printf("Last Name: ");
        p_ptr->last_name = get_string(stdin);
        string_is_valid = valid_string(p_ptr->last_name);
    } while (!string_is_valid);

    // New Birthdate
    printf("\n-- Date of Birth ---------------------\n\n");
    p_ptr->birthdate = get_date();
    printf("\n--------------------------------------\n");

    printf("\n[Success] Edited Passenger %s.\n\n", p_ptr->first_name);
}

void book_reservation(Flight *f_head, Passenger *p_head)
{
    // Variables
    Passenger *passenger = NULL;           // Pointer to the Passenger that will reserve
    Flight *flight = NULL;                 // Pointer to the Flight to be reserved
    Reservation *reservation_ptr = NULL;   // Pointer to a conflicting Reservation
    Reservation *new_reservation = NULL;   // Pointer to the new Reservation Node
    DateTime *d_ptr = NULL, *a_ptr = NULL; // Pointer to a Reservation's DateTimes
    char *passport_number, *flight_id;

    printf("== Book Reservation =======================\n\n");

    // Print passengers (in linear form)
    view_passengers_linear(p_head);

    // Ask for a Passport Number and validate
    printf("Passport Number: ");
    passport_number = get_string(stdin);
    printf("\n");
    if (!validate_passport(passport_number))
    {
        printf("[Error] A valid Passport Number has 9 uppercase letters and/or digits.\n\n");
        return;
    }
    if ((passenger = search_passenger_node(p_head, passport_number)) == NULL)
    {
        printf("[Error] That passenger does not exist.\n\n");
        return;
    }
    free(passport_number);

    // Print flights (in linear form)
    view_flights_linear(f_head, 1);

    // Ask for a Flight ID and validate
    printf("Flight ID: ");
    flight_id = get_string(stdin);
    printf("\n");
    if (!validate_id(flight_id))
    {
        printf("[Error] A valid Flight ID has at least 1 and at most 6 uppercase letters and/or digits only.\n\n");
        free(flight_id);
        return;
    }
    if ((flight = search_flight_node(f_head, flight_id)) == NULL)
    {
        printf("[Error] That Flight does not exist.\n\n");
        free(flight_id);
        return;
    }
    if (flight->passenger_qty == flight->passenger_max)
    {
        printf("[Error] That Flight is fully booked.\n\n");
        free(flight_id);
        return;
    }
    free(flight_id);

    // Start from the head of the Reservations linked list
    reservation_ptr = passenger->reservations;
    // Traverse the linked list
    for (int i = 0; i < passenger->reservation_qty; i++)
    {
        // If we already reserved the flight
        if (reservation_ptr->flight->flight_id == flight->flight_id) // This works because they're the same addresses.
        {
            printf("[Error] You already reserved this Flight.\n\n");
            return;
        }

        // DateTime pointers (to shorten the following code)
        d_ptr = &reservation_ptr->flight->departure;
        a_ptr = &reservation_ptr->flight->arrival;

        // If the Flight conflicts with the Reservation
        if (is_conflicting(*d_ptr, *a_ptr, flight->departure, flight->arrival))
        {

            printf("[Error] That Flight conflicts with current reservations.\n");
            printf("Reserving       : ");
            printf("%6s | %d %s %d %02d:%02d - %d %s %d %02d:%02d\n",
                   flight->flight_id,
                   flight->departure.date.day, flight->departure.date.month, flight->departure.date.year,
                   flight->departure.time.hours, flight->departure.time.minutes,
                   flight->arrival.date.day, flight->arrival.date.month, flight->arrival.date.year,
                   flight->arrival.time.hours, flight->arrival.time.minutes);
            printf("Conflicting with: ");
            printf("%6s | %d %s %d %02d:%02d - %d %s %d %02d:%02d\n\n",
                   reservation_ptr->flight->flight_id,
                   d_ptr->date.day, d_ptr->date.month, d_ptr->date.year,
                   d_ptr->time.hours, d_ptr->time.minutes,
                   a_ptr->date.day, a_ptr->date.month, a_ptr->date.year,
                   a_ptr->time.hours, a_ptr->time.minutes);
            return;
        }
        reservation_ptr = reservation_ptr->next;
    }

    // Create a new Reservation node
    new_reservation = create_reservation_node(flight);

    // Insert the Reservation node to the passenger's reservations linked list
    insert_reservation_node(&passenger->reservations, new_reservation);

    // Update flight and passenger details
    flight->passenger_qty++;
    passenger->reservation_qty++;
    passenger->miles += flight->bonus_miles;

    // Print success message
    printf("[Success] Reserved Flight %s for %s.\n\n", flight->flight_id, passenger->first_name);
}

void remove_reservation(Flight *f_head, Passenger *p_head)
{
    char *passport_number, *flight_id; // Input strings
    Passenger *passenger = NULL;       // Pointer to a Passenger
    Flight *flight = NULL;             // Pointer to a Flight
    Reservation *r_ptr = NULL;         // Pointer to the Reservation to Delete

    printf("== Remove Reservation =====================\n\n");

    // Print passengers (in linear form)
    view_passengers_linear(p_head);

    // Ask for a Passport Number and validate
    printf("Passport Number: ");
    passport_number = get_string(stdin);
    printf("\n");
    if (!validate_passport(passport_number))
    {
        printf("[Error] A valid Passport Number has 9 uppercase letters and/or digits.\n\n");
        free(passport_number);
        return;
    }
    if ((passenger = search_passenger_node(p_head, passport_number)) == NULL)
    {
        printf("[Error] That passenger does not exist.\n\n");
        free(passport_number);
        return;
    }
    if (passenger->reservation_qty == 0)
    {
        printf("[Error] That passenger has no reservations.\n\n");
        free(passport_number);
        return;
    }
    free(passport_number);

    // Print reservations (in linear form)
    view_reservations_linear(passenger->reservations);

    // Ask for a Flight ID and validate
    printf("Flight ID: ");
    flight_id = get_string(stdin);
    printf("\n");
    if (!validate_id(flight_id))
    {
        printf("[Error] A valid Flight ID has at least 1 and at most 6 uppercase letters and/or digits only.\n\n");
        free(flight_id);
        return;
    }
    if ((flight = search_flight_node(f_head, flight_id)) == NULL)
    {
        printf("[Error] That Flight does not exist.\n\n");
        free(flight_id);
        return;
    }
    free(flight_id);

    // Search for the Reservation to delete
    if ((r_ptr = search_reservation_node(passenger->reservations, flight)) == NULL)
    {
        printf("[Error] That reservation does not exist for that passenger.\n\n");
        return;
    }

    // Confirm deletion of Reservation
    if (confirm_delete())
    {
        // Update flight and passenger details
        r_ptr->flight->passenger_qty--;
        passenger->reservation_qty--;
        passenger->miles -= r_ptr->flight->bonus_miles;

        printf("[Success] Deleted reservation %s for %s.\n\n", r_ptr->flight->flight_id, passenger->first_name);
        delete_reservation_node(&passenger->reservations, r_ptr->flight);
    }
    else
    {
        printf("[Info] Did not delete reservation.\n\n");
    }
}

void view_reservations(Passenger *head)
{
    char *passport_number;     // Input string
    Passenger *p_ptr = NULL;   // Pointer to a Passenger
    Flight *f_ptr = NULL;      // Pointer to a Flight
    Reservation *r_ptr = NULL; // Pointer to a Reservation

    printf("== View Reservations ======================\n\n");

    // Print passengers (in linear form)
    view_passengers_linear(head);

    // Ask for a Passport Number and validate
    printf("Passport Number: ");
    passport_number = get_string(stdin);
    printf("\n");
    if (!validate_passport(passport_number))
    {
        printf("[Error] A valid Passport Number has 9 uppercase letters and/or digits.\n\n");
        free(passport_number);
        return;
    }
    if ((p_ptr = search_passenger_node(head, passport_number)) == NULL)
    {
        printf("[Error] That passenger does not exist.\n\n");
        free(passport_number);
        return;
    }
    if (p_ptr->reservation_qty == 0)
    {
        printf("[Error] That passenger has no reservations.\n\n");
        free(passport_number);
        return;
    }
    free(passport_number);

    printf("-- Reservations ----------------------\n\n");

    // Start from the head of the passenger's reservation linked list
    r_ptr = p_ptr->reservations;
    // Traverse and print each reservation
    while (r_ptr != NULL)
    {
        f_ptr = r_ptr->flight;
        printf("Flight ID: %s\n", f_ptr->flight_id);
        printf("Flight:    %s to %s\n", f_ptr->origin, f_ptr->destination);
        printf("- Departure: %d %s %d - %02d:%02d\n",
               f_ptr->departure.date.day, f_ptr->departure.date.month, f_ptr->departure.date.year,
               f_ptr->departure.time.hours, f_ptr->departure.time.minutes);
        printf("- Arrival:   %d %s %d - %02d:%02d\n",
               f_ptr->arrival.date.day, f_ptr->arrival.date.month, f_ptr->arrival.date.year,
               f_ptr->arrival.time.hours, f_ptr->arrival.time.minutes);
        printf("Passengers:  %d\n", f_ptr->passenger_qty);
        printf("Max Seats:   %d\n", f_ptr->passenger_max);
        printf("Bonus Miles: %d\n", f_ptr->bonus_miles);
        printf("\n");
        r_ptr = r_ptr->next;
    }
}

void load(Flight **f_head, Passenger **p_head)
{
    int flight_count = 0, passenger_count = 0;
    Flight *reserved_flight;
    Reservation *r_ptr;

    // Load Flights
    FILE *fp = fopen(FLIGHTS_FILE, "r");
    if (fp == NULL) // If the file does not exist
    {
        printf("== Load ==============================\n\n");
        printf("File 'flights.txt' not found, attempting to create.\n\n");
        fp = fopen(FLIGHTS_FILE, "w"); // Create the file
        if (fp == NULL)                // If fopen failed
        {
            printf("[Error] Critical error. Can't open file.\n\n");
            clean_exit();
        }
        printf("Created 'flights.txt'.\n\n");
        fclose(fp); // Close the file
        return;     // Stop loading
    }

    fscanf(fp, "%d\n", &flight_count); // Read the flight count
    if (flight_count == 0)             // If there are no flights
    {
        return; // Stop loading
    }

    while (!feof(fp)) // While there are flights to read
    {
        // Create a Flight node
        Flight *f_temp = create_flight_node();

        // Scan each field from the file
        f_temp->flight_id = get_string(fp);
        f_temp->origin = get_string(fp);
        f_temp->destination = get_string(fp);
        fscanf(fp, "%d %s %d - %d:%d\n",
               &f_temp->departure.date.day, f_temp->departure.date.month, &f_temp->departure.date.year,
               &f_temp->departure.time.hours, &f_temp->departure.time.minutes);
        fscanf(fp, "%d %s %d - %d:%d\n",
               &f_temp->arrival.date.day, f_temp->arrival.date.month, &f_temp->arrival.date.year,
               &f_temp->arrival.time.hours, &f_temp->arrival.time.minutes);
        fscanf(fp, "%d\n", &f_temp->passenger_qty);
        fscanf(fp, "%d\n", &f_temp->passenger_max);
        fscanf(fp, "%d\n", &f_temp->bonus_miles);

        // Insert the Flight to the Linked List
        insert_flight_node(&(*f_head), f_temp);
    }
    fclose(fp); // Close the file

    // Load Passengers
    fp = fopen(PASSENGERS_FILE, "r");
    if (fp == NULL) // If the file does not exist
    {
        printf("File 'passengers.txt' not found, attempting to create.\n\n");
        fp = fopen(FLIGHTS_FILE, "w"); // Create the file
        if (fp == NULL)                // If fopen failed
        {
            printf("[Error] Critical error. Can't open file.\n\n");
            clean_exit();
        }
        fclose(fp); // Close the file
        printf("Created 'passengers.txt'.\n\n");
        return; // Stop loading
    }

    fscanf(fp, "%d\n", &passenger_count); // Read the passenger count
    if (passenger_count == 0)             // If there are no passengers
    {
        return; // Stop loading
    }

    while (!feof(fp)) // While there are passengers to read
    {
        // Create a Passenger node
        Passenger *p_temp = create_passenger_node();

        // Scan each field from the file
        p_temp->last_name = get_string(fp);
        p_temp->first_name = get_string(fp);
        p_temp->passport_number = get_string(fp);
        fscanf(fp, "%d %s %d\n",
               &p_temp->birthdate.day, p_temp->birthdate.month, &p_temp->birthdate.year);
        fscanf(fp, "%d\n", &p_temp->reservation_qty);
        for (int i = 0; i < p_temp->reservation_qty; i++)
        {
            // Get the Flight ID
            char *flight_id = get_string(fp);

            // Search for the flight
            reserved_flight = search_flight_node(*f_head, flight_id);
            if (reserved_flight == NULL)
            {
                printf("[Error] Critical error. Flight missing from flights.txt file.\nCannot continue.\n");
                printf("[Info] Please do not edit the files manually.\n\n");
                clean_exit();
            }

            free(flight_id); // Free the string

            // Create a Reservation node
            r_ptr = create_reservation_node(reserved_flight);
            // Insert the Reservation node to the passenger's reservations linked list
            insert_reservation_node(&p_temp->reservations, r_ptr);
        }
        fscanf(fp, "%d\n", &p_temp->miles);

        // Insert to the Passengers linked list
        insert_passenger_node(&(*p_head), p_temp);
    }
    fclose(fp); // Close the file
}

void save(Flight *f_head, Passenger *p_head)
{
    Flight *f_ptr = f_head;                         // Start from the head
    Passenger *p_ptr = p_head;                      // Start from the head
    Reservation *r_ptr = NULL;                      // Pointer to a Reservation
    int flight_count = count_flights(f_head);       // Count the quantity of Flights
    int passenger_count = count_passengers(p_head); // Count the quantity of Passengers

    // Save Flights
    FILE *fp = fopen(FLIGHTS_FILE, "w");

    // Write the Flight count
    fprintf(fp, "%d\n", flight_count);

    // Print each Flight to the file
    while (f_ptr != NULL)
    {
        fprintf(fp, "%s\n", f_ptr->flight_id);
        fprintf(fp, "%s\n", f_ptr->origin);
        fprintf(fp, "%s\n", f_ptr->destination);
        fprintf(fp, "%d %s %d - %02d:%02d\n",
                f_ptr->departure.date.day, f_ptr->departure.date.month, f_ptr->departure.date.year,
                f_ptr->departure.time.hours, f_ptr->departure.time.minutes);
        fprintf(fp, "%d %s %d - %02d:%02d\n",
                f_ptr->arrival.date.day, f_ptr->arrival.date.month, f_ptr->arrival.date.year,
                f_ptr->arrival.time.hours, f_ptr->arrival.time.minutes);
        fprintf(fp, "%d\n", f_ptr->passenger_qty);
        fprintf(fp, "%d\n", f_ptr->passenger_max);
        fprintf(fp, "%d\n", f_ptr->bonus_miles);

        f_ptr = f_ptr->next;
    }
    fclose(fp); // Close the file

    // Save Passengers
    fp = fopen(PASSENGERS_FILE, "w");

    // Write the Passenger count
    fprintf(fp, "%d\n", passenger_count);

    // Write each Passenger to the file
    while (p_ptr != NULL)
    {
        fprintf(fp, "%s\n", p_ptr->last_name);
        fprintf(fp, "%s\n", p_ptr->first_name);
        fprintf(fp, "%s\n", p_ptr->passport_number);
        fprintf(fp, "%d %s %d\n",
                p_ptr->birthdate.day, p_ptr->birthdate.month, p_ptr->birthdate.year);
        fprintf(fp, "%d\n", p_ptr->reservation_qty);
        r_ptr = p_ptr->reservations;
        for (int i = 0; i < p_ptr->reservation_qty; i++)
        {
            fprintf(fp, "%s\n", r_ptr->flight->flight_id);
            r_ptr = r_ptr->next;
        }
        fprintf(fp, "%d\n", p_ptr->miles);

        p_ptr = p_ptr->next;
    }
    fclose(fp); // Close the file
}
