/*  Tabamo, Euan Jed S. - ST2L - Feb 13, 2024
    Exercise 2 - BMI Calculator
 */
#include <stdio.h>
#include <stdbool.h>

// Function prototypes
char *bmi_category(float bmi);

int main(void)
{
    // Initialize variables
    int choice = 0;
    float bmi = 0;

    // Main menu loop
    while (true)
    {
        printf("1. Measure in metric\n");
        printf("2. Measure in standard\n");
        printf("3. Exit\n");
        printf("\nPlease enter your choice: ");
        scanf("%d", &choice);

        // Tell the user if the choice is invalid
        if (choice > 3 || choice < 1)
        {
            printf("ERROR: Invalid choice.\n\n");
        }

        // Ask user for metric measurements
        if (choice == 1)
        {
            float weight_m, height_m;
            printf("Enter weight in kilograms: ");
            scanf("%f", &weight_m);
            printf("Enter height in centimeters: ");
            scanf("%f", &height_m);

            // Convert centimeter height to meters
            height_m /= 100;

            // Calculate BMI
            bmi = weight_m / (height_m * height_m);

            // Print results
            printf("Your BMI is: %.2f\n", bmi);
            printf("BMI Category: %s\n", bmi_category(bmi));
            printf("\n");
        }
        // Ask user for standard measurements
        if (choice == 2)
        {
            float weight_i, height_i, height_feet, height_inches;
            printf("Enter weight in pounds: ");
            scanf("%f", &weight_i);
            printf("Enter height in feet: ");
            scanf("%f", &height_feet);
            printf("Enter height in inches: ");
            scanf("%f", &height_inches);

            // Calculate Height
            height_i = (height_feet * 12) + height_inches;

            // Calculate BMI
            bmi = (weight_i / (height_i * height_i)) * 703;

            // Print results
            printf("Your BMI is: %.2f\n", bmi);
            printf("BMI Category: %s\n", bmi_category(bmi));
            printf("\n");
        }
        // Break out of the loop
        if (choice == 3)
        {
            printf("Goodbye!\n");
            break;
        }
    }

    return 0;
}

// Returns a string (char *) of the BMI category depending on the bmi
char *bmi_category(float bmi)
{
    if (bmi < 18.5)
    {
        return "Underweight";
    }
    else if (bmi <= 24.9)
    {
        return "Normal weight";
    }
    else if (bmi <= 29.9)
    {
        return "Overweight";
    }
    else if (bmi >= 30)
    {
        return "Obesity";
    }
    else
    {
        return "Unknown";
    }
}