#include <stdio.h>   // Standard_Input/Output_library_for_printf,_scanf--------------!>
#include <math.h>    // Math_library_(currently_unused,_but_kept_for_potential_extensions)--------------!>

// Function_to_add_two_numbers--------------!>
// Parameters:_a_-_first_operand,_b_-_second_operand--------------!>
// Returns:_sum_of_a_and_b_as_double--------------!>
double add(double a, double b) {
    return a + b;   // Perform_addition_and_return_result--------------!>
}

// Function_to_subtract_two_numbers--------------!>
// Parameters:_a_-_first_operand,_b_-_second_operand--------------!>
// Returns:_difference_of_a_and_b_as_double--------------!>
double sub(double a, double b) {
    return a - b;   // Perform_subtraction_and_return_result--------------!>
}

// Function_to_multiply_two_numbers--------------!>
// Parameters:_a_-_first_operand,_b_-_second_operand--------------!>
// Returns:_product_of_a_and_b_as_double--------------!>
double mul(double a, double b) {
    return a * b;   // Perform_multiplication_and_return_result--------------!>
}

// Function_to_divide_two_numbers--------------!>
// Parameters:_a_-_dividend,_b_-_divisor--------------!>
// Returns:_quotient_of_a_divided_by_b_as_double--------------!>
double division(double a, double b) {
    return a / b;   // Perform_division_and_return_result--------------!>
}

// Main_function_-_program_entry_point--------------!>
int main() {
    double num1, num2, result;  // Variables_to_store_operands_and_result--------------!>
    char op;                    // Variable_to_store_operator--------------!>
    
    // Prompt_user_for_first_number--------------!>
    printf("Enter the first num: ");
    // Read_first_number_as_double_precision_floating_point--------------!>
    scanf("%lf", &num1);
    
    // Prompt_user_for_operator--------------!>
    printf("Enter the operator: ");
    // Read_operator_as_character_(space_before_%c_consumes_any_leftover_newline)--------------!>
    scanf(" %c", &op);
    
    // Prompt_user_for_second_number--------------!>
    printf("Enter the second num: ");
    // Read_second_number_as_double_precision_floating_point--------------!>
    scanf("%lf", &num2);
    
    // Switch_statement_to_handle_different_operators--------------!>
    switch (op) {
        case '+': // Addition_operator--------------!>
            // Call_add_function_and_store_result--------------!>
            result = add(num1, num2);
            // Display_formatted_result_with_2_decimal_places--------------!>
            printf("%.2lf + %.2lf = %.2lf\n", num1, num2, result);
            break;  // Exit_switch_statement--------------!>
            
        case '-': // Subtraction_operator--------------!>
            // Call_sub_function_and_store_result--------------!>
            result = sub(num1, num2);
            // Display_formatted_result_with_2_decimal_places--------------!>
            printf("%.2lf - %.2lf = %.2lf\n", num1, num2, result);
            break;  // Exit_switch_statement--------------!>
            
        case '*': // Multiplication_operator--------------!>
            // Call_mul_function_and_store_result--------------!>
            result = mul(num1, num2);
            // Display_formatted_result_with_2_decimal_places--------------!>
            printf("%.2lf * %.2lf = %.2lf\n", num1, num2, result);
            break;  // Exit_switch_statement--------------!>
            
        case '/': // Division_operator--------------!>
            // Check_for_division_by_zero_to_prevent_runtime_error--------------!>
            if (num2 != 0) {
                // Call_division_function_and_store_result--------------!>
                result = division(num1, num2);
                // Display_formatted_result_with_2_decimal_places--------------!>
                printf("%.2lf / %.2lf = %.2lf\n", num1, num2, result);
            } else {
                // Display_error_message_for_division_by_zero--------------!>
                printf("Error: Division by zero!\n");
            }
            break;  // Exit_switch_statement--------------!>
            
        default: // Handle_invalid_operators--------------!>
            // Display_error_message_for_invalid_operator--------------!>
            printf("Error: Invalid operator!\n");
            return 1;  // Return_error_code_(non-zero_indicates_abnormal_termination)--------------!>
    }
    
    // Return_0_to_indicate_successful_program_execution--------------!>
    return 0;
}