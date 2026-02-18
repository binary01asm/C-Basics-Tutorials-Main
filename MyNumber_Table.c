#include <stdio.h>  //include_standard_input/output_header_file_for_printf_and_scanf--------------!>

int main()  //main_function_-_program_execution_starts_here--------------!>
{
	int table, count=10;  //declare_integer_variables:_'table'_for_user_input,_'count'_initialized_to_10_as_loop_limit--------------!>
	
	printf("Enter the table number: ");  //display_prompt_message_asking_user_to_enter_a_number--------------!>
	scanf("%d",&table);  //read_integer_input_from_user_and_store_it_in_variable_'table'_using_address-of_operator--------------!>

	for (int i = 0; i<=count;  i++){  //for_loop:_initialize_i=0,_loop_while_i<=10,_increment_i_by_1_each_iteration--------------!>
	printf("%d x %d = %d\n",table,i,table*i);  //print_multiplication_table_in_format:_number_x_multiplier_=_product_with_newline--------------!>
	
	}  //end_of_for_loop--------------!>
	
	return main();  //recursively_call_main_function_-_creates_infinite_loop_and_will_cause_stack_overflow--------------!>
}  //end_of_main_function--------------!>