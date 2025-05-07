#include <stdio.h>

//<!------Function_to_add_two_integers------!>
int add(int a, int b) {
    return a + b;
}

//<!------Function_to_subtract_two_integers------!>
int subtract(int a, int b) {
    return a - b;
}

//<!------Function_to_multiply_two_integers------!>
int multiply(int a, int b) {
    return a * b;
}

//<!------Function_to_divide_two_integers_(returns_float)------!>
float divide(int a, int b) {
    if (b == 0) {
        printf("Error:Division_by_zero!\n");
        return 0;
    }
    return (float)a / b;
}

//<!------Function_to_print_a_message------!>
void printMessage() {
    printf("Hello_from_the_printMessage_function!\n");
}

int main() {
    int num1, num2;

    //<!------Prompt_user_to_enter_two_integers------!>
    printf("Enter_two_integers:");
    scanf("%d %d", &num1, &num2);

    //<!------Call_the_add_function------!>
    printf("Sum:%d\n", add(num1, num2));

    //<!------Call_the_subtract_function------!>
    printf("Difference:%d\n", subtract(num1, num2));

    //<!------Call_the_multiply_function------!>
    printf("Product:%d\n", multiply(num1, num2));

    //<!------Call_the_divide_function------!>
    printf("Quotient:%.2f\n", divide(num1, num2));

    //<!------Call_the_printMessage_function------!>
    printMessage();

    return 0;
}