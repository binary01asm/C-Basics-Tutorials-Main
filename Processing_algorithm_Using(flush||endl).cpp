#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

int main() {
    /*_Program_start_initialization_------!>*/
    
    cout << "Processing: 0%";
    /*_Output_initial_progress_message_without_newline_------!>*/
    
    for(int i=1; i<=10; i++) {
        /*_Loop_starts_from_1_to_10_inclusive_------!>*/
        
        this_thread::sleep_for(chrono::milliseconds(1000)); 
        /*_Pause_execution_for_1000_milliseconds_(1_second)_------!>*/
        
        cout << "\rProcessing: " << i*10 << "%" << flush;
        /*_Carriage_return_updates_same_line_with_new_percentage_and_flushes_output_------!>*/
        //cout << "\rProcessing: " << i*10 << "%" << endl;
        /*_Alternative_commented_line_would_add_newline_instead_of_flush_------!>*/
    }
    /*_Loop_ends_after_10_iterations_completing_100%_progress_display_------!>*/
    
    cout << "Starting program..." << endl;
    /*_Display_program_start_message_with_newline_------!>*/
    
    cout << "Task 1 completed" << endl;
    /*_Display_first_task_completion_message_with_newline_------!>*/
    
    cout << "Task 2 completed" << endl;
    /*_Display_second_task_completion_message_with_newline_------!>*/
}
/*_Main_function_ends_and_program_terminates_------!>*/