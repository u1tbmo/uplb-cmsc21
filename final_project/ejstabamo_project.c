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
    {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December",
};

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
    Date date;
    Time time;
} DateTime;

typedef struct Flight // A structure for a Flight with flight details.
{
    char *flight_id;
    char *destination;
    char *origin;
    DateTime departure;
    DateTime arrival;
    int passenger_qty;
    int passenger_max;
    int bonus_miles;
    struct Flight *next;
} Flight;

typedef struct Reservation // A structure for a Reservation with a Flight.
{
    Flight *flight;
    struct Reservation *next;
} Reservation;

typedef struct Passenger // A structure for a Passenger with passenger details.
{
    char *first_name;
    char *last_name;
    Date birthdate;
    char *passport_number;
    int miles;
    int reservation_qty;
    struct Reservation *reservations;
    struct Passenger *next;
} Passenger;

// Helper Functions

int main_menu();                                                                                     // Prints a menu and returns an integer
int view_flights_menu();                                                                             // Prints the menu for viewing flights and returns an integer
bool confirm_delete();                                                                               // Asks the user if they are sure of deleting data
void clear_buffer();                                                                                 // Clears the input buffer
bool is_leap(int year);                                                                              // Checks if a year is a leap year
bool valid_month(char *input);                                                                       // Checks if a month is valid
bool valid_day(int day, char *month, int year);                                                      // Checks if a day is valid
int flight_compare(Flight *a, Flight *b);                                                            // Compares two flights
int passenger_compare(Passenger *a, Passenger *b);                                                   // Compares two passengers
bool validate_id(char *id);                                                                          // Checks if an ID is valid
bool validate_passport(char *passport_numnber);                                                      // Checks if a passport is valid
int month_to_int(char *month);                                                                       // Converts a month to its number
bool is_future(DateTime old, DateTime new);                                                          // Checks if a date is in the future (from another date)
bool is_conflicting(DateTime departure1, DateTime arrival1, DateTime departure2, DateTime arrival2); // Checks if a DateTime is in the bounds of another DateTime
long long datetime_to_minutes(DateTime dt);

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

// Linked List Functions

Flight *create_flight_node();                                             // Creates a Flight node
Flight *search_flight_node(Flight *head, char *flight_id);                // Searches for a Flight in the linked list and returns a pointer to it
void insert_flight_node(Flight **head, Flight *node);                     // Insert a Flight to the linked list considering order
void delete_flight_node(Flight **head, char *flight_id);                  // Deletes a Flight from the linked list
int count_flights(Flight *head);                                          // Counts the number of Flights in the linked list
Passenger *create_passenger_node();                                       // Creates a Passenger Node
Passenger *search_passenger_node(Passenger *head, char *passport_number); // Searches for a Passenger in the linked list and returns a pointer to it
void insert_passenger_node(Passenger **head, Passenger *node);            // Insert a Passenger to the linked list considering order
int count_passengers(Passenger *head);                                    // Counts the number of Passengers in the linked list
Reservation *create_reservation_node(Flight *flight);                     // Creates a Reservation Node
Reservation *search_reservation_node(Reservation *head, Flight *flight);  // Searches for a Reservation in the linked list and returns a pointer to it
void insert_reservation_node(Reservation **head, Reservation *node);      // Insert a Reservation to the linked list considering order
void delete_reservation_node(Reservation **head, Flight *flight);         // Deletes a Reservation from the linked list
int count_reservations(Reservation *head);                                // Counts the number of Reservations in the linked list

// Program Functionality

void add_flight(Flight **head);                             // Adds a flight to the database
void edit_flight(Flight *head);                             // Edits a flight in the database
void view_flights(Flight *head, int mode);                  // Views flights in the database
bool view_flights_linear(Flight *head, int mode);           // Views flights in the database (linear format)
void view_passengers_linear(Passenger *head);               // Views passengers in the database (linear format)
void view_reservations_linear(Reservation *head);           // Views reservations for a passenger (linear format)
void delete_flight(Flight **head);                          // Deletes a flight from the database
void add_passenger(Passenger **head);                       // Adds a passenger to the database
void edit_passenger(Passenger *head);                       // Edits a passenger in the database
void book_reservation(Flight *f_head, Passenger *p_head);   // Books a flight reservation for a passenger
void remove_reservation(Flight *f_head, Passenger *p_head); // Removes a flight reservation from a passenger
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
        choice = main_menu();
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

    printf("================== Menu ===================\n\n");
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

int view_flights_menu()
{
    int choice;

    printf("============== View Flights ===============\n\n");
    printf("1 | View Specific Flight\n");
    printf("2 | View All Available Flights\n");
    printf("3 | View All Fully-booked Flights\n");
    printf("4 | View All Flights\n");
    printf("0 | Back\n\n");
    choice = get_int("Enter choice: ");

    return choice;
}

bool confirm_delete()
{
    char *choice; // Variable to hold user's input

    printf("Enter [Y] to confirm deletion: ");
    choice = get_string(stdin);

    if (strcmp(choice, "Y") == 0) // Check if the user only entered "Y"
    {
        free(choice);
        return true; // return true if input matches "Y"
    }
    else
    {
        printf("[Info] '%s' does not match 'Y'. Cancelling.\n\n", choice);
        free(choice);
        return false; // return false otherwise
    }
}

void clear_buffer()
{
    int c;
    while ((c = fgetc(stdin)) != '\n' && c != EOF)
        ; // clears the input buffer
}

bool valid_month(char *input)
{
    for (int i = 0; i < 12; i++) // Iterate over the MONTHS array
    {
        if (strcmp(input, MONTHS[i]) == 0)
        {
            return true; // return true if input is in the MONTHS array
        }
    }
    return false; // return false otherwise
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

bool valid_day(int day, char *month, int year)
{
    if (strcmp(month, "February") == 0) // Check if the month is February
    {
        if (is_leap(year)) // If the year is a leap year, check if day is in range [1,29]
        {
            return day >= 1 && day <= 29;
        }
        else // Otherwise, check if day is in range [1,28]
        {
            return day >= 1 && day <= 28;
        }
    }
    else if (strcmp(month, "April") == 0 || strcmp(month, "June") == 0 ||
             strcmp(month, "September") == 0 || strcmp(month, "November") == 0)
    {
        return day >= 1 && day <= 30; // Check if the day is in range [1,30]
    }
    else
    {
        return day >= 1 && day <= 31; // Check if the day is in range [1,31]
    }
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
        return origin_cmp;
    }

    // Sort by Destination
    int destination_cmp = strcmp(a->destination, b->destination);
    if (destination_cmp != 0)
    {
        return destination_cmp;
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
        return first_name_cmp;
    }

    // Sort by Last Names
    int last_name_cmp = strcmp(a->last_name, b->last_name);
    if (last_name_cmp != 0)
    {
        return last_name_cmp;
    }

    // If all else fails, the passengers are equal
    return 0;
}

bool validate_id(char *id)
{
    if (strlen(id) > FLIGHT_ID_LEN)
    {
        return false;
    }
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
    for (int i = 0; i < 12; i++) // Iterate over the MONTHS array
    {
        if (strcmp(month, MONTHS[i]) == 0)
        {
            return i + 1; // Returns the month number of a month string
        }
    }
    return -1;
}

bool is_future(DateTime old, DateTime new)
{
    // Convert DateTime to minutes since some point, for easy comparison
    long long old_in_minutes = datetime_to_minutes(old);
    long long new_in_minutes = datetime_to_minutes(new);

    // Check if the new DateTime is in the future compared to the old DateTime
    return new_in_minutes > old_in_minutes;
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

long long datetime_to_minutes(DateTime dt)
{
    return dt.date.year * 525600 +
           month_to_int(dt.date.month) * 43800 +
           dt.date.day * 1440 +
           dt.time.hours * 60 +
           dt.time.minutes;
}

char *get_string(FILE *stream)
{
    // Variables
    int buffer_length = INITIAL_BUFFER_LEN, c, i = 0;
    char *temp = NULL;

    // Allocate memory for a string
    char *buffer = (char *)malloc(sizeof(char) * buffer_length);
    if (buffer == NULL) // If malloc failed
    {
        printf("[Error] Memory allocation failed.\n\n");
        return NULL;
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
                return NULL;
            }
        }
    }

    // Null-terminate the string
    buffer[i] = '\0';

    temp = (char *)realloc(buffer, sizeof(char) * (i + 1)); // Shrink the buffer to the actual size
    if (temp != NULL)                                       // If realloc does not fail
    {
        buffer = temp; // The buffer is resized to the optimal size
    }
    // If realloc DOES fail, return the string with unoptimal size

    return buffer;
}

int get_int(char *prompt)
{
    int number;         // Holds the number that is converted from the string
    char *input = NULL; // Holds the string from the user
    char *endptr;       // Stores the endptr from strtol

    // Repeatedly ask for an integer
    do
    {
        // Ask the user for a number in string form
        printf("%s", prompt);
        input = get_string(stdin);

        number = strtol(input, &endptr, 0); // Convert the string to an integer

        // Check for invalid invalid input
        if (*endptr != '\0' || input[0] == '\n' || endptr == input)
        {
            printf("[Error] Please enter a valid positive integer.\n");
        }
    } while (*endptr != '\0' || input[0] == '\n' || endptr == input);
    free(input); // Free the string

    return number;
}

Date get_date()
{
    Date new_date; // Holds the new Date
    char *input = NULL, *endptr;
    bool month_is_valid, day_is_valid;

    // Ask for a year
    do
    {
        new_date.year = get_int("Year:  ");
        if (new_date.year < 1909) // First recorded use of an airport :)
        {
            printf("[Error] Invalid year. Please type a valid year.\n");
        }
    } while (new_date.year < 1909);

    // Ask for a month
    do
    {
        printf("Month: ");
        input = get_string(stdin);

        month_is_valid = valid_month(input);
        if (!month_is_valid)
        {
            printf("[Error] Invalid month. Please type a valid month name.\n");
        }
        else
        {
            strcpy(new_date.month, input);
        }

        free(input);
    } while (!month_is_valid);

    // Ask for a day
    do
    {
        new_date.day = get_int("Day:   ");

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
    DateTime new_datetime;
    Date new_date;
    Time new_time;
    char *input = NULL, *endptr;
    bool month_is_valid, datetime_is_valid = true;

    do
    {
        // Ask for a year
        do
        {
            new_date.year = get_int("Year:    ");
            if (new_date.year < 1909) // First recorded use of an airport :)
            {
                printf("[Error] Invalid year. Please type a valid year.\n");
            }
        } while (new_date.year < 1909);

        // Ask for a month
        do
        {
            if (input != NULL)
            {
                free(input);
                input = NULL;
            }
            printf("Month:   ");
            input = get_string(stdin);
            month_is_valid = valid_month(input);
            if (!month_is_valid)
            {
                printf("[Error] Invalid month. Please type a valid month name.\n");
            }
            else
            {
                strcpy(new_date.month, input);
            }
        } while (!month_is_valid);
        free(input);
        input = NULL;

        // Ask for a day
        do
        {
            new_date.day = get_int("Day:     ");
            if (!(valid_day(new_date.day, new_date.month, new_date.year)))
            {
                printf("[Error] Invalid day. Please type a valid day.\n");
            }

        } while (!(valid_day(new_date.day, new_date.month, new_date.year)));

        // Ask for hours
        do
        {
            new_time.hours = get_int("Hour:    ");
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

        new_datetime = (DateTime){.date = new_date, .time = new_time};
        if (is_arrival)
        {
            datetime_is_valid = is_future(*departure, new_datetime);
        }

        if (!datetime_is_valid)
        {
            printf("[Error] Your arrival must be after the departure.\n");
        }
    } while (!datetime_is_valid);
    return new_datetime;
}

void free_flight_node(Flight *node)
{
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
        return NULL;
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
    // Traverse the linked list
    Flight *curr = head;
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
    Flight *prev = *head;
    Flight *curr = *head;
    Flight *temp;

    if (search_flight_node(*head, flight_id) == NULL) // If the flight does not exist
    {
        printf("[Error] Flight does not exist.\n\n");
        return;
    }

    if (strcmp((*head)->flight_id, flight_id) == 0) // If the node to delete is the head
    {
        temp = *head;
        *head = (*head)->next;

        free_flight_node(temp);
        return;
    }

    while (strcmp(curr->flight_id, flight_id) != 0) // Find the node to delete
    {
        prev = curr;
        curr = curr->next;
    }
    prev->next = curr->next; // Remove the node from the list

    free_flight_node(curr); // Free the deleted node
}

int count_flights(Flight *head)
{
    int count = 0;
    Flight *ptr = head;

    while (ptr != NULL)
    {
        count++;
        ptr = ptr->next;
    }

    return count;
}

Passenger *create_passenger_node()
{
    // Allocate memory for a Passenger node
    Passenger *new_passenger = (Passenger *)malloc(sizeof(Passenger));
    if (new_passenger == NULL) // If malloc failed
    {
        printf("[Error] Memory allocation failed.\n\n");
        return NULL;
    }

    // Initialize fields to defaults
    *new_passenger = (Passenger){
        .first_name = NULL,
        .last_name = NULL,
        .birthdate = (Date){.day = 0, .month = "", .year = 0},
        .passport_number = NULL,
        .reservation_qty = 0,
        .miles = 0}; // Compound Literal

    return new_passenger;
}

Passenger *search_passenger_node(Passenger *head, char *passport_number)
{
    // Traverse the linked list
    Passenger *curr = head;
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
    Passenger *curr = *head;

    // If the list is empty OR the new node is before the *head in order
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
    Passenger *ptr = head;

    while (ptr != NULL)
    {
        count++;
        ptr = ptr->next;
    }

    return count;
}

Reservation *create_reservation_node(Flight *flight)
{
    // Allocate memory for a Reservation node
    Reservation *new_reservation = (Reservation *)malloc(sizeof(Reservation));
    if (new_reservation == NULL) // If malloc failed
    {
        printf("[Error] Memory allocation failed.\n\n");
        return NULL;
    }

    // Initialize fields to defaults
    *new_reservation = (Reservation){
        .flight = flight, .next = NULL}; // Compound Literal

    return new_reservation;
}

Reservation *search_reservation_node(Reservation *head, Flight *flight)
{
    // Traverse the linked list
    Reservation *curr = head;
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
    Reservation *prev = *head;
    Reservation *curr = *head;
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
    Reservation *ptr = head;

    while (ptr != NULL)
    {
        count++;
        ptr = ptr->next;
    }

    return count;
}

void add_flight(Flight **head)
{
    char *temp = NULL;

    printf("== Add Flight =============================\n\n");

    // Flight ID
    printf("Flight ID:   ");
    temp = get_string(stdin);
    if (!validate_id(temp))
    {
        printf("[Error] A valid Flight ID has at most 6 uppercase letters and/or digits only.\n\n");
        free(temp);
        return;
    }
    if (search_flight_node(*head, temp) != NULL)
    {
        printf("[Error] That flight already exists.\n\n");
        free(temp);
        return;
    }

    Flight *new_flight = create_flight_node();

    new_flight->flight_id = temp;

    // Origin
    printf("Origin:      ");
    new_flight->origin = get_string(stdin);

    // Destination
    printf("Destination: ");
    new_flight->destination = get_string(stdin);

    // Departure
    printf("\n-- Departure -------------------------\n\n");
    new_flight->departure = get_datetime(false, NULL);

    // Arrival
    printf("\n-- Arrival ---------------------------\n\n");
    new_flight->arrival = get_datetime(true, &new_flight->departure);

    // Number of Booked Passengers is already set to 0

    // Max Count of Passengers
    printf("\n--------------------------------------\n");
    do
    {
        new_flight->passenger_max = get_int("Max Passengers: ");
        if (new_flight->passenger_max < MIN_PASSENGERS)
        {
            printf("[Error] The maximum number of passengers must be at least %d.\n", MIN_PASSENGERS);
        }
    } while (new_flight->passenger_max < MIN_PASSENGERS);

    // Bonus Miles
    do
    {
        new_flight->bonus_miles = get_int("Bonus Miles:    ");
        if (new_flight->bonus_miles < 0)
        {
            printf("[Error] The bonus miles must be at least 0.\n");
        }
    } while (new_flight->bonus_miles < 0);

    insert_flight_node(&(*head), new_flight);

    printf("\n[Success] Added Flight %s.\n\n", new_flight->flight_id);
}

void edit_flight(Flight *head)
{
    char *flight_id;
    Flight *node_ptr = NULL;

    printf("== Edit Flight ============================\n\n");

    view_flights_linear(head, 1);

    // Ask for a Flight ID
    printf("Flight ID:   ");
    flight_id = get_string(stdin);
    if (!validate_id(flight_id))
    {
        printf("[Error] A valid Flight ID has at most 6 uppercase letters and/or digits only.\n\n");
        return;
    }
    if ((node_ptr = search_flight_node(head, flight_id)) == NULL)
    {
        printf("[Error] That flight does not exist.\n\n");
        return;
    }
    free(flight_id);

    // New Departure DateTime
    printf("\n-- Departure -------------------------\n\n");
    node_ptr->departure = get_datetime(false, NULL);

    // New Arrival DateTime
    printf("\n-- Arrival ---------------------------\n\n");
    node_ptr->arrival = get_datetime(true, &node_ptr->departure);

    // New Max Count of Passengers
    printf("\n--------------------------------------\n");
    do
    {
        node_ptr->passenger_max = get_int("Max Passengers: ");
        if (node_ptr->passenger_max < MIN_PASSENGERS)
        {
            printf("[Error] The maximum number of passengers must be at least %d.\n", MIN_PASSENGERS);
        }
        if (node_ptr->passenger_max < node_ptr->passenger_qty)
        {
            printf("[Error] You cannot decrease maximum passengers below number of reserved passengers.\n");
        }
    } while (node_ptr->passenger_max < MIN_PASSENGERS);

    printf("\n[Success] Edited Flight %s.\n\n", node_ptr->flight_id);
}

void view_flights(Flight *head, int mode)
{
    Flight *curr = head;
    int count = 0;

    switch (mode)
    {
    case 1:
        char *flight_id;
        printf("-- View Flights > Specific -----------\n\n");

        view_flights_linear(head, 3);

        // Ask for a Flight ID
        printf("Flight ID: ");
        flight_id = get_string(stdin);
        printf("\n");
        if (!validate_id(flight_id))
        {
            printf("[Error] A valid Flight ID has at most 6 uppercase letters and/or digits only.\n\n");
            free(flight_id);
            return;
        }
        if ((curr = search_flight_node(head, flight_id)) == NULL)
        {
            printf("[Error] Flight does not exist.\n\n");
            free(flight_id);
            return;
        }
        free(flight_id);

        printf("Flight ID: %s\n", curr->flight_id);
        printf("Flight:    %s to %s\n", curr->origin, curr->destination);
        printf("- Departure: %d %s %d - %02d:%02d\n",
               curr->departure.date.day, curr->departure.date.month, curr->departure.date.year,
               curr->departure.time.hours, curr->departure.time.minutes);
        printf("- Arrival:   %d %s %d - %02d:%02d\n",
               curr->arrival.date.day, curr->arrival.date.month, curr->arrival.date.year,
               curr->arrival.time.hours, curr->arrival.time.minutes);
        printf("Passengers:  %d\n", curr->passenger_qty);
        printf("Max Seats:   %d\n", curr->passenger_max);
        printf("Bonus Miles: %d\n", curr->bonus_miles);
        printf("\n");

        break;
    case 2:
        printf("-- View Flights > Available ----------\n\n");
        while (curr != NULL)
        {
            if (curr->passenger_qty < curr->passenger_max)
            {
                printf("Flight ID: %s\n", curr->flight_id);
                printf("Flight:    %s to %s\n", curr->origin, curr->destination);
                printf("- Departure: %d %s %d - %02d:%02d\n",
                       curr->departure.date.day, curr->departure.date.month, curr->departure.date.year,
                       curr->departure.time.hours, curr->departure.time.minutes);
                printf("- Arrival:   %d %s %d - %02d:%02d\n",
                       curr->arrival.date.day, curr->arrival.date.month, curr->arrival.date.year,
                       curr->arrival.time.hours, curr->arrival.time.minutes);
                printf("Passengers:  %d\n", curr->passenger_qty);
                printf("Max Seats:   %d\n", curr->passenger_max);
                printf("Bonus Miles: %d\n", curr->bonus_miles);
                printf("\n");
                count++;
            }
            curr = curr->next;
        }

        if (count == 0)
        {
            printf("\nNo flights fit the criteria.\n\n");
        }
        break;
    case 3:
        printf("-- View Flights > Fully-Booked -------\n\n");
        while (curr != NULL)
        {
            if (curr->passenger_qty == curr->passenger_max)
            {
                printf("Flight ID: %s\n", curr->flight_id);
                printf("Flight:    %s to %s\n", curr->origin, curr->destination);
                printf("- Departure: %d %s %d - %02d:%02d\n",
                       curr->departure.date.day, curr->departure.date.month, curr->departure.date.year,
                       curr->departure.time.hours, curr->departure.time.minutes);
                printf("- Arrival:   %d %s %d - %02d:%02d\n",
                       curr->arrival.date.day, curr->arrival.date.month, curr->arrival.date.year,
                       curr->arrival.time.hours, curr->arrival.time.minutes);
                printf("Passengers:  %d\n", curr->passenger_qty);
                printf("Max Seats:   %d\n", curr->passenger_max);
                printf("Bonus Miles: %d\n", curr->bonus_miles);
                printf("\n");
                count++;
            }
            curr = curr->next;
        }

        if (count == 0)
        {
            printf("No flights fit the criteria.\n\n");
        }
        break;
    case 4:
        printf("-- View Flights > All ----------------\n\n");
        while (curr != NULL)
        {
            printf("Flight ID: %s\n", curr->flight_id);
            printf("Flight:    %s to %s\n", curr->origin, curr->destination);
            printf("- Departure: %d %s %d - %02d:%02d\n",
                   curr->departure.date.day, curr->departure.date.month, curr->departure.date.year,
                   curr->departure.time.hours, curr->departure.time.minutes);
            printf("- Arrival:   %d %s %d - %02d:%02d\n",
                   curr->arrival.date.day, curr->arrival.date.month, curr->arrival.date.year,
                   curr->arrival.time.hours, curr->arrival.time.minutes);
            printf("Passengers:  %d\n", curr->passenger_qty);
            printf("Max Seats:   %d\n", curr->passenger_max);
            printf("Bonus Miles: %d\n", curr->bonus_miles);
            printf("\n");
            curr = curr->next;
        }
        break;
    default:
        printf("\n[Error] Invalid choice.\n\n");
        break;
    }
}

bool view_flights_linear(Flight *head, int mode)
{
    Flight *ptr = head;
    int count = 0;
    switch (mode)
    {
    case 1: // Available Flights (for Booking)
        printf("-- All Flights -----------------------\n\n");
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
    case 2: // Empty Flights (for Deleting)
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
            printf("No flights are empty.\n\n");
            return false;
        }
        printf("\n");
        break;
    case 3: // All Flights (for Viewing)
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
    Passenger *ptr = head;

    printf("-- Passengers ------------------------\n\n");
    while (ptr != NULL)
    {
        printf("%s, %s | %s | %d flights reserved\n",
               ptr->last_name, ptr->first_name,
               ptr->passport_number, ptr->reservation_qty);
        ptr = ptr->next;
    }
    printf("\n");
}

void view_reservations_linear(Reservation *head)
{
    Reservation *ptr = head;
    Flight *f_ptr = NULL;
    printf("-- Reservations ----------------------\n\n");
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
    Flight *ptr;
    bool empty_flights_exists;

    printf("== Delete Flight ==========================\n\n");

    empty_flights_exists = view_flights_linear(*head, 2);

    if (!empty_flights_exists)
    {
        return;
    }

    // Ask for a Flight ID
    printf("Flight ID: ");
    char *flight_id = get_string(stdin);
    printf("\n");

    if (!validate_id(flight_id))
    {
        printf("[Error] A valid Flight ID has at most 6 uppercase letters and/or digits only.\n\n");
        return;
    }
    if ((ptr = search_flight_node(*head, flight_id)) == NULL)
    {
        printf("[Error] That flight does not exist.\n\n");
        return;
    }
    if (ptr->passenger_qty != 0)
    {
        printf("[Error] Passengers have already booked this flight.\n\n");
        return;
    }
    free(flight_id);

    if (confirm_delete())
    {
        printf("[Success] Deleted flight %s.\n\n", ptr->flight_id);
        delete_flight_node(&(*head), ptr->flight_id);
    }
    else
    {
        printf("[Info] Did not delete flight.\n\n");
    }
}

void add_passenger(Passenger **head)
{
    printf("== Add Passenger ==========================\n\n");

    char *first_name;
    char *last_name;
    Date birthdate;
    char *passport_number;
    bool passport_is_valid = false;

    // Name
    printf("First Name: ");
    first_name = get_string(stdin);
    printf("Last Name:  ");
    last_name = get_string(stdin);

    // Birthdate
    printf("\n-- Date of Birth ---------------------\n\n");
    birthdate = get_date();
    printf("\n--------------------------------------\n\n");

    // Passport Number
    do
    {
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
            return;
        }
    } while (!passport_is_valid);

    Passenger *new_passenger = create_passenger_node();

    new_passenger->first_name = first_name;
    new_passenger->last_name = last_name;
    new_passenger->birthdate = birthdate;
    new_passenger->passport_number = passport_number;

    // Number of Miles
    new_passenger->miles = get_int("Number of Miles: ");

    insert_passenger_node(*(&head), new_passenger);

    printf("\n[Success] Added Passenger %s.\n\n", new_passenger->first_name);
}

void edit_passenger(Passenger *head)
{
    printf("== Edit Passenger =========================\n\n");

    view_passengers_linear(head);

    char *passport_number;
    Passenger *node_ptr = NULL;

    // Ask for a Passport Number
    printf("Passport Number: ");
    passport_number = get_string(stdin);
    printf("\n");
    if (!validate_passport(passport_number))
    {
        printf("[Error] A valid Passport Number has 9 uppercase letters and/or digits.\n\n");
        return;
    }
    if ((node_ptr = search_passenger_node(head, passport_number)) == NULL)
    {
        printf("[Error] That passenger does not exist.\n\n");
        return;
    }
    free(passport_number);

    // New Last Name
    printf("Last Name: ");
    free(node_ptr->last_name);
    node_ptr->last_name = get_string(stdin);

    // New Birthdate
    printf("\n-- Date of Birth ---------------------\n\n");
    node_ptr->birthdate = get_date();
    printf("\n--------------------------------------\n\n");

    printf("\n[Success] Edited Passenger %s.\n\n", node_ptr->first_name);
}

void book_reservation(Flight *f_head, Passenger *p_head)
{
    // Variables
    Passenger *passenger = NULL;
    Flight *flight = NULL;
    DateTime *d_ptr = NULL, *a_ptr = NULL;
    Reservation *new_reservation = NULL;
    Reservation *reservation_ptr = NULL;
    char *passport_number, *flight_id;

    printf("== Book Reservation =======================\n\n");

    view_passengers_linear(p_head);

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

    view_flights_linear(f_head, 1);

    printf("Flight ID: ");
    flight_id = get_string(stdin);
    printf("\n");
    if (!validate_id(flight_id))
    {
        printf("[Error] A valid Flight ID has at most 6 uppercase letters and/or digits only.\n\n");
        return;
    }
    if ((flight = search_flight_node(f_head, flight_id)) == NULL)
    {
        printf("[Error] That flight does not exist.\n\n");
        return;
    }
    if (flight->passenger_qty == flight->passenger_max)
    {
        printf("[Error] That flight is fully booked.\n\n");
        return;
    }

    reservation_ptr = passenger->reservations;

    for (int i = 0; i < passenger->reservation_qty; i++)
    {
        if (reservation_ptr->flight->flight_id == flight->flight_id) // This works because they're the same addresses.
        {
            printf("[Error] You already reserved this flight.\n\n");
            return;
        }

        d_ptr = &reservation_ptr->flight->departure;
        a_ptr = &reservation_ptr->flight->arrival;

        if (is_conflicting(reservation_ptr->flight->departure,
                           reservation_ptr->flight->arrival,
                           flight->departure, flight->arrival))
        {
            printf("[Error] That flight conflicts with current reservations.\n");
            printf("Reserving:        ");
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
    free(flight_id);

    new_reservation = create_reservation_node(flight);

    insert_reservation_node(&passenger->reservations, new_reservation);

    flight->passenger_qty++;
    passenger->reservation_qty++;
    passenger->miles += flight->bonus_miles;

    printf("[Success] Reserved Flight %s for %s.\n\n", flight->flight_id, passenger->first_name);
}

void remove_reservation(Flight *f_head, Passenger *p_head)
{
    char *passport_number, *flight_id;
    Passenger *passenger = NULL;
    Flight *flight = NULL;
    Reservation *r_ptr = NULL;
    Reservation *reservation = NULL;

    printf("== Remove Reservation =====================\n\n");

    view_passengers_linear(p_head);

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
    if (passenger->reservation_qty == 0)
    {
        printf("[Error] That passenger has no reservations.\n\n");
        return;
    }
    free(passport_number);

    view_reservations_linear(passenger->reservations);

    printf("Flight ID: ");
    flight_id = get_string(stdin);
    printf("\n");
    if (!validate_id(flight_id))
    {
        printf("[Error] A valid Flight ID has at most 6 uppercase letters and/or digits only.\n\n");
        return;
    }
    if ((flight = search_flight_node(f_head, flight_id)) == NULL)
    {
        printf("[Error] That flight does not exist.\n\n");
        return;
    }
    free(flight_id);

    reservation = search_reservation_node(passenger->reservations, flight);

    if (confirm_delete())
    {
        passenger->reservation_qty--;
        reservation->flight->passenger_qty--;
        passenger->miles -= reservation->flight->bonus_miles;

        printf("[Success] Deleted reservation %s for %s.\n\n", reservation->flight->flight_id, passenger->first_name);
        delete_reservation_node(&passenger->reservations, reservation->flight);
    }
    else
    {
        printf("[Info] Did not delete reservation.\n\n");
    }
}

void view_reservations(Passenger *head)
{
    char *passport_number;
    Passenger *p_ptr = NULL;
    Flight *f_ptr = NULL;
    Reservation *r_ptr = NULL;

    printf("== View Reservations ======================\n\n");

    view_passengers_linear(head);

    printf("Passport Number: ");
    passport_number = get_string(stdin);
    printf("\n");
    if (!validate_passport(passport_number))
    {
        printf("[Error] A valid Passport Number has 9 uppercase letters and/or digits.\n\n");
        return;
    }
    if ((p_ptr = search_passenger_node(head, passport_number)) == NULL)
    {
        printf("[Error] That passenger does not exist.\n\n");
        return;
    }
    if (p_ptr->reservation_qty == 0)
    {
        printf("[Error] That passenger has no reservations.\n\n");
        return;
    }
    free(passport_number);

    printf("-- Reservations ----------------------\n\n");

    r_ptr = p_ptr->reservations;
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

    // Load Flights
    FILE *fp = fopen(FLIGHTS_FILE, "r");
    if (fp == NULL)
    {
        printf("== Load ==============================\n\n");
        printf("First time running the program.\nCreated 'flights.txt'.\n\n");
        fp = fopen(FLIGHTS_FILE, "w");
        fclose(fp);
        return;
    }

    fscanf(fp, "%d\n", &flight_count);
    if (flight_count == 0)
    {
        return;
    }

    while (!feof(fp))
    {
        Flight *f_temp = create_flight_node();
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

        insert_flight_node(&(*f_head), f_temp);
    }
    fclose(fp);

    // Load Passengers
    fp = fopen(PASSENGERS_FILE, "r");
    if (fp == NULL)
    {
        printf("Created 'passengers.txt'.\n\n");
        fp = fopen(FLIGHTS_FILE, "w");
        fclose(fp);
        return;
    }

    fscanf(fp, "%d\n", &passenger_count);
    if (passenger_count == 0)
    {
        return;
    }

    while (!feof(fp))
    {
        Passenger *p_temp = create_passenger_node();
        p_temp->last_name = get_string(fp);
        p_temp->first_name = get_string(fp);
        p_temp->passport_number = get_string(fp);
        fscanf(fp, "%d %s %d\n",
               &p_temp->birthdate.day, p_temp->birthdate.month, &p_temp->birthdate.year);
        fscanf(fp, "%d\n", &p_temp->reservation_qty);
        for (int i = 0; i < p_temp->reservation_qty; i++)
        {
            Flight *reserved_flight = NULL;
            char *flight_id = get_string(fp);

            reserved_flight = search_flight_node(*f_head, flight_id);

            free(flight_id);

            Reservation *r_temp = create_reservation_node(reserved_flight);

            insert_reservation_node(&p_temp->reservations, r_temp);
        }
        fscanf(fp, "%d\n", &p_temp->miles);

        insert_passenger_node(&(*p_head), p_temp);
    }
    fclose(fp);
}

void save(Flight *f_head, Passenger *p_head)
{
    Flight *f_ptr = f_head;
    Passenger *p_ptr = p_head;
    Reservation *r_ptr = NULL;
    int flight_count = count_flights(f_head);
    int passenger_count = count_passengers(p_head);

    // Save Flights
    FILE *fp = fopen(FLIGHTS_FILE, "w");

    fprintf(fp, "%d\n", flight_count);

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
    fclose(fp);

    // Save Passengers
    fp = fopen(PASSENGERS_FILE, "w");

    fprintf(fp, "%d\n", passenger_count);

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
    fclose(fp);
}