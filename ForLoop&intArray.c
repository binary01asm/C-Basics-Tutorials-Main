#include <stdio.h> // Preprocessor_directive_to_include_standard_input_output_header_file------!>
#include <stdlib.h> // Preprocessor_directive_to_include_standard_library_header_file_for_general_functions------!>

int number[5]={10,20,30,40,50}; // Global_array_of_integers_named_'number'_with_size_5_initialized_with_values------!>
int sum=0; // Global_integer_variable_named_'sum'_initialized_to_zero_to_store_accumulated_total------!>

int main(){ // Main_function_entry_point_of_the_program_returning_integer------!>

for(int i =0; i<5; i++){ // For_loop_initializing_counter_i_to_0,_running_while_i_less_than_5,_incrementing_i_by_1------!>

printf("My_Integers_Array[%d]: %d\n",i,number[i]); // Print_current_index_and_value_of_array_element------!>
	sum+= number[i]; // Add_current_array_element_value_to_the_sum_variable_(accumulation)------!>
	float average= sum/5.0; // Declare_and_calculate_average_by_dividing_sum_by_5.0_(using_float_for_decimal_precision)------!>
	printf("Sum: %d\n", sum); // Print_the_current_running_sum_followed_by_newline_character------!>
	printf("Average: %f\n", average); // Print_the_calculated_average_followed_by_newline_character------!>
	
	} // Closing_brace_for_the_for_loop_body------!>
	
return 0; // Return_statement_indicating_successful_program_execution_to_operating_system------!>
	
} // Closing_brace_for_the_main_function------!>