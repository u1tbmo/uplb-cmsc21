# Project Progress

## Program Functionality Requirements

### Structure Definitions

- [x] Flights
- [x] Passengers
- [x] Reservations

### Time

- [x] Program retrieves system time

### Save Load Functionality

- [x] Loads from Files
  - [x] Creates the files if they do not exist.
- [x] Saves to Files

### Main Program Functionality

- [x] Add Flight
  - [x] Disallow invalid flight IDs
  - [x] Disallow duplicate IDs
  - [x] Disallow dates in the past
- [x] Edit Flight
  - [x] Error if flight does not exist
  - [x] Disallow editing if the flight is ongoing
  - [x] Disallow editing if the flight has arrived/ended
  - [x] Disallow lowering the max passengers allowed below the number of already reserved passengers
  - [x] Editing ensures order of data
- [x] View Flights
  - [x] Multiple choices through a menu
  - [x] View Specific Flight
  - [x] View Available Flights
  - [x] View Empty Flights
  - [x] View All Flights
- [x] Delete Flight
  - [x] Immediately terminate upon no empty/removable flights.
  - [x] Disallow deletion of booked flights if the flight has not yet arrived.
  - [x] Allow deletion of booked flights if the flight has arrived.
    - [x] Upon deleting a finished flight, delete all reservations of that finished flight.
- [x] Add Passenger
  - [x] Disallow invalid passport numbers
  - [x] Disallow duplicate passports
- [x] Edit Passenger
- [x] Book Reservation
  - [x] Terminates if there no available flights exist.
  - [x] Disallow booking a departed flight.
  - [x] Disallow booking a fully-booked flight.
  - [x] Disallow booking the same flight more than once.
  - [x] Disallow booking a flight that conflicts with current reservations.
- [x] Remove Flight Reservation
  - [x] Revokes bonus miles if the flight has been removed before the flight (cancelling the reservation)
  - [x] Does not revoke bonus miles if the flight has been removed after the flight.
- [x] View Reservations

### General Restrictions

- [x] Strings cannot be empty
- [x] Integers cannot be negative
- [x] Dates must be valid

## Other Program Additions

### Quality of Life

- [x] Capitalize in full on input
  - [x] Flight ID
  - [x] Passport Number
- [x] Capitalize on input
  - [x] Names
  - [x] Months
  - [x] Locations

### Order of Data

- [x] Flights are sorted by departure, source, then destination
- [x] Passengers are sorted in alphabetical order

### Possible Improvements

- [x] Colored UI
- [x] Compute for the Arrival DateTime instead of asking for it from the user.
  - [x] Replace asking for an arrival datetime with asking for a flight duration in Add and Edit.
- [x] Cleaner Linear View Functions

### Memory

- [x] Free all memory if functions like `malloc()` or `realloc()` fail.
  - [x] Immediately exit with `EXIT_FAILURE` if the program cannot run anymore due to lack of memory.
