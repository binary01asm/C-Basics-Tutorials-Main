#include <stdio.h> // Preprocessor_directive_to_include_standard_input_output_header_file------!>
int main() // Main_function_entry_point_of_the_program------!>
{ // Opening_brace_to_start_the_function_body------!>
	char array[5] = {'A','B','C','D','E'};
	
	// Method_1:_Print_using_a_for_loop------!>
	printf("Array elements (using for loop):\n"); // Print_header------!>
	for(int i = 0; i < 5; i++) // Loop_through_array_indices------!>
	{ // Opening_brace_for_loop_body------!>
		printf("array[%d] = %c\n", i, array[i]); // Print_each_element_with_its_index------!>
	} // Closing_brace_for_loop_body------!>
	
	// Method_2:_Print_all_elements_in_a_single_line------!>
	printf("\nArray elements (single line): "); // Print_header------!>
	for(int i = 0; i < 5; i++) // Loop_through_array_indices------!>
	{ // Opening_brace_for_loop_body------!>
		printf("%c ", array[i]); // Print_each_element_followed_by_a_space------!>
	} // Closing_brace_for_loop_body------!>
	printf("\n"); // Print_newline_character------!>
	
	// Method_3:_Print_as_a_string------!>
	printf("\nAs a string: "); // Print_header------!>
	printf("%.5s\n", array); // Print_array_as_string_limited_to_5_characters------!>
	
	return 0; // Return_statement_indicating_successful_program_execution_to_operating_system------!>
} // Closing_brace_to_end_the_main_function------!>