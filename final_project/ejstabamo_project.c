/*  Tabamo, Euan Jed S. - ST2L - June 2, 2024
    Final Project - Flight Passenger Management System
*/

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ANSI Color Codes - https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a */

#define RED "\e[0;91m"
#define GREEN "\e[0;92m"
#define YELLOW "\e[0;93m"
#define BLUE "\e[0;94m"
#define B_CYAN "\e[1;36m"
#define RESET "\e[0m"

/* Global Constants/Definitions
    - STR_LEN means is for a strlen(), without the NUL terminator.
    - SIZE means is for a size, like an array/string, which (for a string) will include the NUL terminator.
*/

#define FLIGHT_ID_STR_LEN 6    // the strlen() of a flight ID
#define PASSPORT_NUM_STR_LEN 9 // the strlen() of a passport number

#define MONTH_STR_SIZE 10      // the maximum array size of a month string (September is the longest string)
#define INITIAL_BUFFER_SIZE 16 // the initial number of bytes allocated for a string

#define MIN_PASSENGERS_PER_FLIGHT 1         // the minimum number of passengers allowed for a flight
#define MAX_AGE 130                         // the maximum age allowed for a passenger
#define MIN_FLIGHT_DURATION_IN_MINUTES 1    // the minimum flight duration allowed for a flight
#define MAX_FLIGHT_DURATION_IN_MINUTES 1130 // the maximum flight duration allowed for a flight (from Singapore (SIN) to New York (JFK))
#define VIEW_FLIGHTS_LINEAR_AVAILABLE 1     // the mode for viewing available flights in view_flights_linear()
#define VIEW_FLIGHTS_LINEAR_REMOVABLE 2     // the mode for viewing removable flights in view_flights_linear()
#define VIEW_FLIGHTS_LINEAR_ALL 3           // the mode for viewing all flights in view_flights_linear()

#define FLIGHTS_FILE "flights.txt"       // the string representing the file where flights are saved
#define PASSENGERS_FILE "passengers.txt" // the string representing the file where passengers are saved
#define MAX_BACKUPS 2                    // the maximum number of backups allowed
#define MAX_FILENAME_LENGTH 256          // the maximum length of a filename

const char *MONTHS[12] = {"January", "February", "March", "April", // an array of month strings
                          "May", "June", "July", "August",
                          "September", "October", "November", "December"};

/* Structures */

typedef struct Date // A structure for a Date with the day, month, and year components.
{
    int day;                    // An int for the Day component of a Date.
    char month[MONTH_STR_SIZE]; // A str for the Month component of a Date.
    int year;                   // An int for the Year component of a Date.
} Date;

typedef struct Time // A structure for a Time with the hours and minutes components.
{
    int hours;   // An int for the Hours component of a Time.
    int minutes; // An int for the Minutes component of a Time.
} Time;

typedef struct DateTime // A structure for a DateTime with a Date and Time.
{
    struct Date date; // A Date struct for the Date component
    struct Time time; // A Time struct for the Time component
} DateTime;

typedef struct Flight // A structure for a Flight with Flight details.
{
    char *flight_id;           // A str for the Flight's flight_id
    char *destination;         // A str for the Flight's destination country
    char *origin;              // A str for the Flight's origin country
    struct DateTime departure; // A DateTime for the Flight's departure
    struct DateTime arrival;   // A DateTime for the Flight's arrival
    int passenger_qty;         // An int for the Flight's current number of passengers
    int passenger_max;         // An int for the Flight's maximum number of passengers allowed
    int bonus_miles;           // An int for the number of bonus miles a passenger gets for booking the flight
    struct Flight *prev;       // A ptr to the previous Flight node
    struct Flight *next;       // A ptr to the next Flight node
} Flight;

typedef struct Reservation // A structure for a Reservation with a Flight.
{
    struct Flight *flight;    // A ptr to the Flight reserved
    struct Reservation *prev; // A ptr to the previous Reservation node
    struct Reservation *next; // A ptr to the next Reservation node
} Reservation;

typedef struct Passenger // A structure for a Passenger with passenger details.
{
    char *first_name;                 // A str for the Passenger's first name
    char *last_name;                  // A str for the Passenger's last name
    struct Date birth_date;           // A Date for the Passenger's birth date
    char *passport_number;            // A str for the Passenger's passport number
    int miles;                        // An int for the Passenger's number of miles accumulated
    int reservation_qty;              // An int for the Passenger's number of reservations
    struct Reservation *reservations; // A ptr to the Passenger's (linked) list of reservations
    struct Passenger *prev;           // A ptr to the previous Passenger node
    struct Passenger *next;           // A ptr to the next Passenger node
} Passenger;

typedef struct FlightStatus
{
    bool flight_departed; // A boolean indicating if a flight has departed.
    bool flight_arrived;  // A boolean indicating if a flight has arrived.
} FlightStatus;

/* General Helper Functions */

int main_menu();                   // Prints a menu and returns an integer of the user's choice
bool confirm_delete(char *prompt); // Asks the user if they are sure of deleting data
void clean_exit();                 // Cleanly exits the program when there is no more memory left
void update_current_datetime();    // Updates the current date and time

/* String Helper Functions */

char *toupper_string(char *string);    // Converts a string to uppercase
char *capitalize_string(char *string); // Capitalizes a string

/* Flight Status Functions */

FlightStatus retrieve_flight_status(Flight *flight); // Gets the status of a flight

/* Comparison Helper Functions */

int flight_compare(Flight *a, Flight *b);          // Compares two flights
int passenger_compare(Passenger *a, Passenger *b); // Compares two passengers

/* Validation Helper Functions */

bool is_valid_day(int day, char *month, int year); // Checks if a day is valid
bool is_valid_month(char *input);                  // Checks if a month is valid
bool is_valid_nonempty_string(char *string);       // Checks if a string is valid (nonempty)
bool is_valid_id(char *id);                        // Checks if an ID is valid
bool is_valid_passport(char *passport_number);     // Checks if a passport is valid

/* DateTime Helper Functions */

int month_to_int(char *month);                                        // Converts a month string to its number
long long datetime_to_minutes(DateTime dt);                           // Converts a DateTime to minutes
int days_in_month(char *month, int year);                             // Gets the number of days in a month
DateTime compute_arrival_datetime(DateTime departure, Time duration); // Gets an arrival DateTime from a departure DateTime and duration

/* DateTime Check Helper Functions */

bool is_conflicting(DateTime departure1, DateTime arrival1, // Checks if a DateTime is in the bounds of another DateTime
                    DateTime departure2, DateTime arrival2);
bool is_leap(int year);                         // Checks if a year is a leap year
bool is_future(DateTime date1, DateTime date2); // Checks if a DateTime is in the future (from another DateTime)
bool is_valid_birthdate(Date birth_date);       // Checks if a birth date is valid

/* Input Functions */

char *get_string(char *prompt, FILE *stream);  // Prompts a user for a string or gets a string from a file (with automatic dynamic memory allocation)
int get_int(char *prompt);                     // Prompts a user for an integer
Date get_date(char *prompt);                   // Prompts a user for a Date (for DateTimes, birthdays)
Time get_time(char *prompt, bool is_duration); // Prompts a user for a Time (for DateTimes, durations)
DateTime get_departure_datetime(char *prompt); // Prompts a user for a departure DateTime

/* Free Memory Functions */

void free_flight_node(Flight *node);            // Frees memory allocated for a Flight node
void free_passenger_node(Passenger *node);      // Frees memory allocated for a Passenger node
void free_flights_list(Flight *head);           // Frees all memory allocated for the flights linked list
void free_passengers_list(Passenger *head);     // Frees all memory allocated for the passengers linked list
void free_reservations_list(Reservation *head); // Frees all memory allocated for the reservations linked list

/* Flight Linked List Functions */

Flight *create_flight_node();                              // Creates a Flight node
Flight *search_flight_node(Flight *head, char *flight_id); // Searches for a Flight in the linked list and returns a pointer to it
void insert_flight_node(Flight **head, Flight *node);      // Insert a Flight to the linked list considering order
void reinsert_flight_node(Flight **head, Flight *node);    // Reinserts a Flight to the linked list (ensures order of data is maintained after editing a node)
void delete_flight_node(Flight **head, char *flight_id);   // Deletes a Flight from the linked list
int count_flights(Flight *head);                           // Counts the number of Flights in the linked list

/* Passenger Linked List Functions */

Passenger *create_passenger_node();                                       // Creates a Passenger Node
Passenger *search_passenger_node(Passenger *head, char *passport_number); // Searches for a Passenger in the linked list and returns a pointer to it
void insert_passenger_node(Passenger **head, Passenger *node);            // Insert a Passenger to the linked list considering order
int count_passengers(Passenger *head);                                    // Counts the number of Passengers in the linked list

/* Reservation Linked List Functions */

Reservation *create_reservation_node(Flight *flight);                    // Creates a Reservation Node
Reservation *search_reservation_node(Reservation *head, Flight *flight); // Searches for a Reservation in the linked list and returns a pointer to it
void insert_reservation_node(Reservation **head, Reservation *node);     // Insert a Reservation to the linked list considering order
void delete_reservation_node(Reservation **head, Flight *flight);        // Deletes a Reservation from the linked list
int count_reservations(Reservation *head);                               // Counts the number of Reservations in the linked list

/* Program Functionality */

void add_flight(Flight **head);                             // Adds a Flight to the database
void edit_flight(Flight **head);                            // Edits a Flight in the database
int view_flights_menu();                                    // Prints the menu for viewing flights and returns an integer
void view_flights(Flight *head, int mode);                  // Views flights in the database
void delete_flight(Flight **f_head, Passenger *p_head);     // Deletes a Flight from the database
void add_passenger(Passenger **head);                       // Adds a passenger to the database
void edit_passenger(Passenger *head);                       // Edits a passenger in the database
void book_reservation(Flight *f_head, Passenger *p_head);   // Books a Flight reservation for a passenger
void remove_reservation(Flight *f_head, Passenger *p_head); // Removes a Flight reservation from a passenger
void view_reservations(Passenger *p_head);                  // Views all reservations of a passenger

/* Linear View / Print Functions */

bool view_flights_linear(Flight *head, int mode); // Views flights in the database (linear format), returns false if no flights can be printed
void view_passengers_linear(Passenger *head);     // Views passengers in the database (linear format)
void view_reservations_linear(Reservation *head); // Views reservations for a passenger (linear format)
void print_flight(Flight *flight);                // Prints all details of a Flight
void print_passenger(Passenger *passenger);       // Prints all details of a Passenger

/* File Functions */

void load_flights(Flight **f_head, FILE *fp);                           // Load flights from a file
void load_passengers(Flight *f_head, Passenger **p_head, FILE *fp);     // Load passengers from a file
void load(Flight **f_head, Passenger **p_head);                         // Load flights and passengers
void save_flights(Flight *f_head, FILE *fp);                            // Save flights to a file
void save_passengers(Passenger *p_head, FILE *fp);                      // Save passengers to a file
void save(Flight *f_head, Passenger *p_head);                           // Save flights and passengers
void create_backups(char *flights_filename, char *passengers_filename); // Create backups of files
bool file_exists(char *filename);                                       // Check if a file exists

/* Global Linked Lists */

Flight *flights = NULL;       // Global Flights Linked List
Passenger *passengers = NULL; // Global Passengers Linked List

/* Global Time */

DateTime current_datetime; // Global Current DateTime

int main()
{
    // Variables
    int choice, view_choice;

    // Load Flights and Passengers from files
    load(&flights, &passengers);

    // Main Program Loop
    do
    {
        // Print the menu and ask the user for a choice
        choice = main_menu();

        printf("\n");

        // Call the function based on the user's choice
        switch (choice)
        {
        // Add Flight
        case 1:
            add_flight(&flights);
            break;

        // Edit Flight
        case 2:
            edit_flight(&flights);
            break;

        // View Flights
        case 3:
            // Enter a submenu loop
            do
            {
                // Print the menu and ask the user for a choice
                view_choice = view_flights_menu();
                printf("\n");
                switch (view_choice)
                {
                // Pass the view_choice to view_flights
                case 1:
                case 2:
                case 3:
                case 4:
                    view_flights(flights, view_choice);
                    break;
                // Return to the main menu
                case 0:
                    break;
                default:
                    printf(RED "Oops! Please enter a valid choice.\n\n" RESET);
                    break;
                }
            } while (view_choice != 0);
            break;

        // Delete Flight
        case 4:
            delete_flight(&flights, passengers);
            break;

        // Add Passenger
        case 5:
            add_passenger(&passengers);
            break;

        // Edit Passenger
        case 6:
            edit_passenger(passengers);
            break;

        // Book Reservation
        case 7:
            book_reservation(flights, passengers);
            break;

        // Remove Reservation
        case 8:
            remove_reservation(flights, passengers);
            break;

        // View Reservation
        case 9:
            view_reservations(passengers);
            break;

        // Exit
        case 0:
            // Save Flights and Passengers to Files
            save(flights, passengers);

            // Print a Goodbye message
            printf(YELLOW "Goodbye!\n\n" RESET);
            break;

        // Invalid Choice
        default:
            printf(RED "Oops! Please enter a valid choice.\n\n" RESET);
            break;
        }
    } while (choice != 0);

    // Free allocated memory
    free_flights_list(flights);
    free_passengers_list(passengers);

    return 0;
}

int main_menu()
{
    // Variables
    int choice;

    // Set the current date when the main menu starts
    update_current_datetime();

    // Print the current_datetime
    printf(B_CYAN "============== Date and Time ==============\n" RESET);
    printf(" %d %s %d %02d:%02d\n",
           current_datetime.date.day, current_datetime.date.month, current_datetime.date.year,
           current_datetime.time.hours, current_datetime.time.minutes);

    // Print the main menu
    printf(B_CYAN "================== Menu ===================\n" RESET);
    printf(" 1 | Add Flight\n");
    printf(" 2 | Edit Flight\n");
    printf(" 3 | View Flights\n");
    printf(" 4 | Delete Flight\n");
    printf(" 5 | Add Passenger\n");
    printf(" 6 | Edit Passenger Details\n");
    printf(" 7 | Book Flight Reservation\n");
    printf(" 8 | Remove Flight Reservation\n");
    printf(" 9 | View Reservations\n");
    printf(" 0 | Exit\n\n");

    // Ask the user for an integer
    choice = get_int("Enter choice: ");

    return choice;
}

bool confirm_delete(char *prompt)
{
    // Variable
    char *choice;

    // Ask the user for a string
    choice = get_string(prompt, stdin);

    // Check if the user only entered "Y"
    if (strcmp(choice, "Y") == 0 || strcmp(choice, "y") == 0)
    {
        free(choice);
        return true; // Return true if input matches "Y"
    }
    else
    {
        printf(BLUE "\nInfo: Deletion failed. User entered '%s'.\n" RESET, choice);
        free(choice);
        return false; // Return false otherwise
    }
}

void clean_exit()
{
    // Free allocated memory from linked lists
    free_flights_list(flights);
    free_passengers_list(passengers);

    // Exit with a failure status
    exit(EXIT_FAILURE);
}

void update_current_datetime()
{
    // Initialize a time_t variable with the current time and set the struct tm pointer to the local time
    time_t now = time(NULL);
    struct tm *time_now = localtime(&now);

    // Initialize a Date from the struct tm's year, month, and day
    Date c_date = {
        .year = (time_now->tm_year + 1900), // tm_year => years since 1900
        .day = time_now->tm_mday,
    };
    strncpy(c_date.month, MONTHS[time_now->tm_mon], MONTH_STR_SIZE);

    // Initialize a Time from the struct tm's hours and minutes
    Time c_time = {
        .hours = time_now->tm_hour,
        .minutes = time_now->tm_min,
    };

    // Set the current_datetime to the current date and time
    current_datetime = (DateTime){
        .date = c_date,
        .time = c_time,
    };
}

char *toupper_string(char *string)
{
    if (string != NULL)
    {
        for (int i = 0; string[i] != '\0'; i++)
        {
            if (islower(string[i]))
            {
                string[i] = toupper(string[i]); // Convert lowercase letters to uppercase
            }
        }
    }
    return string;
}

char *capitalize_string(char *string)
{
    if (string != NULL)
    {
        for (int i = 0; string[i] != '\0'; i++)
        {
            switch (i)
            {
            case 0: // Convert the first character to uppercase
                string[i] = toupper(string[i]);
                break;
            default: // Convert all other characters to lowercase
                if (string[i - 1] != ' ')
                {
                    string[i] = tolower(string[i]);
                }
                else
                {
                    string[i] = toupper(string[i]);
                }
                break;
            }
        }
    }
    return string;
}

FlightStatus retrieve_flight_status(Flight *flight)
{
    // Variable
    FlightStatus status;

    // Update the current_datetime
    update_current_datetime();

    // Check if the current datetime is NOT in the future of the flight's departure (current_datetime <= departure)
    status.flight_departed = !is_future(current_datetime, flight->departure);

    // Check if the current_datetime is in the future of the flight's arrival (arrival < current_datetime)
    status.flight_arrived = is_future(flight->arrival, current_datetime);

    return status;
}

int flight_compare(Flight *a, Flight *b)
{
    // Convert DateTime to minutes
    long long a_departure_in_minutes = datetime_to_minutes(a->departure);
    long long b_departure_in_minutes = datetime_to_minutes(b->departure);

    // Sort by Departure
    if (a_departure_in_minutes - b_departure_in_minutes != 0)
    {
        return a_departure_in_minutes - b_departure_in_minutes; // Return an integer indicating the order
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

bool is_valid_day(int day, char *month, int year)
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

bool is_valid_month(char *input)
{
    for (int i = 0; i < 12; i++)
    {
        if (strcmp(input, MONTHS[i]) == 0)
        {
            return true; // Return true if input is in the MONTHS array
        }
    }
    return false; // Return false otherwise
}

bool is_valid_nonempty_string(char *string)
{
    if (strlen(string) == 0)
    {
        return false; // Return false if the string is empty
    }
    return true; // Return true otherwise
}

bool is_valid_id(char *id)
{
    if (strlen(id) == 0)
    {
        return false; // Return false if the id is length 0 (no id)
    }
    if (strlen(id) > FLIGHT_ID_STR_LEN)
    {
        return false; // Return false if the id length exceeds the maximum length FLIGHT_ID_STR_LEN
    }
    for (int i = 0; id[i] != '\0'; i++)
    {
        if (!isalnum(id[i]))
        {
            return false; // Return false if the id contains non-alphanumeric characters
        }
    }
    return true; // Return true otherwise
}

bool is_valid_passport(char *passport_number)
{
    if (strlen(passport_number) != PASSPORT_NUM_STR_LEN)
    {
        return false; // Return false if the passport number length is not equal to PASSPORT_NUM_STR_LEN
    }
    for (int i = 0; passport_number[i] != '\0'; i++)
    {
        if (!isalnum(passport_number[i]))
        {
            return false; // Return false if the passport number contains non-alphanumeric characters
        }
    }
    return true; // Return true otherwise
}

int month_to_int(char *month)
{
    for (int i = 0; i < 12; i++)
    {
        // Check if the input matches any month in the array
        if (strcmp(month, MONTHS[i]) == 0)
        {
            return i + 1; // Returns the month number of a month string
        }
    }
    return -1; // Return -1 if the month is not found
}

long long datetime_to_minutes(DateTime dt)
{
    // Convert each DateTime component to minutes, get the sum, then return the result
    return dt.date.year * 525600 +
           month_to_int(dt.date.month) * 43800 +
           dt.date.day * 1440 +
           dt.time.hours * 60 +
           dt.time.minutes;
}

int days_in_month(char *month, int year)
{
    // Check if the month is February
    if (strcmp(month, "February") == 0)
    {
        // If the year is a leap year
        if (is_leap(year))
        {
            return 29; // Return 29 days if the year is a leap year and the month is February
        }
        else
        {
            return 28; // Return 28 days if the year is not a leap year and the month is February
        }
    }
    // Check if the month is April, June, September, or November
    else if (strcmp(month, "April") == 0 || strcmp(month, "June") == 0 ||
             strcmp(month, "September") == 0 || strcmp(month, "November") == 0)
    {
        return 30; // Return 30 days if the month is April, June, September, or November
    }
    else
    {
        return 31; // Return 31 days if the month for all other months
    }
}

DateTime compute_arrival_datetime(DateTime departure, Time duration)
{
    // Initialize arrival DateTime
    DateTime arrival = departure;
    arrival.time.hours += duration.hours;     // Add the duration to the hours
    arrival.time.minutes += duration.minutes; // Add the duration to the minutes

    // Normalize the minutes
    while (arrival.time.minutes >= 60) // If the minutes exceed 59
    {
        arrival.time.minutes -= 60; // Subtract 60 minutes
        arrival.time.hours++;       // Add an hour
    }
    // Normalize the hours
    while (arrival.time.hours >= 24) // If the hours exceed 23

    {
        arrival.time.hours -= 24; // Subtract 24 hours
        arrival.date.day++;       // Add a day
    }
    // Normalize the days
    while (arrival.date.day > days_in_month(arrival.date.month, arrival.date.year)) // If the day exceeds the number of days in the month
    {
        arrival.date.day -= days_in_month(arrival.date.month, arrival.date.year); // Subtract the number of days in the month of the current month
        if (month_to_int(arrival.date.month) == 12)                               // If the month is currently December
        {
            arrival.date.year++; // Increment the year
        }
        strncpy(arrival.date.month, MONTHS[month_to_int(arrival.date.month) % 12], MONTH_STR_SIZE); // Increment the month
    }

    return arrival; // Return the arrival DateTime
}

bool is_conflicting(DateTime departure1, DateTime arrival1, DateTime departure2, DateTime arrival2)
{
    // Convert DateTime to minutes
    long long departure1_in_minutes = datetime_to_minutes(departure1);
    long long arrival1_in_minutes = datetime_to_minutes(arrival1);
    long long departure2_in_minutes = datetime_to_minutes(departure2);
    long long arrival2_in_minutes = datetime_to_minutes(arrival2);

    // Check if the DateTime is in the bounds of another DateTime
    if ((departure1_in_minutes <= arrival2_in_minutes) && (arrival1_in_minutes >= departure2_in_minutes))
    {
        return true; // Return true if the DateTime is in the bounds of another DateTime
    }
    else
    {
        return false; // Return false otherwise
    }
}

bool is_leap(int year)
{
    if (year % 4 != 0) // A leap year must be divisible by 4...
    {
        return false;
    }
    if (year % 100 != 0) // ...but must not be divisible by 100...
    {
        return true;
    }
    if (year % 400 == 0) // ...unless it is divisible by 400
    {
        return true;
    }
    return false;
}

bool is_future(DateTime date1, DateTime date2)
{
    // Check if the date2 DateTime is in the future compared to the date1 DateTime
    return datetime_to_minutes(date2) > datetime_to_minutes(date1);
}

bool is_valid_birthdate(Date birth_date)
{
    // Check if the age of the person is less than or equal to MAX_AGE
    return current_datetime.date.year - birth_date.year <= MAX_AGE;
}

char *get_string(char *prompt, FILE *stream)
{
    // Variables
    int buffer_length = INITIAL_BUFFER_SIZE, c, i = 0;
    char *temp;

    // Print prompt if there is a prompt
    if (prompt != NULL)
    {
        printf("%s", prompt);
    }

    // Allocate memory for a string
    char *buffer = (char *)malloc(sizeof(char) * buffer_length);
    if (buffer == NULL) // If malloc failed
    {
        printf(RED "Oops! Memory allocation failed.\n\n" RESET);
        clean_exit();
    }

    while ((c = fgetc(stream)) != '\n' && c != EOF) // Read characters from the file stream one by one
    {
        // Fill the buffer with the characters from the file stream
        buffer[i++] = c;

        // If we've reached the end of the buffer, reallocate more memory
        if (i == buffer_length)
        {
            buffer_length *= 2;                                           // Increase size of buffer
            temp = (char *)realloc(buffer, sizeof(char) * buffer_length); // Reallocate memory
            if (temp == NULL)                                             // If realloc failed.
            {
                printf(RED "Oops! Memory allocation failed.\n\n" RESET);
                free(buffer);
                clean_exit();
            }
            else
            {
                buffer = temp; // Point the buffer to the new memory (if it ever moved)
            }
        }
    }

    // Null-terminate the string
    buffer[i] = '\0';

    // Shrink the buffer to the actual size
    temp = (char *)realloc(buffer, sizeof(char) * (i + 1));
    if (temp == NULL) // If realloc fails
    {
        printf(RED "Oops! Memory allocation failed.\n\n" RESET);
        free(buffer);
        clean_exit();
    }
    buffer = temp; // The buffer is resized to the optimal size

    return buffer; // Return the string
}

int get_int(char *prompt)
{
    // Variables
    int number;
    char *input = NULL;
    char *endptr = NULL;

    // Repeatedly ask for an integer until it is valid
    do
    {
        // Free previous input from the previous loop
        if (input != NULL)
        {
            free(input);
        }
        // Ask the user for a number in string form
        input = get_string(prompt, stdin);

        // Convert the string to an integer
        number = strtol(input, &endptr, 10);

        // Check for invalid invalid input
        if (*endptr != '\0' || input[0] == '\n' || endptr == input || number < 0)
        {
            printf(RED "Oops! Please enter a valid positive integer.\n" RESET);
        }
    } while (*endptr != '\0' || input[0] == '\n' || endptr == input || number < 0);
    free(input); // Free the string

    return number; // Return the integer
}

Date get_date(char *prompt)
{
    // Variables
    Date new_date;
    char *input = NULL;
    bool month_is_valid = false, day_is_valid = false;

    // Print prompt if there is a prompt
    if (prompt != NULL)
    {
        printf("\n%s\n\n", prompt);
    }

    // Ask for a month
    do
    {
        input = capitalize_string(get_string("Month:   ", stdin));
        month_is_valid = is_valid_month(input);
        if (!month_is_valid)
        {
            printf(RED "Oops! Invalid month. Please type a valid month name.\n" RESET);
        }
        else
        {
            strncpy(new_date.month, input, MONTH_STR_SIZE);
        }
        free(input);
    } while (!month_is_valid);

    // Ask for a day
    do
    {
        new_date.day = get_int("Day:     ");
        day_is_valid = is_valid_day(new_date.day, new_date.month, new_date.year);
        if (!day_is_valid)
        {
            printf(RED "Oops! Invalid day. Please type a valid day.\n" RESET);
        }
    } while (!day_is_valid);

    // Ask for a year
    new_date.year = get_int("Year:    ");

    return new_date;
}

Time get_time(char *prompt, bool is_duration)
{
    // Variables
    Time new_time;
    bool is_valid_duration = false;

    // If we are getting a duration, repeatedly ask for a duration until it is valid
    do
    {
        // Print prompt if there is a prompt
        if (prompt != NULL)
        {
            printf("\n%s\n\n", prompt);
        }

        // Ask for hours
        do
        {
            new_time.hours = get_int((!is_duration) ? "Hours (00-23):   " : "Hours (00-18):   ");
            if (new_time.hours < 0 || new_time.hours > 23)
            {
                printf(RED "Oops! Hours are not in range (0-23).\n" RESET);
            }
        } while (new_time.hours < 0 || new_time.hours > 23);

        // Ask for minutes
        do
        {
            new_time.minutes = get_int("Minutes (00-59): ");
            if (new_time.minutes < 0 || new_time.minutes > 59)
            {
                printf(RED "Oops! Minutes are not in range (0-59).\n" RESET);
            }
        } while (new_time.minutes < 0 || new_time.minutes > 59);

        // If we are getting a duration
        if (is_duration)
        {
            // Check if the duration is valid
            if (new_time.hours * 60 + new_time.minutes >= MIN_FLIGHT_DURATION_IN_MINUTES && new_time.hours * 60 + new_time.minutes <= MAX_FLIGHT_DURATION_IN_MINUTES)
            {
                is_valid_duration = true;
            }
            else
            {
                printf(RED "Oops! Duration is not in range (00:01 - 18:50).\n" RESET);
            }
        }
    } while ((is_duration && !is_valid_duration));

    return new_time;
}

DateTime get_departure_datetime(char *prompt)
{
    // Variables
    Date new_date;
    Time new_time;
    DateTime new_datetime;
    bool datetime_is_future = false;

    // Update the current datetime
    update_current_datetime();

    do
    {
        // Print prompt if there is a prompt
        if (prompt != NULL)
        {
            printf("\n%s\n\n", prompt);
            printf("Current: ");
            printf("%d %s %d %02d:%02d\n\n",
                   current_datetime.date.day, current_datetime.date.month, current_datetime.date.year,
                   current_datetime.time.hours, current_datetime.time.minutes);
        }
        // Ask for a Date (Year, Month, Day)
        new_date = get_date(NULL);

        // Ask for Time (Hours, Minutes)
        new_time = get_time(B_CYAN "--- Departure Time --------------------" RESET, false);

        // Reupdate the current datetime
        update_current_datetime();

        // Check if the DateTime is in the future
        datetime_is_future = is_future(current_datetime, (DateTime){.date = new_date, .time = new_time});

        if (!datetime_is_future)
        {
            printf(B_CYAN "\n---------------------------------------\n\n" RESET);
            printf(RED "Oops! Departure must be after current date and time.\n" RESET);
            printf(BLUE "Current:   ");
            printf("%d %s %d %02d:%02d\n" RESET,
                   current_datetime.date.day, current_datetime.date.month, current_datetime.date.year,
                   current_datetime.time.hours, current_datetime.time.minutes);
            printf(RED "Departure: ");
            printf("%d %s %d %02d:%02d\n" RESET,
                   new_date.day, new_date.month, new_date.year,
                   new_time.hours, new_time.minutes);
        }
    } while (!datetime_is_future);

    // Set the DateTime components using the acquired Date and Time
    new_datetime = (DateTime){.date = new_date, .time = new_time};

    return new_datetime;
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
        free_reservations_list(node->reservations);

        // Free the node
        free(node);
    }
}

void free_flights_list(Flight *head)
{
    // Free every node in the linked list
    while (head != NULL)
    {
        Flight *temp = head;
        head = head->next;
        free_flight_node(temp);
    }
}

void free_passengers_list(Passenger *head)
{
    // Free every node in the linked list
    while (head != NULL)
    {
        Passenger *temp = head;
        head = head->next;
        free_passenger_node(temp);
    }
}

void free_reservations_list(Reservation *head)
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
        printf(RED "Oops! Memory allocation failed.\n" RESET);
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
        .prev = NULL,
        .next = NULL};

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
            return curr; // Return the pointer to the node if found
        }
        curr = curr->next;
    }
    return NULL; // Return NULL if not found
}

void insert_flight_node(Flight **head, Flight *node)
{
    // Start from the head
    Flight *curr = *head;

    // If the list is empty OR the new node is before the head in order
    if (*head == NULL || flight_compare(node, *head) < 0)
    {
        node->next = *head;
        if (*head != NULL)
        {
            (*head)->prev = node;
        }
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
        if (curr->next != NULL)
        {
            curr->next->prev = node;
        }
        curr->next = node;
        node->prev = curr;
    }
}

void reinsert_flight_node(Flight **head, Flight *node)
{
    // If the node to delete is the head
    if (*head == node)
    {
        *head = (*head)->next;
        if (*head != NULL)
        {
            (*head)->prev = NULL;
        }
    }
    else
    {
        if (node->next != NULL)
        {
            node->next->prev = node->prev;
        }
        if (node->prev != NULL)
        {
            node->prev->next = node->next;
        }
    }

    // Reset the node's pointers
    node->prev = NULL;
    node->next = NULL;

    // Reinsert the node
    insert_flight_node(&(*head), node);
}

void delete_flight_node(Flight **head, char *flight_id)
{
    // Start from the head
    Flight *curr = *head;

    // If the node to delete is the head
    if (strcmp((*head)->flight_id, flight_id) == 0)
    {
        *head = (*head)->next;

        free_flight_node(curr);
        return;
    }

    // Find the node to delete
    while (curr != NULL && strcmp(curr->flight_id, flight_id) != 0)
    {
        curr = curr->next;
    }

    // If the node was not found
    if (curr == NULL)
    {
        printf(RED "Oops! That Flight does not exist.\n\n" RESET);
        return;
    }

    // Remove the node from the list
    if (curr->next != NULL)
    {
        curr->next->prev = curr->prev;
    }
    if (curr->prev != NULL)
    {
        curr->prev->next = curr->next;
    }
    free_flight_node(curr); // Free the deleted node
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
        printf(RED "Oops! Memory allocation failed.\n\n" RESET);
        clean_exit();
    }

    // Initialize fields to defaults
    *new_passenger = (Passenger){
        .first_name = NULL,
        .last_name = NULL,
        .birth_date = (Date){.day = 0, .month = "", .year = 0},
        .passport_number = NULL,
        .reservation_qty = 0,
        .miles = 0,
        .prev = NULL,
        .next = NULL};

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
            return curr; // Return the pointer to the node
        }
        curr = curr->next;
    }

    return NULL; // Return NULL if node does not exist
}

void insert_passenger_node(Passenger **head, Passenger *node)
{
    // Start from the head
    Passenger *curr = *head;

    // If the list is empty OR the new node is before the head in order
    if (*head == NULL || passenger_compare(node, *head) < 0)
    {
        node->next = *head;
        if (node->next != NULL)
        {
            node->next->prev = node;
        }
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
        if (node->next != NULL)
        {
            node->next->prev = node;
        }
        curr->next = node;
        node->prev = curr;
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
        printf(RED "Oops! Memory allocation failed.\n\n" RESET);
        clean_exit();
    }

    // Initialize field with the Flight
    *new_reservation = (Reservation){
        .flight = flight, .prev = NULL, .next = NULL};

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
            return curr; // Return the pointer to the node
        }
        curr = curr->next;
    }
    return NULL; // Return NULL if node does not exist
}

void insert_reservation_node(Reservation **head, Reservation *node)
{
    // Start from the head
    Reservation *curr = *head;

    // If the list is empty OR the new node is before the *head in order
    if (*head == NULL || flight_compare(node->flight, (*head)->flight) < 0)
    {
        node->next = *head;
        if (node->next != NULL)
        {
            node->next->prev = node;
        }
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
        if (node->next != NULL)
        {
            node->next->prev = node;
        }
        curr->next = node;
        node->prev = curr;
    }
}

void delete_reservation_node(Reservation **head, Flight *flight)
{
    // Start from the head
    Reservation *curr = *head;

    if ((*head)->flight == flight) // If the node to delete is the head
    {
        *head = (*head)->next;
        if (*head != NULL)
        {
            (*head)->prev = NULL;
        }
        free(curr);
        return;
    }

    // Find the node to delete
    while (curr->flight != flight) // Find the node to delete
    {
        curr = curr->next;
    }

    // If the node was not found
    if (curr == NULL)
    {
        printf(RED "Oops! Reservation does not exist.\n\n" RESET);
        return;
    }
    // Remove the node from the linked list
    if (curr->next != NULL)
    {
        curr->next->prev = curr->prev;
    }
    if (curr->prev != NULL)
    {
        curr->prev->next = curr->next;
    }
    free(curr); // Free the deleted node
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
    // Variables
    char *flight_id = NULL;
    Time duration;
    bool string_is_valid = false, dest_is_origin = true;

    printf(B_CYAN "== Add Flight =============================\n\n" RESET);

    // Ask for Flight ID and validate
    flight_id = toupper_string(get_string("Flight ID:   ", stdin));
    if (!is_valid_id(flight_id))
    {
        printf(RED "Oops! A valid Flight ID has at least 1 and at most 6 uppercase letters and/or digits only.\n\n" RESET);
        free(flight_id);
        return;
    }
    if (search_flight_node(*head, flight_id) != NULL)
    {
        printf(RED "Oops! That Flight already exists.\n\n" RESET);
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

        new_flight->origin = capitalize_string(get_string("Origin:      ", stdin));
        string_is_valid = is_valid_nonempty_string(new_flight->origin);

        if (!string_is_valid)
        {
            printf(RED "Oops! Origin cannot be empty.\n" RESET);
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

        new_flight->destination = capitalize_string(get_string("Destination: ", stdin));
        string_is_valid = is_valid_nonempty_string(new_flight->destination);
        if (!string_is_valid)
        {
            printf(RED "Oops! Destination cannot be empty.\n" RESET);
        }
        if ((dest_is_origin = strcmp(new_flight->destination, new_flight->origin) == 0))
        {
            printf(RED "Oops! Destination cannot be the same as the origin.\n" RESET);
        }
    } while (!string_is_valid || dest_is_origin);

    // Ask for Departure DateTime
    new_flight->departure = get_departure_datetime(B_CYAN "--- Departure Date --------------------" RESET);

    // Ask for Duration
    duration = get_time(B_CYAN "--- Duration --------------------------" RESET, "duration");

    // Compute for the Arrival DateTime
    new_flight->arrival = compute_arrival_datetime(new_flight->departure, duration);

    // Number of Booked Passengers is already set to 0

    // Ask for Max Count of Passengers (Seats)
    printf(B_CYAN "\n---------------------------------------\n\n" RESET);
    do
    {
        new_flight->passenger_max = get_int("Max Seats:   ");
        if (new_flight->passenger_max < MIN_PASSENGERS_PER_FLIGHT)
        {
            printf(RED "Oops! The maximum number of passengers must be at least %d.\n" RESET, MIN_PASSENGERS_PER_FLIGHT);
        }
    } while (new_flight->passenger_max < MIN_PASSENGERS_PER_FLIGHT);

    // Ask for Bonus Miles
    do
    {
        new_flight->bonus_miles = get_int("Bonus Miles: ");
        if (new_flight->bonus_miles < 0)
        {
            printf(RED "Oops! The bonus miles must be at least 0.\n" RESET);
        }
    } while (new_flight->bonus_miles < 0);

    // Insert the Flight to the linked list
    insert_flight_node(&(*head), new_flight);

    // Print a success message
    printf(GREEN "\nSuccess: Added Flight %s.\n\n" RESET, new_flight->flight_id);
}

void edit_flight(Flight **head)
{
    // Variables
    char *flight_id;
    Flight *f_ptr = NULL;
    Time duration;
    FlightStatus status;

    printf(B_CYAN "== Edit Flight ============================\n\n" RESET);

    // If there are no flights to edit
    if (*head == NULL)
    {
        printf(BLUE "Info: There are currently no flights.\n\n" RESET);
        return;
    }

    // Print flights (in linear form)
    view_flights_linear(*head, VIEW_FLIGHTS_LINEAR_ALL);

    // Ask for a Flight ID and validate
    flight_id = toupper_string(get_string("Flight ID: ", stdin));
    if (!is_valid_id(flight_id))
    {
        printf(RED "\nOops! A valid Flight ID has at least 1 and at most 6 uppercase letters and/or digits only.\n\n" RESET);
        free(flight_id);
        return;
    }
    if ((f_ptr = search_flight_node(*head, flight_id)) == NULL)
    {
        printf(RED "\nOops! That Flight does not exist.\n\n" RESET);
        free(flight_id);
        return;
    }
    free(flight_id);

    status = retrieve_flight_status(f_ptr);
    if (status.flight_departed && !status.flight_arrived)
    {
        printf(RED "\nOops! That Flight is currently ongoing.\n\n" RESET);
        return;
    }
    else if (status.flight_departed && status.flight_arrived)
    {
        printf(RED "\nOops! That Flight has already arrived.\n\n" RESET);
        return;
    }

    printf(B_CYAN "\n--- Flight Details --------------------\n\n" RESET);
    print_flight(f_ptr);

    // New Departure DateTime
    f_ptr->departure = get_departure_datetime(B_CYAN "--- Departure Date --------------------" RESET);

    // New Duration (for a new Arrival DateTime)
    duration = get_time(B_CYAN "--- Duration --------------------------" RESET, "duration");

    // Compute for the new Arrival DateTime
    f_ptr->arrival = compute_arrival_datetime(f_ptr->departure, duration);

    // New Max Passengers (Seats)
    printf(B_CYAN "\n---------------------------------------\n\n" RESET);
    do
    {
        f_ptr->passenger_max = get_int("Max Seats: ");
        if (f_ptr->passenger_max < MIN_PASSENGERS_PER_FLIGHT)
        {
            printf(RED "Oops! The maximum number of passengers must be at least %d.\n" RESET, MIN_PASSENGERS_PER_FLIGHT);
        }
        else if (f_ptr->passenger_max < f_ptr->passenger_qty)
        {
            printf(RED "Oops! You cannot decrease maximum passengers below number of reserved passengers.\n" RESET);
        }
    } while (f_ptr->passenger_max < MIN_PASSENGERS_PER_FLIGHT || f_ptr->passenger_max < f_ptr->passenger_qty);

    reinsert_flight_node(&(*head), f_ptr);

    printf(GREEN "\nSuccess: Edited Flight %s.\n\n" RESET, f_ptr->flight_id);
}

int view_flights_menu()
{
    int choice;

    printf(B_CYAN "============== View Flights ===============\n" RESET);
    printf(" 1 | View Specific Flight\n");
    printf(" 2 | View All Available Flights\n");
    printf(" 3 | View All Fully-booked Flights\n");
    printf(" 4 | View All Flights\n");
    printf(" 0 | Back\n\n");
    choice = get_int("Enter choice: ");

    return choice;
}

void view_flights(Flight *head, int mode)
{
    // Variables
    int count = 0;
    Flight *ptr = head;
    char *flight_id;
    bool flights_exist, should_print;
    FlightStatus status;

    switch (mode)
    {
    case 1: // Mode 1: View Specific Flight
        printf(B_CYAN "--- View Flights > Specific -----------\n\n" RESET);

        // Print flights (in linear form)
        flights_exist = view_flights_linear(head, VIEW_FLIGHTS_LINEAR_ALL);
        if (!flights_exist)
        {
            return;
        }

        // Ask for a Flight ID and Validate
        flight_id = toupper_string(get_string("Flight ID: ", stdin));
        printf("\n");
        if (!is_valid_id(flight_id))
        {
            printf(RED "Oops! A valid Flight ID has at least 1 and at most 6 uppercase letters and/or digits only.\n\n" RESET);
            free(flight_id);
            return;
        }
        if ((ptr = search_flight_node(head, flight_id)) == NULL)
        {
            printf(RED "Oops! That Flight does not exist.\n\n" RESET);
            free(flight_id);
            return;
        }
        free(flight_id);

        // Print the flight
        print_flight(ptr);
        printf("\n");

        return;
    case 2: // Mode 2: View Available Flights
        printf(B_CYAN "--- View Flights > Available ----------\n\n" RESET);
        break;
    case 3: // Mode 3: View Full Flights
        printf(B_CYAN "--- View Flights > Fully-Booked -------\n\n" RESET);
        break;
    case 4: // Mode 4: View All Flights
        printf(B_CYAN "--- View Flights > All ----------------\n\n" RESET);
        break;
    default:
        printf(RED "Oops! Please enter a valid choice.\n\n" RESET);
        break;
    }

    while (ptr != NULL)
    {
        should_print = false;

        // Retrieve the Flight Status
        status = retrieve_flight_status(ptr);

        switch (mode)
        {
        case 2: // Mode 2: View Available Flights
            should_print = !status.flight_departed && !status.flight_arrived && ptr->passenger_qty < ptr->passenger_max;
            break;
        case 3: // Mode 3: View Full Flights
            should_print = ptr->passenger_qty == ptr->passenger_max;
            break;
        case 4: // Mode 4: View All Flights
            should_print = true;
            break;
        }

        if (should_print)
        {
            print_flight(ptr);
            printf("\n");
            count++;
        }

        ptr = ptr->next;
    }

    if (count == 0)
    {
        printf(BLUE "Info: There are currently no flights that fit the criteria.\n\n" RESET);
    }
}

void delete_flight(Flight **f_head, Passenger *p_head)
{
    // Variables
    Flight *f_ptr;
    Passenger *p_ptr;
    bool removable_flights_exist;
    FlightStatus status;

    printf(B_CYAN "== Delete Flight ==========================\n\n" RESET);

    // If there are no flights to delete
    if (*f_head == NULL)
    {
        printf(BLUE "Info: There are currently no flights.\n\n" RESET);
        return;
    }

    // Print flights (in linear form)
    removable_flights_exist = view_flights_linear(*f_head, VIEW_FLIGHTS_LINEAR_REMOVABLE);
    if (!removable_flights_exist)
    {
        return;
    }

    // Ask for a Flight ID and validate
    char *flight_id = toupper_string(get_string("Flight ID: ", stdin));
    printf("\n");
    if (!is_valid_id(flight_id))
    {
        printf(RED "Oops! A valid Flight ID has at least 1 and at most 6 uppercase letters and/or digits only.\n\n" RESET);
        free(flight_id);
        return;
    }
    if ((f_ptr = search_flight_node(*f_head, flight_id)) == NULL)
    {
        printf(RED "Oops! That Flight does not exist.\n\n" RESET);
        free(flight_id);
        return;
    }
    free(flight_id);

    // Retrieve the status of the flight
    status = retrieve_flight_status(f_ptr);

    if (!status.flight_arrived && f_ptr->passenger_qty != 0)
    {
        printf(RED "Oops! Passengers have already booked this Flight.\n\n" RESET);
        return;
    }

    // Confirm deletion of Flight
    if (confirm_delete("Do you want to delete this Flight? (y/n): "))
    {
        if (status.flight_arrived)
        {
            p_ptr = p_head;
            while (p_ptr != NULL)
            {
                if (search_reservation_node(p_ptr->reservations, f_ptr) != NULL)
                {
                    f_ptr->passenger_qty--;
                    p_ptr->reservation_qty--;
                    printf(BLUE "Info: Removed finished flight %s for %s.\n" RESET,
                           f_ptr->flight_id, p_ptr->first_name);
                    delete_reservation_node(&p_ptr->reservations, f_ptr);
                }
                p_ptr = p_ptr->next;
            }
        }
        printf(GREEN "Success: Deleted Flight %s.\n\n" RESET, f_ptr->flight_id);
        delete_flight_node(&(*f_head), f_ptr->flight_id);
    }
    else
    {
        printf(BLUE "Info: Did not delete Flight.\n\n" RESET);
    }
}

void add_passenger(Passenger **head)
{
    // Variables
    char *first_name = NULL;
    char *last_name = NULL;
    Date birth_date;
    char *passport_number = NULL;
    bool passport_is_valid, string_is_valid, birthdate_is_valid;

    printf(B_CYAN "== Add Passenger ==========================\n\n" RESET);

    // Ask for Name
    do
    {
        // Free previous input from the previous loop
        if (first_name != NULL)
        {
            free(first_name);
        }
        first_name = capitalize_string(get_string("First Name: ", stdin));
        string_is_valid = is_valid_nonempty_string(first_name);
        if (!string_is_valid)
        {
            printf(RED "Oops! First name cannot be empty.\n" RESET);
        }
    } while (!string_is_valid);
    do
    {
        // Free previous input from the previous loop
        if (last_name != NULL)
        {
            free(last_name);
        }
        last_name = capitalize_string(get_string("Last Name:  ", stdin));
        string_is_valid = is_valid_nonempty_string(last_name);
        if (!string_is_valid)
        {
            printf(RED "Oops! Last name cannot be empty.\n" RESET);
        }
    } while (!string_is_valid);

    // Ask for Birth Date
    do
    {
        // Update the current datetime before asking for the birth date
        update_current_datetime();

        birth_date = get_date(B_CYAN "--- Date of Birth ---------------------" RESET);
        printf(B_CYAN "\n---------------------------------------\n\n" RESET);
        birthdate_is_valid = is_valid_birthdate(birth_date);
        if (!birthdate_is_valid)
        {
            printf(RED "Oops! Please enter a valid birth date. The maximum age is %d.\n" RESET, MAX_AGE);
        }
    } while (!birthdate_is_valid);

    // Ask for Passport Number and validate
    do
    {
        // Free previous input from the previous loop
        if (passport_number != NULL)
        {
            free(passport_number);
        }
        passport_number = toupper_string(get_string("Passport Number: ", stdin));
        if (!(passport_is_valid = is_valid_passport(passport_number)))
        {
            printf(RED "\nOops! A valid passport number has 9 uppercase letters and/or digits.\n\n" RESET);
        }
        if (search_passenger_node(*head, passport_number) != NULL)
        {
            printf(RED "\nOops! Passenger with that passport number already exists.\n\n" RESET);
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
    new_passenger->birth_date = birth_date;
    new_passenger->passport_number = passport_number;

    // Ask for Number of Miles
    do
    {
        new_passenger->miles = get_int("Number of Miles: ");
        if (new_passenger->miles < 0)
        {
            printf(RED "Oops! The number of miles must be at least 0.\n" RESET);
        }
    } while (new_passenger->miles < 0);

    // Insert the Passenger to the linked list
    insert_passenger_node(*(&head), new_passenger);

    // Print success message
    printf(GREEN "\nSuccess: Added Passenger %s.\n\n" RESET, new_passenger->first_name);
}

void edit_passenger(Passenger *head)
{
    // Variable
    bool string_is_valid, birthdate_is_valid;

    printf(B_CYAN "== Edit Passenger =========================\n\n" RESET);

    // If there are no passengers to edit
    if (head == NULL)
    {
        printf(BLUE "Info: There are currently no passengers.\n\n" RESET);
        return;
    }

    // Print passengers (in linear form)
    view_passengers_linear(head);

    char *passport_number;   // Input string
    Passenger *p_ptr = NULL; // Pointer to the Passenger to edit

    // Ask for a Passport Number and validate
    passport_number = toupper_string(get_string("Passport Number: ", stdin));
    printf("\n");
    if (!is_valid_passport(passport_number))
    {
        printf(RED "Oops! A valid Passport Number has 9 uppercase letters and/or digits.\n\n" RESET);
        free(passport_number);
        return;
    }
    if ((p_ptr = search_passenger_node(head, passport_number)) == NULL)
    {
        printf(RED "Oops! That passenger does not exist.\n\n" RESET);
        free(passport_number);
        return;
    }
    free(passport_number);

    printf(B_CYAN "\n--- Passenger Details -----------------\n\n" RESET);
    print_passenger(p_ptr);
    printf(B_CYAN "\n---------------------------------------\n\n" RESET);

    // New Last Name
    do
    {
        if (p_ptr->last_name != NULL)
        {
            free(p_ptr->last_name);
        }
        p_ptr->last_name = capitalize_string(get_string("Last Name: ", stdin));
        string_is_valid = is_valid_nonempty_string(p_ptr->last_name);
        if (!string_is_valid)
        {
            printf(RED "Oops! Last name cannot be empty.\n\n" RESET);
        }
    } while (!string_is_valid);

    // New Birth Date
    do
    {
        // Update the current datetime before asking for the birth date
        update_current_datetime();

        p_ptr->birth_date = get_date(B_CYAN "--- Date of Birth ---------------------" RESET);
        printf(B_CYAN "\n---------------------------------------\n" RESET);
        birthdate_is_valid = is_valid_birthdate(p_ptr->birth_date);
        if (!birthdate_is_valid)
        {
            printf(RED "\nOops! Please enter a valid birth date. The maximum age is %d.\n" RESET, MAX_AGE);
        }
    } while (!birthdate_is_valid);

    printf(GREEN "\nSuccess: Edited Passenger %s.\n\n" RESET, p_ptr->first_name);
}

void book_reservation(Flight *f_head, Passenger *p_head)
{
    // Variables
    Passenger *passenger = NULL;
    Flight *flight = NULL;
    Reservation *reservation_ptr = NULL;
    Reservation *new_reservation = NULL;
    DateTime *d_ptr = NULL, *a_ptr = NULL;
    FlightStatus status;
    char *passport_number, *flight_id;
    bool available_flights_exists;

    printf(B_CYAN "== Book Reservation =======================\n\n" RESET);

    // If there are no flights to book and no passengers that can book
    if (f_head == NULL && p_head == NULL)
    {
        printf(BLUE "Info: There are currently no flights and passengers.\n\n" RESET);
        return;
    }
    // If there are no flights to book
    if (f_head == NULL)
    {
        printf(BLUE "Info: There are currently no flights.\n\n" RESET);
        return;
    }
    // If there are no passengers that can book
    if (p_head == NULL)
    {
        printf(BLUE "Info: There are currently no passengers.\n\n" RESET);
        return;
    }

    // Print passengers (in linear form)
    view_passengers_linear(p_head);

    // Ask for a Passport Number and validate
    passport_number = toupper_string(get_string("Passport Number: ", stdin));
    printf("\n");
    if (!is_valid_passport(passport_number))
    {
        printf(RED "Oops! A valid Passport Number has 9 uppercase letters and/or digits.\n\n" RESET);
        free(passport_number);
        return;
    }
    if ((passenger = search_passenger_node(p_head, passport_number)) == NULL)
    {
        printf(RED "Oops! That passenger does not exist.\n\n" RESET);
        free(passport_number);
        return;
    }
    free(passport_number);

    // Print flights (in linear form)
    available_flights_exists = view_flights_linear(f_head, VIEW_FLIGHTS_LINEAR_AVAILABLE);
    if (!available_flights_exists)
    {
        return;
    }

    // Ask for a Flight ID and validate
    flight_id = toupper_string(get_string("Flight ID: ", stdin));
    printf("\n");
    if (!is_valid_id(flight_id))
    {
        printf(RED "Oops! A valid Flight ID has at least 1 and at most 6 uppercase letters and/or digits only.\n\n" RESET);
        free(flight_id);
        return;
    }
    if ((flight = search_flight_node(f_head, flight_id)) == NULL)
    {
        printf(RED "Oops! That Flight does not exist.\n\n" RESET);
        free(flight_id);
        return;
    }

    // Retrieve the status of the flight
    status = retrieve_flight_status(flight);

    if (status.flight_departed)
    {
        printf(RED "Oops! That Flight has already departed.\n\n" RESET);
        free(flight_id);
        return;
    }
    if (flight->passenger_qty == flight->passenger_max)
    {
        printf(RED "Oops! That Flight is fully booked.\n\n" RESET);
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
            printf(RED "Oops! You already reserved this Flight.\n\n" RESET);
            return;
        }

        // DateTime pointers (to shorten the following code)
        d_ptr = &reservation_ptr->flight->departure;
        a_ptr = &reservation_ptr->flight->arrival;

        // If the Flight conflicts with the Reservation
        if (is_conflicting(*d_ptr, *a_ptr, flight->departure, flight->arrival))
        {

            printf(RED "Oops! That Flight conflicts with current reservations.\n" RESET);
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

    // Create a new Reservation node
    new_reservation = create_reservation_node(flight);

    // Insert the Reservation node to the passenger's reservations linked list
    insert_reservation_node(&passenger->reservations, new_reservation);

    // Update flight and passenger details
    flight->passenger_qty++;
    passenger->reservation_qty++;
    passenger->miles += flight->bonus_miles;

    // Print success message
    printf(GREEN "Success: Reserved Flight %s for %s.\n\n" RESET, flight->flight_id, passenger->first_name);
}

void remove_reservation(Flight *f_head, Passenger *p_head)
{
    // Variables
    char *passport_number, *flight_id;
    Passenger *passenger = NULL;
    Flight *flight = NULL;
    Reservation *r_ptr = NULL;
    FlightStatus status;

    printf(B_CYAN "== Remove Reservation =====================\n\n" RESET);

    // If there are no passengers to remove reservations from
    if (p_head == NULL)
    {
        printf(BLUE "Info: There are currently no passengers.\n\n" RESET);
        return;
    }

    // Print passengers (in linear form)
    view_passengers_linear(p_head);

    // Ask for a Passport Number and validate
    passport_number = toupper_string(get_string("Passport Number: ", stdin));
    printf("\n");
    if (!is_valid_passport(passport_number))
    {
        printf(RED "Oops! A valid Passport Number has 9 uppercase letters and/or digits.\n\n" RESET);
        free(passport_number);
        return;
    }
    if ((passenger = search_passenger_node(p_head, passport_number)) == NULL)
    {
        printf(RED "Oops! That passenger does not exist.\n\n" RESET);
        free(passport_number);
        return;
    }
    if (passenger->reservations == NULL)
    {
        printf(RED "Oops! That passenger has no reservations.\n\n" RESET);
        free(passport_number);
        return;
    }
    free(passport_number);

    // Print reservations (in linear form)
    view_reservations_linear(passenger->reservations);

    // Ask for a Flight ID and validate
    flight_id = toupper_string(get_string("Flight ID: ", stdin));
    printf("\n");
    if (!is_valid_id(flight_id))
    {
        printf(RED "Oops! A valid Flight ID has at least 1 and at most 6 uppercase letters and/or digits only.\n\n" RESET);
        free(flight_id);
        return;
    }
    if ((flight = search_flight_node(f_head, flight_id)) == NULL)
    {
        printf(RED "Oops! That Flight does not exist.\n\n" RESET);
        free(flight_id);
        return;
    }
    free(flight_id);

    // Search for the Reservation to delete
    if ((r_ptr = search_reservation_node(passenger->reservations, flight)) == NULL)
    {
        printf(RED "Oops! That reservation does not exist for that passenger.\n\n" RESET);
        return;
    }

    // Get the current status of the flight we are trying to remove
    status = retrieve_flight_status(flight);

    if (status.flight_departed && !status.flight_arrived)
    {
        printf(RED "Oops! You cannot remove an ongoing flight from your reservations.\n\n");
        return;
    }

    // Confirm deletion of Reservation
    if (confirm_delete("Do you want to delete this Reservation? (y/n): "))
    {
        // Update flight and passenger details
        r_ptr->flight->passenger_qty--;
        passenger->reservation_qty--;

        // If the flight has not yet departed, this removal is a cancellation of the flight.
        // We revoke the bonus miles gained from this reservation.
        if (!status.flight_departed)
        {
            passenger->miles -= r_ptr->flight->bonus_miles;
            printf(GREEN "Success: Cancelled reserved flight %s for %s.\n\n" RESET, r_ptr->flight->flight_id, passenger->first_name);
        }
        // If the flight has departed (and ended), this removal is not a cancellation of the flight.
        // We do not revoke the bonus miles gained from this reservation.
        else
        {
            printf(GREEN "Success: Removed finished flight %s for %s.\n\n" RESET, r_ptr->flight->flight_id, passenger->first_name);
        }

        delete_reservation_node(&passenger->reservations, r_ptr->flight);
    }
    else
    {
        printf(BLUE "Info: Did not delete Reservation.\n\n" RESET);
    }
}

void view_reservations(Passenger *head)
{
    // Variables
    char *passport_number;
    Passenger *p_ptr = NULL;
    Flight *f_ptr = NULL;
    Reservation *r_ptr = NULL;
    FlightStatus status;

    printf(B_CYAN "== View Reservations ======================\n\n" RESET);

    // If there are no passengers to view reservations from
    if (head == NULL)
    {
        printf(BLUE "Info: There are currently no passengers.\n\n" RESET);
        return;
    }

    // Print passengers (in linear form)
    view_passengers_linear(head);

    // Ask for a Passport Number and validate
    passport_number = toupper_string(get_string("Passport Number: ", stdin));
    printf("\n");
    if (!is_valid_passport(passport_number))
    {
        printf(RED "Oops! A valid Passport Number has 9 uppercase letters and/or digits.\n\n" RESET);
        free(passport_number);
        return;
    }
    if ((p_ptr = search_passenger_node(head, passport_number)) == NULL)
    {
        printf(RED "Oops! That passenger does not exist.\n\n" RESET);
        free(passport_number);
        return;
    }
    if (p_ptr->reservations == NULL)
    {
        printf(RED "Oops! That passenger has no reservations.\n\n" RESET);
        free(passport_number);
        return;
    }
    free(passport_number);

    printf(B_CYAN "--- Reservations ----------------------\n\n" RESET);

    // Start from the head of the passenger's reservation linked list
    r_ptr = p_ptr->reservations;
    // Traverse and print each reservation
    while (r_ptr != NULL)
    {
        print_flight(r_ptr->flight);
        printf("\n");
        r_ptr = r_ptr->next;
    }
}

bool view_flights_linear(Flight *head, int mode)
{
    // Variables
    int count = 0;
    int max_origin_length = 0, max_destination_length = 0;
    int max_departure_month_length = 0, max_arrival_month_length = 0;
    Flight *ptr = head;
    FlightStatus status;

    // First Pass: Get the maximum lengths of the strings for formatting
    while (ptr != NULL)
    {
        // Get the lengths of the strings
        int origin_length = strlen(ptr->origin);
        int destination_length = strlen(ptr->destination);
        int departure_month_length = strlen(ptr->departure.date.month);
        int arrival_month_length = strlen(ptr->arrival.date.month);

        // Check if we need to calculate the maximum lengths
        bool calculate_length = false;
        switch (mode)
        {
        case 1: // Mode 1: Available Flights (for Booking)
            status = retrieve_flight_status(ptr);
            if (ptr->passenger_qty < ptr->passenger_max && !status.flight_departed)
            {
                calculate_length = true;
            }
            break;
        case 2: // Mode 2: Empty/Removable Flights (for Deleting)
            status = retrieve_flight_status(ptr);
            if (ptr->passenger_qty == 0 || status.flight_arrived)
            {
                calculate_length = true;
            }
            break;
        case 3: // Mode 3: All Flights (for Viewing)
            calculate_length = true;
            break;
        }

        // Calculate the maximum lengths if it fits the criteria
        if (calculate_length)
        {
            if (max_origin_length < origin_length)
            {
                max_origin_length = origin_length;
            }
            if (max_destination_length < destination_length)
            {
                max_destination_length = destination_length;
            }
            if (max_departure_month_length < departure_month_length)
            {
                max_departure_month_length = departure_month_length;
            }
            if (max_arrival_month_length < arrival_month_length)
            {
                max_arrival_month_length = arrival_month_length;
            }
        }

        ptr = ptr->next;
    }

    switch (mode)
    {
    case 1:
        printf(B_CYAN "--- Available Flights -----------------\n\n" RESET);
        break;
    case 2:
        printf(B_CYAN "--- Empty/Removable Flights -----------\n\n" RESET);
        break;
    case 3:
        printf(B_CYAN "--- All Flights -----------------------\n\n" RESET);
        break;
    }

    // Second Pass: Print the flights
    ptr = head;
    while (ptr != NULL)
    {
        bool should_print = false;
        switch (mode)
        {
        case 1: // Mode 1: Available Flights (for Booking)
            status = retrieve_flight_status(ptr);
            if (ptr->passenger_qty < ptr->passenger_max && !status.flight_departed)
            {
                should_print = true;
            }
            break;
        case 2: // Mode 2: Empty/Removable Flights (for Deleting)

            // Get the current status of the flight
            status = retrieve_flight_status(ptr);

            // The flight is removable if there are no passengers or the flight has already arrived.
            if (ptr->passenger_qty == 0 || status.flight_arrived)
            {
                should_print = true;
            }
            break;
        case 3: // Mode 3: All Flights (for Viewing)
            should_print = true;
            break;
        }

        // Print the flight if it fits the criteria
        if (should_print)
        {
            printf("%6s | %-*s to %-*s | %02d %-*s %02d %02d:%02d - %02d %-*s %02d %02d:%02d\n",
                   ptr->flight_id, max_origin_length, ptr->origin, max_destination_length, ptr->destination,
                   ptr->departure.date.day, max_departure_month_length, ptr->departure.date.month, ptr->departure.date.year,
                   ptr->departure.time.hours, ptr->departure.time.minutes,
                   ptr->arrival.date.day, max_arrival_month_length, ptr->arrival.date.month, ptr->arrival.date.year,
                   ptr->arrival.time.hours, ptr->arrival.time.minutes);
            count++;
        }
        ptr = ptr->next;
    }

    // If no flights fit the criteria
    if (count == 0)
    {
        switch (mode)
        {
        case 1:
            printf(BLUE "Info: No flights are available.\n\n" RESET);
            break;
        case 2:
            printf(BLUE "Info: No flights are empty/removable.\n\n" RESET);
            break;
        case 3:
            printf(BLUE "Info: No flights are found.\n\n" RESET);
            break;
        }
        return false;
    }

    printf("\n");
    return true;
}

void view_passengers_linear(Passenger *head)
{
    // Variables
    Passenger *ptr = head;
    int max_name_length = 0;
    int max_reservation_qty_length = 0, max_miles_length = 0;

    // First Pass: Get the maximum lengths of the strings for formatting
    while (ptr != NULL)
    {
        int first_name_length = strlen(ptr->first_name);
        int last_name_length = strlen(ptr->last_name);
        int reservation_qty_length = snprintf(NULL, 0, "%d", ptr->reservation_qty);
        int miles_length = snprintf(NULL, 0, "%d", ptr->miles);

        if (max_name_length < first_name_length + last_name_length)
        {
            max_name_length = first_name_length + last_name_length;
        }
        if (max_reservation_qty_length < reservation_qty_length)
        {
            max_reservation_qty_length = reservation_qty_length;
        }
        if (max_miles_length < miles_length)
        {
            max_miles_length = miles_length;
        }
        ptr = ptr->next;
    }

    printf(B_CYAN "--- Passengers ------------------------\n\n" RESET);

    // Second Pass: Print the passengers
    ptr = head;
    while (ptr != NULL)
    {
        char *full_name = (char *)malloc(sizeof(char) * max_name_length + 3);
        snprintf(full_name, max_name_length + 3, "%s, %s", ptr->last_name, ptr->first_name);
        printf("%9s | %-*s | %*d flights reserved | %*d miles\n",
               ptr->passport_number,
               max_name_length + 2, full_name,
               max_reservation_qty_length, ptr->reservation_qty,
               max_miles_length, ptr->miles);
        free(full_name);
        ptr = ptr->next;
    }
    printf("\n");
}

void view_reservations_linear(Reservation *head)
{
    // Variables
    int max_origin_length = 0, max_destination_length = 0;
    int max_departure_month_length = 0, max_arrival_month_length = 0;
    Reservation *r_ptr = head;
    Flight *f_ptr = NULL;

    // First Pass: Get the maximum lengths of the strings for formatting
    while (r_ptr != NULL)
    {
        f_ptr = r_ptr->flight;

        int origin_length = strlen(f_ptr->origin);
        int destination_length = strlen(f_ptr->destination);
        int departure_month_length = strlen(f_ptr->departure.date.month);
        int arrival_month_length = strlen(f_ptr->arrival.date.month);

        if (max_origin_length < origin_length)
        {
            max_origin_length = origin_length;
        }
        if (max_destination_length < destination_length)
        {
            max_destination_length = destination_length;
        }
        if (max_departure_month_length < departure_month_length)
        {
            max_departure_month_length = departure_month_length;
        }
        if (max_arrival_month_length < arrival_month_length)
        {
            max_arrival_month_length = arrival_month_length;
        }

        r_ptr = r_ptr->next;
    }

    printf(B_CYAN "--- Reservations ----------------------\n\n" RESET);

    // Second Pass: Print the reservations
    r_ptr = head;
    f_ptr = NULL;
    while (r_ptr != NULL)
    {
        f_ptr = r_ptr->flight;
        printf("%6s | %-*s to %-*s | %02d %-*s %02d %02d:%02d - %02d %-*s %02d %02d:%02d\n",
               f_ptr->flight_id, max_origin_length, f_ptr->origin, max_destination_length, f_ptr->destination,
               f_ptr->departure.date.day, max_departure_month_length, f_ptr->departure.date.month, f_ptr->departure.date.year,
               f_ptr->departure.time.hours, f_ptr->departure.time.minutes,
               f_ptr->arrival.date.day, max_arrival_month_length, f_ptr->arrival.date.month, f_ptr->arrival.date.year,
               f_ptr->arrival.time.hours, f_ptr->arrival.time.minutes);
        r_ptr = r_ptr->next;
    }
    printf("\n");
}

void print_flight(Flight *flight)
{
    // Retrieve the Flight Status
    FlightStatus status = retrieve_flight_status(flight);

    printf(YELLOW "Flight ID: %s\n" RESET, flight->flight_id);
    printf("Flight:    %s to %s\n", flight->origin, flight->destination);

    if (status.flight_departed && !status.flight_arrived)
    {
        printf(RED "- Status:    Ongoing\n" RESET);
    }
    else if (status.flight_arrived)
    {
        printf(BLUE "- Status:    Arrived\n" RESET);
    }
    else if (flight->passenger_qty == flight->passenger_max)
    {
        printf(RED "- Status:    Fully-booked\n" RESET);
    }
    else
    {
        printf(GREEN "- Status:    Available\n" RESET);
    }

    printf("- Departure: %d %s %d - %02d:%02d\n",
           flight->departure.date.day, flight->departure.date.month, flight->departure.date.year,
           flight->departure.time.hours, flight->departure.time.minutes);
    printf("- Arrival:   %d %s %d - %02d:%02d\n",
           flight->arrival.date.day, flight->arrival.date.month, flight->arrival.date.year,
           flight->arrival.time.hours, flight->arrival.time.minutes);
    printf("Passengers:  %d\n", flight->passenger_qty);
    printf("Max Seats:   %d\n", flight->passenger_max);
    printf("Bonus Miles: %d\n", flight->bonus_miles);
}

void print_passenger(Passenger *passenger)
{
    Reservation *r_ptr = passenger->reservations;
    int count = 0;

    printf(YELLOW "Passport Number: %s\n" RESET, passenger->passport_number);
    printf("Name:         %s, %s\n" RESET, passenger->last_name, passenger->first_name);
    printf("Birth Date:   %02d %s %d\n" RESET, passenger->birth_date.day, passenger->birth_date.month, passenger->birth_date.year);
    printf("Reservations: %d\n" RESET, passenger->reservation_qty);
    printf("- ");
    while (r_ptr != NULL)
    {
        (r_ptr->next == NULL || (count % 5 == 0 && count != 0)) ? printf("%s\n", r_ptr->flight->flight_id) : printf("%s, ", r_ptr->flight->flight_id);
        count++;
        r_ptr = r_ptr->next;
    }
    printf("Miles:        %d\n" RESET, passenger->miles);
}

void load_flights(Flight **f_head, FILE *fp)
{
    // Variable
    int flight_count = 0;

    fscanf(fp, "%d\n", &flight_count); // Read the flight count
    if (flight_count != 0)             // If there are flights
    {
        while (!feof(fp)) // While there are flights to read
        {
            // Create a Flight node
            Flight *f_temp = create_flight_node();

            // Scan each field from the file
            f_temp->flight_id = get_string(NULL, fp);
            f_temp->origin = get_string(NULL, fp);
            f_temp->destination = get_string(NULL, fp);
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
    }
}

void load_passengers(Flight *f_head, Passenger **p_head, FILE *fp)
{
    // Variables
    int passenger_count = 0;
    Flight *reserved_flight;
    Reservation *r_ptr;

    fscanf(fp, "%d\n", &passenger_count); // Read the passenger count
    if (passenger_count == 0)             // If there are no passengers
    {
        return;
    }

    while (!feof(fp)) // While there are passengers to read
    {
        // Create a Passenger node
        Passenger *p_temp = create_passenger_node();

        // Scan each field from the file
        p_temp->last_name = get_string(NULL, fp);
        p_temp->first_name = get_string(NULL, fp);
        p_temp->passport_number = get_string(NULL, fp);
        fscanf(fp, "%d %s %d\n",
               &p_temp->birth_date.day,
               p_temp->birth_date.month,
               &p_temp->birth_date.year);
        fscanf(fp, "%d\n", &p_temp->reservation_qty);

        // Read the reservations
        for (int i = 0; i < p_temp->reservation_qty; i++)
        {
            // Get the Flight ID
            char *flight_id = get_string(NULL, fp);

            // Search for the flight
            reserved_flight = search_flight_node(f_head, flight_id);
            if (reserved_flight == NULL)
            {
                printf(RED "Oops! Critical error. Flight missing from flights.txt file.\nCannot continue.\n" RESET);
                printf(BLUE "Info: This may have happened because files were edited manually.\n\n" RESET);
                free_passenger_node(p_temp);
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
}

void load(Flight **f_head, Passenger **p_head)
{
    // Load Flights
    FILE *fp = fopen(FLIGHTS_FILE, "r");
    if (fp != NULL) // If the file exists
    {
        load_flights(&(*f_head), fp);
        fclose(fp);
    }

    // Load Passengers
    fp = fopen(PASSENGERS_FILE, "r");
    if (fp != NULL) // If both files exist
    {
        load_passengers(*f_head, &(*p_head), fp);
        fclose(fp);
    }
}

void save_flights(Flight *f_head, FILE *fp)
{
    // Variables
    int flight_count = count_flights(f_head);
    Flight *f_ptr = f_head;

    // Write the Flight count
    fprintf(fp, "%d\n", flight_count);

    // Print each Flight detail to the file
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
}

void save_passengers(Passenger *p_head, FILE *fp)
{
    // Variables
    int passenger_count = count_passengers(p_head);
    Passenger *p_ptr = p_head;
    Reservation *r_ptr = NULL;

    // Write the Passenger count
    fprintf(fp, "%d\n", passenger_count);

    // Write each Passenger detail to the file
    while (p_ptr != NULL)
    {
        fprintf(fp, "%s\n", p_ptr->last_name);
        fprintf(fp, "%s\n", p_ptr->first_name);
        fprintf(fp, "%s\n", p_ptr->passport_number);
        fprintf(fp, "%d %s %d\n",
                p_ptr->birth_date.day, p_ptr->birth_date.month, p_ptr->birth_date.year);
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
}

void save(Flight *f_head, Passenger *p_head)
{
    // Move files one tier up
    create_backups(FLIGHTS_FILE, PASSENGERS_FILE);

    // Save Flights
    FILE *flights_fp = fopen(FLIGHTS_FILE, "w");
    if (flights_fp != NULL)
    {
        save_flights(f_head, flights_fp);
        fclose(flights_fp);
    }

    // Save Passengers
    FILE *passengers_fp = fopen(PASSENGERS_FILE, "w");
    if (passengers_fp != NULL)
    {
        save_passengers(p_head, passengers_fp);
        fclose(passengers_fp);
    }
}

void create_backups(char *flights_filename, char *passengers_filename)
{
    // Variables
    char old_flights_filename[MAX_FILENAME_LENGTH], old_passengers_filename[MAX_FILENAME_LENGTH];
    char new_flights_filename[MAX_FILENAME_LENGTH], new_passengers_filename[MAX_FILENAME_LENGTH];

    // Shift the backups up by one
    for (int i = MAX_BACKUPS - 1; i > 0; i--)
    {
        snprintf(old_flights_filename, MAX_FILENAME_LENGTH, "backup_%d_%s", i, FLIGHTS_FILE);
        snprintf(old_passengers_filename, MAX_FILENAME_LENGTH, "backup_%d_%s", i, PASSENGERS_FILE);
        snprintf(new_flights_filename, MAX_FILENAME_LENGTH, "backup_%d_%s", i + 1, FLIGHTS_FILE);
        snprintf(new_passengers_filename, MAX_FILENAME_LENGTH, "backup_%d_%s", i + 1, PASSENGERS_FILE);

        if (file_exists(old_flights_filename) && file_exists(old_passengers_filename))
        {
            if (rename(old_flights_filename, new_flights_filename) != 0)
            {
                printf(RED "Error moving %s to %s\n" RESET, old_flights_filename, new_flights_filename);
                return;
            }
            if (rename(old_passengers_filename, new_passengers_filename) != 0)
            {
                printf(RED "Error moving %s to %s\n" RESET, old_passengers_filename, new_passengers_filename);
                return;
            }
        }
    }

    // flights.txt, passengers.txt -> backup_1
    if (file_exists(flights_filename) && file_exists(passengers_filename))
    {
        if (rename(flights_filename, "backup_1_flights.txt") != 0)
        {
            printf(RED "Error moving flights.txt to backup_1_flights.txt" RESET);
            return;
        }
        if (rename(passengers_filename, "backup_1_passengers.txt") != 0)
        {
            printf(RED "Error moving passengers.txt to backup_1_passengers.txt" RESET);
            return;
        }
    }
}

bool file_exists(char *filename)
{
    // Check if the file exists
    FILE *fp = fopen(filename, "r");
    if (fp != NULL)
    {
        fclose(fp);
        return true;
    }
    return false;
}