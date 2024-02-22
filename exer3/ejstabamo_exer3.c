/*  Tabamo, Euan Jed S. - ST2L - Feb 20, 2024
    Exercise 3 - Maze Bank ATM
*/

#include <stdio.h>
#include <stdbool.h>

// Define Constants
// https://www.geeksforgeeks.org/c-define-preprocessor/
#define MENU_EXIT_OPTION 6
#define MAX_NAME_LENGTH 50

// Function Prototypes
void print_header();
void print_main_menu();
void request_pin(int current_pin);
void check_balance(int current_pin, float balance, char *account_name);
float deposit(int current_pin, float balance);
float withdraw(int current_pin, float balance);
int change_pin(int current_pin);

int main()
{
    // Variables
    int choice = 0;
    float balance = 500;
    char account_name[MAX_NAME_LENGTH] = "savings";
    int current_pin = 1234;

    // Print the header and a hello message
    print_header();
    printf("Hello!\n");

    // Request the PIN
    request_pin(current_pin);

    // Initiate the loop
    while (choice != MENU_EXIT_OPTION)
    {
        // Print header and menu options
        print_header();
        print_main_menu();

        // Ask for a choice
        printf("***************************\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        // Clear the input buffer
        // https://www.geeksforgeeks.org/clearing-the-input-buffer-in-cc/
        while ((getchar()) != '\n')
            ;

        printf("\n");

        // Call the appropriate function based on the choice (or exit the program)
        switch (choice)
        {
        case 1:
            check_balance(current_pin, balance, account_name);
            break;
        case 2:
            balance = deposit(current_pin, balance);
            break;
        case 3:
            balance = withdraw(current_pin, balance);
            break;
        case 4:
            current_pin = change_pin(current_pin);
            break;
        case 5:
            // Request for a PIN
            request_pin(current_pin);

            // Ask for a new account name
            printf("Enter new account name: ");

            // Modifying the number of chars scanf() reads
            // https://stackoverflow.com/a/12306624
            scanf("%49s", account_name);

            // Clear the input buffer
            while ((getchar()) != '\n')
                ;

            printf("---------------------------\n");
            printf("Account Name: %s\n", account_name);
            printf("---------------------------\n\n");
            break;
        case 6:
            printf("---------------------------\n");
            printf("Bye!\n");
            printf("---------------------------\n\n");
            break;

        default:
            printf("ERROR: Unknown choice!\n");
            break;
        }
    }
}

// Prints the header "Welcome to Maze Bank!"
void print_header()
{
    printf("***************************\n");
    printf("*  Welcome to Maze Bank!  *\n");
    printf("***************************\n");
}

// Prints the main menu options
void print_main_menu()
{
    printf("1 | Check Balance          \n");
    printf("2 | Deposit                \n");
    printf("3 | Withdraw               \n");
    printf("4 | Change PIN             \n");
    printf("5 | Set Account Name       \n");
    printf("6 | End Transaction        \n");
}

// Prompts the user for a PIN
void request_pin(int current_pin)
{
    // Ask for a PIN until it is correct.
    int input_pin;
    printf("Please enter your 4-digit PIN\n(user) ");
    do
    {
        scanf("%d", &input_pin);

        // Clear the input buffer
        while ((getchar()) != '\n')
            ;

        if (input_pin < 1000 || input_pin > 9999)
        {
            printf("ERROR: PIN must have 4 digits.\n");
        }
        else if (input_pin != current_pin)
        {
            printf("You entered an incorrect PIN. Please try again!\n(user) ");
        }
    } while (input_pin != current_pin);
    printf("\n");

    return;
}

// Checks the current balance
void check_balance(int current_pin, float balance, char *account_name)
{
    // Request for a PIN
    request_pin(current_pin);

    // Show output
    printf("---------------------------\n");
    printf("Account Name: %s\n", account_name);
    printf("Balance: %.2f\n", balance);
    printf("---------------------------\n\n");

    return;
}

// Asks for a deposit amount and returns the updated balance
float deposit(int current_pin, float balance)
{
    float deposit_amt;

    // Request for a PIN
    request_pin(current_pin);

    // Ask for amount to deposit
    printf("Deposit Amount: ");
    scanf("%f", &deposit_amt);

    // Clear the input buffer
    while ((getchar()) != '\n')
        ;

    // Show and return the updated balance
    printf("---------------------------\n");
    printf("New Balance: %.2f\n", balance + deposit_amt);
    printf("---------------------------\n\n");
    return balance + deposit_amt;
}
// Asks for amount to withdraw and returns the updated balance
float withdraw(int current_pin, float balance)
{
    float withdraw_amt;

    // Request for a PIN
    request_pin(current_pin);

    // Ask for amount to withdraw
    printf("Withdraw Amount: ");
    scanf("%f", &withdraw_amt);

    // Clear the input buffer
    while ((getchar()) != '\n')
        ;

    // Check if amount requested exceeds current balance
    if (withdraw_amt > balance)
    {
        printf("ERROR: Amount exceeds current balance!\n");
        printf("Current Balance: %.2f\n", balance);
        printf("Requested Amount: %.2f\n", withdraw_amt);
        return balance;
    }
    // Check if amount requested is negative (resulting in balance increase).
    else if (withdraw_amt < 0)
    {
        printf("ERROR: Amount is negative!\n");
        return balance;
    }
    else
    {
        printf("---------------------------\n");
        printf("New Balance: %.2f\n", balance - withdraw_amt);
        printf("---------------------------\n\n");
        return balance - withdraw_amt;
    }
}

// Changes the PIN of the account
int change_pin(int current_pin)
{
    int new_pin;

    // Request for a PIN
    request_pin(current_pin);

    // Ask for a new PIN
    printf("Enter new PIN: ");
    scanf("%d", &new_pin);

    // Clear the input buffer
    while ((getchar()) != '\n')
        ;

    // Validate PIN
    if (new_pin < 1000 || new_pin > 9999)
    {
        printf("ERROR: PIN must have 4 digits.\n");
        return current_pin;
    }
    else
    {
        printf("Changed PIN successfully!\n");
        return new_pin;
    }
}