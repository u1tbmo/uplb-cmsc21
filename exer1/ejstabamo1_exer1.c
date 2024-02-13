/*  Tabamo, Euan Jed S. - ST2L - Feb 13, 2024
    Exercise 1 - GWA Calculator
 */

#include <stdio.h>

int main(void)
{
    // Initialize variables
    int courses = 0, total_units = 0;
    float sum = 0, gwa = 0;

    // While the number of courses is not at least 5, repeat the question.
    do
    {
        // Ask the user for the number of academic courses enrolled this semester.
        printf("How many courses (minimum of 5) this semester affect your GWA? ");
        scanf("%d", &courses);

        if (courses < 5)
        {
            printf("The minimum number of courses is 5.\n");
        }

    } while (courses < 5);

    // Ask the user for each course's number of units and their grade.
    for (int i = 0; i < courses; i++)
    {
        // Declare the variables for each
        int course_units;
        float course_grade;

        // Ask the user for the units and their grade in a course.
        printf("Enter course units: ");
        scanf("%d", &course_units);
        printf("Enter grade: ");
        scanf("%f", &course_grade);

        // Add the units of this course to the total number of units
        total_units += course_units;

        // Add to the sum the product of the units and their grade
        sum += course_grade * course_units;
    }

    // Calculate the GWA
    gwa = (float)sum / total_units;

    // Print the GWA and the number of units
    printf("\n");
    printf("GWA: %.2f\n", gwa);
    printf("Total units: %d\n", total_units);
}