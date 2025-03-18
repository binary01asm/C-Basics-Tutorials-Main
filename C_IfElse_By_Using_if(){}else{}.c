#include <stdio.h>

int main() {
    int num;  //<!------Variable_to_store_the_input_number------!>
    
    printf("Enter_a_number:");  //<!------Prompt_the_user_to_enter_a_number------!>
    scanf("%d", &num);  //<!------Read_the_input_number_from_the_user------!>

    if (num % 2 == 0) {  //<!------Check_if_the_number_is_even------!>
        printf("The_number_is_even.\n");  //<!------Print_message_if_the_number_is_even------!>
    } else {  //<!------Else_block_for_odd_number_check------!>
        printf("The_number_is_odd.\n");  //<!------Print_message_if_the_number_is_odd------!>
    }
    
    return 0;  //<!------Indicate_successful_program_execution------!>
}