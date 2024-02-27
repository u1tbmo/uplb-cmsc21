#include <stdio.h>

// Function Prototype
float calculate_growth(float current_population, int remaining_years, float annual_growth_rate);

int main()
{
    // Variables
    int population, years;
    float growth_rate, result;

    // Ask for population and validate.
    printf("Population (n): ");
    scanf("%d", &population);
    if (population <= 0)
    {
        printf("Error 1: Population must at least be 1.\n");
        return 1;
    }

    // Ask for growth rate
    printf("Growth Rate (y%%): ");
    scanf("%f", &growth_rate);

    // Ask for number of years
    printf("Years (x): ");
    scanf("%d", &years);
    if (years < 0)
    {
        printf("Error 2: Number of years cannot be negative.\n");
        return 2;
    }

    // Get the result
    result = calculate_growth(population, years, growth_rate / 100);

    // Print the result
    printf("\nAfter %d year/s, at %.2f%% growth rate, the population will be %f\n", years, growth_rate, result);

    return 0;
}

// A recursive function calculating the growth of a population.
float calculate_growth(float current_population, int remaining_years, float annual_growth_rate)
{
    // Base Case
    if (remaining_years == 0)
    {
        // In 0 remaining_years, there will be no growth in the current_population.
        return current_population;
    }

    // Recursive Case
    // If remaining_years > 0, calculate the current_population growth, and decrement remaining_years.
    return calculate_growth(current_population + (current_population * annual_growth_rate), remaining_years - 1, annual_growth_rate);
}