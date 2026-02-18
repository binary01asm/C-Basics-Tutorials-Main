#include <stdio.h>
#include <string.h>

int main() {
    int limit = 50; //<!------_Initialize_the_limit_for_the_loop_------!>

    //<!------_Start_a_loop_that_runs_from_0_to_the_specified_limit_------!>
    for (int loop = 0; loop <= limit; loop++) {
        //<!------_Print_the_current_loop_iteration_------!>
        printf("Loop Started...%d\n", loop);
    }

    return 0; //<!------_Indicate_successful_program_termination_------!>
}