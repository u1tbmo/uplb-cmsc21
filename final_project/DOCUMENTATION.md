# Code Documentation

## Table of Contents

- [Code Documentation](#code-documentation)
  - [Table of Contents](#table-of-contents)
  - [Structures](#structures)
    - [Date](#date)
    - [Time](#time)
    - [DateTime](#datetime)
    - [Flight](#flight)
    - [Reservation](#reservation)
    - [Passenger](#passenger)
    - [FlightStatus](#flightstatus)
  - [Main Program Functionality](#main-program-functionality)
    - [`void add_flight(Flight **head)`](#void-add_flightflight-head)
    - [`void edit_flight(Flight **head)`](#void-edit_flightflight-head)
    - [`void view_flights(Flight *head, int mode)`](#void-view_flightsflight-head-int-mode)
      - [Mode 1: View Specific Flight](#mode-1-view-specific-flight)
      - [Mode 2: View Available Flights](#mode-2-view-available-flights)
      - [Mode 3: View Fully-Booked Flights](#mode-3-view-fully-booked-flights)
      - [Mode 4: View All Flights](#mode-4-view-all-flights)
    - [`void delete_flight(Flight **f_head, Passenger *p_head)`](#void-delete_flightflight-f_head-passenger-p_head)
    - [`void add_passenger(Passenger **head)`](#void-add_passengerpassenger-head)
    - [`void edit_passenger(Passenger *head)`](#void-edit_passengerpassenger-head)
    - [`void book_reservation(Flight *f_head, Passenger *p_head)`](#void-book_reservationflight-f_head-passenger-p_head)
    - [`void remove_reservation(Flight *f_head, Passenger *p_head)`](#void-remove_reservationflight-f_head-passenger-p_head)
    - [`void view_reservations(Passenger *head)`](#void-view_reservationspassenger-head)

## Structures

### Date

```c
typedef struct Date
{
    int day;
    char month[MONTH_STR_SIZE];
    int year;
} Date;
```

### Time

```c
typedef struct Time
{
    int hours;
    int minutes;
} Time;
```

### DateTime

```c
typedef struct DateTime
{
    struct Date date;
    struct Time time;
} DateTime;
```

### Flight

```c
typedef struct Flight
{
    char *flight_id;
    char *destination;
    char *origin;
    struct DateTime departure;
    struct DateTime arrival;
    int passenger_qty;
    int passenger_max;
    int bonus_miles;
    struct Flight *prev;
    struct Flight *next;
} Flight;
```

### Reservation

```c
typedef struct Reservation
{
    struct Flight *flight;
    struct Reservation *prev;
    struct Reservation *next;
} Reservation;
```

### Passenger

```c
typedef struct Passenger
{
    char *first_name;
    char *last_name;
    struct Date birth_date;
    char *passport_number;
    int miles;
    int reservation_qty;
    struct Reservation *reservations;
    struct Passenger *prev;
    struct Passenger *next;
} Passenger;
```

### FlightStatus

```c
typedef struct FlightStatus
{
    bool flight_departed;
    bool flight_arrived;
} FlightStatus;
```

## Main Program Functionality

### `void add_flight(Flight **head)`

Creates a new Flight and inserts that new Flight to a linked list.

1. Ask the user for the Flight's ID.
   - If the input `flight_id` is not valid or if the input `flight_id` already exists. Print an Error and .
2. Ask the user for the Flight's origin.
   - If the input `origin` is empty,the function asks for another input.
3. Ask the user for the Flight's destination.
   - If the input `destination` is empty, the function asks for another input.
4. Ask the user for the Flight's departure date and time.
   - The function asks the `year`, the `month`, and the `day` of `departure`. Invalid inputs prompt the user for another input. The function also ensures that the `departure` must be in the future of the current date and time.
5. Ask the user for a Flight duration.
   - If the input is less than `MIN_FLIGHT_DURATION_IN_MINUTES` and more than `MAX_FLIGHT_DURATION_IN_MINUTES` minutes, the function asks for another input.
6. The function then computes the `arrival` date and time of the Flight.
7. Ask the user for the Flight's maximum number of passengers.
   - If the input `passenger_max` is less than `MIN_PASSENGERS_PER_FLIGHT`, the function asks for another input.
8. Ask the user for the number of bonus miles the Flight gives to passengers who take that Flight.
   - If the input `bonus_miles` is less than 0, the function asks for another input.
9. Insert this Flight to the linked list.
10. Print a Success message and terminate.

---

### `void edit_flight(Flight **head)`

Edits a currently existing Flight in a linked list.

---

1. Print all Flights in the linked list in linear form.
2. Ask the user for the Flight's ID.
   - If the input `flight_id` is not valid or if the input `flight_id` does not exist. Print an Error and terminate.
3. Retrieve the status of the Flight.
   - If the Flight has already departed and/or arrived, print an Error and terminate.
4. Ask the user for the Flight's new departure.
   - The function asks the `year`, the `month`, and the `day` of `departure`. Invalid inputs prompt the user for another input. The function also ensures that the `departure` must be in the future of the current date and time.
5. Ask the user for a new Flight duration.
   - If the input is less than `MIN_FLIGHT_DURATION_IN_MINUTES` and more than `MAX_FLIGHT_DURATION_IN_MINUTES` minutes, the function asks for another input.
6. The function then computes the `arrival` date and time of the Flight.
7. Ask the user for the Flight's new maximum number of passengers.
   - If the input `passenger_max` is less than `MIN_PASSENGERS_PER_FLIGHT`, the function asks for another input.
8. The function then reinserts the Flight into the linked list.
   - This ensures that the linked list is sorted correctly.
9. Print a Success message and terminate.

---

### `void view_flights(Flight *head, int mode)`

Views all Flights in a linked list (depending on the mode parameter).

Depending on the mode parameter, this function can either view a specific Flight, or available Flights, or fully-booked Flights, or all Flights.

#### Mode 1: View Specific Flight

1. Print all Flights in the linked list in linear form.
2. Ask the user for the Flight's ID.
   - If the input `flight_id` is not valid or if the input `flight_id` does not exist, print an Error and terminate.
3. Print the flight's details.

#### Mode 2: View Available Flights

1. Prints all Flights in the linked list that are not full and have not yet departed.
   - If no flights were printed, the function prints an Info message saying so.

#### Mode 3: View Fully-Booked Flights

1. Prints all Flights in the linked list that are full.
   - If no flights, were printed, the function prints an Info message saying so.

#### Mode 4: View All Flights

1. Prints all flights in the linked list.

---

### `void delete_flight(Flight **f_head, Passenger *p_head)`

1. Print all empty/removable flights in the linked list in linear form.
2. Ask the user for the Flight's ID.
   - If the input `flight_id` is not valid or if the input `flight_id` does not exist. Print an Error and terminate.
3. Retrieve the `status` of the Flight.
   - If the Flight has been booked and has not yet arrived, print an Error and terminate.
4. Ask the user to confirm the deletion.
   - If confirmation fails, print an Info message and terminate.
5. Delete this flight reservation from every passenger who reserved this flight.
6. Delete the flight from the linked list.
7. Print a Success message and terminate.

---

### `void add_passenger(Passenger **head)`

1. Ask for a `first_name` and `last_name`.
   - Invalid inputs prompt the user for another input.
2. Ask for a `birth_date`.
3. Ask for a Passport Number.
   - If the `passport_number` is invalid, the function prints an Error message and asks for another input.
   - If the `passport_number` already exists in the database, print an Error message and terminate.
4. Ask for the number of `miles` accumulated (if there are any).
5. Insert this Passenger to the linked list.
6. Print a Success message and terminate.

---

### `void edit_passenger(Passenger *head)`

Edits a currently existing Passenger in a linked list.

1. Print all Passengers in linear form.
2. Ask the user for a passport number and search the Passenger with that passport number.
   - If the input `passport_number` is not valid or if the Passenger with `passport_number` does not exist, print an Error and terminate.
3. Ask for a new `last_name`.
   - Invalid inputs prompt the user for another input.
4. Ask for a new `birth_date`
   - Invalid inputs prompt the user for another input.
5. Edit the details of the Passenger.
6. Print a Success message and terminate.

---

### `void book_reservation(Flight *f_head, Passenger *p_head)`

Books a Flight reservation for a Passenger.

1. Print all Passengers in linear form.
2. Ask the user for a passport number and search the Passenger with that passport number.
   - If the input `passport_number` is not valid or if the Passenger with `passport_number` does not exist, print an Error and terminate.
3. Print all available Flights in linear form.
   - If there are no available Flights, print an Error message and terminate.
4. Ask the user for the Flight's ID.
   - If the input `flight_id` is not valid or if the input `flight_id` does not exist, print an Error and terminate.
5. Retrieve the `status` of the Flight.
   - If the Flight has already departed, print an Error and terminate.
   - If the Flight is full, print an Error and terminate.
6. Traverse the Passenger's `reservations` linked list and check if the Flight can be booked.
   - If the Flight has already been reserved, print an Error and terminate.
   - If the Flight conflicts with another Flight's time schedule, print an Error and terminate.
7. Insert the reservation into the Passenger's `reservations` linked list.
8. Update the Flight's `passenger_qty` and Passenger's `reservation_qty` and `miles`.
9. Print a Success message and terminate.

---

### `void remove_reservation(Flight *f_head, Passenger *p_head)`

Removes or cancels a booked Reservation from a Passenger.

1. Print all Passengers in linear form.
2. Ask the user for a passport number and search the Passenger with that passport number.
   - If the input `passport_number` is not valid or if the Passenger with `passport_number` does not exist, print an Error and terminate.
   - If the searched Passenger does not have any `reservations`, print an Error and terminate.
3. Print all of the Passenger's Reservations in linear form.
4. Ask the user for the Flight's ID.
   - If the input `flight_id` is not valid or if the input `flight_id` does not exist, print an Error and terminate.
   - If the Flight has not been reserved by that Passenger, print an Error and terminate.
5. Retrieve the `status` of the Flight.
   - If the flight is ongoing, print an Error and terminate.
6. Ask the user to confirm the deletion.
   - If confirmation fails, the function prints an Info message and terminates.
7. If the Flight is being removed before the `departure`, revoke the `bonus_miles` gained by reserving the Flight.
   - Print a Success message saying "Cancelled reserved flight."
8. If the Flight is being removed after the `arrival`, do not revoke `bonus_miles`.
   - Print a Success message saying "Removed finished flight."
9. Delete the Reservation from the linked list.
10. Terminate.

---

### `void view_reservations(Passenger *head)`

Views all Reservations of a Passenger.

1. Print all Passengers in linear form.
2. Ask the user for a passport number and search the Passenger with that passport number.
   - If the input `passport_number` is not valid or if the Passenger with `passport_number` does not exist, print an Error and terminate.
   - If the searched Passenger does not have any `reservations`, print an Error and terminate.
3. Print all reservations of the Passenger.
