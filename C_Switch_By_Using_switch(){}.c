#include <stdio.h>

int main() {
    //<!------Declare_variable_to_store_user_input------!>
    int day;

    //<!------Prompt_user_to_enter_a_day_number------!>
    printf("Enter_a_day_number_between_1_and_7_(0_For_Exit):");
    //<!------Read_the_user_input------!>
    scanf("%d", &day);

    //<!------Switch_statement_to_determine_the_day_based_on_input------!>
    switch (day) {
        //<!------Case_0:_Exit_the_program------!>
        case 0:
            printf("Exit_Successful.\n");
            return 0;  //<!------Terminate_program_immediately------!>
            break;      //<!------Break_is_not_reached_but_included_for_completeness------!>
            
        //<!------Case_1:_Monday------!>
        case 1:
            printf("Monday\n");
            break;      //<!------Exit_switch_statement------!>
            
        //<!------Case_2:_Tuesday------!>
        case 2:
            printf("Tuesday\n");
            break;
            
        //<!------Case_3:_Wednesday------!>
        case 3:
            printf("Wednesday\n");
            break;
            
        //<!------Case_4:_Thursday------!>
        case 4:
            printf("Thursday\n");
            break;
            
        //<!------Case_5:_Friday------!>
        case 5:
            printf("Friday\n");
            break;
            
        //<!------Case_6:_Saturday------!>
        case 6:
            printf("Saturday\n");
            break;
            
        //<!------Case_7:_Sunday------!>
        case 7:
            printf("Sunday\n");
            break;
            
        //<!------Default_case:_Handle_invalid_input------!>
        default:
            printf("Invalid_input!_Please_enter_a_number_between_1_and_7_(0_For_Exit).\n");
    }

    //<!------Return_0_to_indicate_successful_execution------!>
    return 0;
}