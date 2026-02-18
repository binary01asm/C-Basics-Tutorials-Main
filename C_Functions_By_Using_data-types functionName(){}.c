#include <stdio.h>

//<!------Function_to_add_two_numbers------!>
float add(float a, float b) {
    return a + b;
}

//<!------Function_to_subtract_two_numbers------!>
float subtract(float a, float b) {
    return a - b;
}

//<!------Function_to_multiply_two_numbers------!>
float multiply(float a, float b) {
    return a * b;
}

//<!------Function_to_divide_two_numbers_(returns_float)------!>
float divide(float a, float b) {
    //<!------Check_for_division_by_zero------!>
    if (b == 0) {
        printf("Error: Division by zero!\n");
        return 0;
    }
    //<!------Return_quotient_if_divisor_is_non-zero------!>
    return a / b;
}

//<!------Function_to_print_a_message------!>
void printMessage() {
    printf("Hello from the printMessage function!\n");
}

int main() {
    //<!------Declare_variables_to_store_user_input------!>
    float num1, num2;

    //<!------Prompt_user_to_enter_two_numbers------!>
    printf("Enter two numbers: ");
    
    //<!------Read_two_float_values_from_user_input------!>
    scanf("%f %f", &num1, &num2);

    //<!------Call_the_add_function_and_display_result------!>
    printf("Sum: %.2f\n", add(num1, num2));

    //<!------Call_the_subtract_function_and_display_result------!>
    printf("Difference: %.2f\n", subtract(num1, num2));

    //<!------Call_the_multiply_function_and_display_result------!>
    printf("Product: %.2f\n", multiply(num1, num2));

    //<!------Call_the_divide_function_with_error_handling------!>
    if (num2 != 0) {
        //<!------Display_quotient_if_divisor_is_non-zero------!>
        printf("Quotient: %.2f\n", divide(num1, num2));
    } else {
        //<!------Call_divide_function_to_show_error_message------!>
        divide(num1, num2);
    }

    //<!------Call_the_printMessage_function------!>
    printMessage();

    //<!------Return_0_to_indicate_successful_execution------!>
    return 0;
}