#include <stdio.h>   // Standard Input/Output library for printf, scanf
#include <math.h>    // Math library (currently unused, but kept for potential extensions)

// Function to add two numbers
// Parameters: a - first operand, b - second operand
// Returns: sum of a and b as double
double add(double a, double b) {
    return a + b;   // Perform addition and return result
}

// Function to subtract two numbers
// Parameters: a - first operand, b - second operand
// Returns: difference of a and b as double
double sub(double a, double b) {
    return a - b;   // Perform subtraction and return result
}

// Function to multiply two numbers
// Parameters: a - first operand, b - second operand
// Returns: product of a and b as double
double mul(double a, double b) {
    return a * b;   // Perform multiplication and return result
}

// Function to divide two numbers
// Parameters: a - dividend, b - divisor
// Returns: quotient of a divided by b as double
double division(double a, double b) {
    return a / b;   // Perform division and return result
}

// Main function - program entry point
int main() {
    double num1, num2, result;  // Variables to store operands and result
    char op;                    // Variable to store operator
    
    // Prompt user for first number
    printf("Enter the first num: ");
    // Read first number as double precision floating point
    scanf("%lf", &num1);
    
    // Prompt user for operator
    printf("Enter the operator: ");
    // Read operator as character (space before %c consumes any leftover newline)
    scanf(" %c", &op);
    
    // Prompt user for second number
    printf("Enter the second num: ");
    // Read second number as double precision floating point
    scanf("%lf", &num2);
    
    // Switch statement to handle different operators
    switch (op) {
        case '+': // Addition operator
            // Call add function and store result
            result = add(num1, num2);
            // Display formatted result with 2 decimal places
            printf("%.2lf + %.2lf = %.2lf\n", num1, num2, result);
            break;  // Exit switch statement
            
        case '-': // Subtraction operator
            // Call sub function and store result
            result = sub(num1, num2);
            // Display formatted result with 2 decimal places
            printf("%.2lf - %.2lf = %.2lf\n", num1, num2, result);
            break;  // Exit switch statement
            
        case '*': // Multiplication operator
            // Call mul function and store result
            result = mul(num1, num2);
            // Display formatted result with 2 decimal places
            printf("%.2lf * %.2lf = %.2lf\n", num1, num2, result);
            break;  // Exit switch statement
            
        case '/': // Division operator
            // Check for division by zero to prevent runtime error
            if (num2 != 0) {
                // Call division function and store result
                result = division(num1, num2);
                // Display formatted result with 2 decimal places
                printf("%.2lf / %.2lf = %.2lf\n", num1, num2, result);
            } else {
                // Display error message for division by zero
                printf("Error: Division by zero!\n");
            }
            break;  // Exit switch statement
            
        default: // Handle invalid operators
            // Display error message for invalid operator
            printf("Error: Invalid operator!\n");
            return 1;  // Return error code (non-zero indicates abnormal termination)
    }
    
    // Return 0 to indicate successful program execution
    return 0;
}