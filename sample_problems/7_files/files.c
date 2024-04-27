#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global Variables

#define MAX_STR_LEN 51
#define STUDENT_NUMBER_LEN 11
#define MAX_STUDENTS 10
#define FILE_LOCATION "class.txt"

// Structures

typedef struct name
{
    char first_name[MAX_STR_LEN];
    char last_name[MAX_STR_LEN];
} name;

typedef struct student_record
{
    struct name *name;
    char student_number[STUDENT_NUMBER_LEN];
    int absences;
} student_record;

//  Prototypes

int menu();                                                                // prints a menu and returns an integer of the user's choice
void load_data(char *load_file, student_record *records, int *count);      // loads data from 'load_file'
void save_data(char *save_file, student_record *records, int count);       // saves data to 'save_file'
void add_student_record(student_record *records, int *count);              // adds a student to the record
void print_records(student_record *records, int count);                    // prints all student records in the 'records' array
void add_absence(student_record *records, int count);                      // adds an absence to a student's 'record'
void delete_student(student_record *records, int *count);                  // deletes a student from the 'records' array
void free_records(student_record *records, int count);                     // frees all records
int is_valid(char *student_number);                                        // returns the validity of a student number
int is_existing(student_record *records, int count, char *student_number); // checks if the student number exists in the record

int main()
{
    int count = 0; // Keep track of number of student records
    int running = 1;
    student_record *records = (student_record *)malloc(sizeof(student_record) * MAX_STUDENTS);
    load_data(FILE_LOCATION, records, &count);

    while (running)
    {
        switch (menu())
        {
        case 0:
            printf("Goodbye!\n");
            running = 0;
            break;
        case 1:
            if (count == MAX_STUDENTS)
            {
                printf("Error: Max records.\n");
                break;
            }
            add_student_record(records, &count);
            break;
        case 2:
            if (count == 0)
            {
                printf("Error: No records.\n");
                break;
            }
            print_records(records, count);
            break;
        case 3:
            if (count == 0)
            {
                printf("Error: No records.\n");
                break;
            }
            add_absence(records, count);
            break;
        case 4:
            if (count == 0)
            {
                printf("Error: No records.\n");
                break;
            }
            delete_student(records, &count);
            break;
        default:
            printf("Error: Invalid choice.\n");
            break;
        }
        save_data(FILE_LOCATION, records, count);
    }

    if (count != 0)
    {
        free_records(records, count);
        free(records);
    }
}

int menu()
{
    int choice, input_buffer;

    printf("\n");
    printf("-----------------------------\n");
    printf("1 | Add Student Record\n");
    printf("2 | Print All Student Records\n");
    printf("3 | Add Absence\n");
    printf("4 | Delete Student\n");
    printf("0 | Exit\n");
    printf("-----------------------------\n");
    printf("Choice: ");
    scanf("%d", &choice);

    while ((input_buffer = getchar()) != '\n' && input_buffer != EOF)
    {
    }; // Clear the input buffer
    printf("\n");

    return choice;
}

void load_data(char *load_file, student_record *records, int *count)
{
    FILE *fptr;
    char *temp;

    do
    {
        fptr = fopen(load_file, "r");
        if (fptr == NULL)
        {
            fptr = fopen(load_file, "w");
            fclose(fptr);
        }
    } while (fptr == NULL);

    char temp[MAX_STR_LEN];
    fgets(temp, MAX_STR_LEN, fptr);
    int count_temp;
    sscanf(temp, "%d", &count_temp);
    *count = count_temp;

    int i = 0;
    int status = 0; // 0 for name, 1 for student number, 2 for absences
    while (fgets(temp, MAX_STR_LEN, fptr))
    {
        switch (status)
        {
        case 0:
            records[i].name = (name *)malloc(sizeof(name));

            // Copy first name
            char *piece = strtok(temp, " ");
            strcpy(records[i].name->first_name, piece);

            // Copy last name
            piece = strtok(NULL, " ");
            if (piece[strlen(piece) - 1] == '\n')
            {
                piece[strlen(piece) - 1] = '\0';
            }
            strcpy(records[i].name->last_name, piece);
            status++;
            break;
        case 1:
            if (temp[strlen(temp) - 1] == '\n')
            {
                temp[strlen(temp) - 1] = '\0';
            }
            strcpy(records[i].student_number, temp);
            status++;
            break;
        case 2:
            int absences;
            sscanf(temp, "%d", &absences);
            records[i].absences = absences;
            i++;
            status = 0;
            break;
        default:
            break;
        }
    }
}

void save_data(char *save_file, student_record *records, int count)
{
    FILE *fptr = fopen(save_file, "w");
    fprintf(fptr, "%d\n", count);
    for (int i = 0; i < count; i++)
    {
        fprintf(fptr, "%s %s\n", records[i].name->first_name, records[i].name->last_name);
        fprintf(fptr, "%s\n", records[i].student_number);
        fprintf(fptr, "%d\n", records[i].absences);
    }
    fclose(fptr);
}

void add_student_record(student_record *records, int *count)
{
    int input_buffer;

    // Create a student record
    student_record record;
    record.name = (name *)malloc(sizeof(name));

    printf("Enter Name (FirstName LastName): ");
    scanf("%s %s", record.name->first_name, record.name->last_name);

    while ((input_buffer = getchar()) != '\n' && input_buffer != EOF)
    {
    }; // Clear the input buffer

    int valid_student_number, existing_student_number;
    do
    {
        printf("Enter Student Number: ");
        scanf("%s", record.student_number);

        while ((input_buffer = getchar()) != '\n' && input_buffer != EOF)
        {
        }; // Clear the input buffer

        valid_student_number = is_valid(record.student_number);
        existing_student_number = is_existing(records, *count, record.student_number);
        if (!valid_student_number)
        {
            printf("Error: Invalid student number.\n");
        }
        if (existing_student_number)
        {
            printf("Error: Student number exists in records.\n");
        }

    } while (!valid_student_number || existing_student_number);

    record.absences = 0;

    // Append to array
    records[(*count)++] = record;
}

void print_records(student_record *records, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("--- Student %d ---------------\n", i + 1);
        printf("Name: %s %s\n", records[i].name->first_name, records[i].name->last_name);
        printf("Student Number: %s\n", records[i].student_number);
        printf("Absences: %d\n", records[i].absences);
        printf("\n");
    }
}

void add_absence(student_record *records, int count)
{
    int input_buffer;

    char student_number[STUDENT_NUMBER_LEN];
    int valid_student_number, existing_student_number;
    do
    {
        printf("Enter student number: ");
        scanf("%s", student_number);

        while ((input_buffer = getchar()) != '\n' && input_buffer != EOF)
        {
        }; // Clear the input buffer

        valid_student_number = is_valid(student_number);
        existing_student_number = is_existing(records, count, student_number);
        if (!valid_student_number)
        {
            printf("Error: Invalid student number.\n");
        }
    } while (!valid_student_number);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(records[i].student_number, student_number) == 0)
        {
            printf("Success: Added an absence to %s\n", records[i].student_number);
            records[i].absences++;
            return;
        }
    }
    printf("Error: Student not found.\n");
}

void delete_student(student_record *records, int *count)
{
    int input_buffer;

    char student_number[STUDENT_NUMBER_LEN];
    int valid_student_number, existing_student_number;
    do
    {
        printf("Enter student number: ");
        scanf("%s", student_number);

        while ((input_buffer = getchar()) != '\n' && input_buffer != EOF)
        {
        }; // Clear the input buffer

        valid_student_number = is_valid(student_number);
        existing_student_number = is_existing(records, *count, student_number);
        if (!valid_student_number)
        {
            printf("Error: Invalid student number.\n");
        }
    } while (!valid_student_number);

    for (int i = 0; i < *count; i++)
    {
        if (strcmp(records[i].student_number, student_number) == 0)
        {
            printf("Success: Deleted student %s\n", student_number);
            free(records[i].name);
            for (int j = i; j < *count - 1; j++)
            {
                records[j] = records[j + 1];
            }
            (*count)--;
            return;
        }
    }
    printf("Error: Student not found.\n");
}

void free_records(student_record *records, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(records[i].name);
    }
}

int is_valid(char *student_number)
{
    if (strlen(student_number) < STUDENT_NUMBER_LEN - 1)
    {
        return 0;
    }

    if (student_number[4] != '-')
    {
        return 0;
    }

    for (int i = 0; i < STUDENT_NUMBER_LEN - 1; i++)
    {
        if (!isdigit(student_number[i]) && i != 4)
        {
            return 0;
        }
    }

    return 1;
}

int is_existing(student_record *records, int count, char *student_number)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(records[i].student_number, student_number) == 0)
        {
            return 1;
        }
    }
    return 0;
}