#include <stdio.h>
#include <stdlib.h>

int main() {
   //<!------_Declare_and_initialize_an_integer_array_with_values_0_to_9_------!>
   int number[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
   
   //<!------_Declare_and_initialize_a_character_array_with_values_'A'_to_'F'_------!>
   char character[] = {'A','B','C','D','E','F'};
   
   //<!------_Print_each_element_of_the_integer_array_------!>
   printf("Choosed Numbers:\t%d\n", number[0]);
   printf("Choosed Numbers:\t%d\n", number[1]);
   printf("Choosed Numbers:\t%d\n", number[2]);
   printf("Choosed Numbers:\t%d\n", number[3]);
   printf("Choosed Numbers:\t%d\n", number[4]);
   printf("Choosed Numbers:\t%d\n", number[5]);
   printf("Choosed Numbers:\t%d\n", number[6]);
   printf("Choosed Numbers:\t%d\n", number[7]);
   printf("Choosed Numbers:\t%d\n", number[8]);
   printf("Choosed Numbers:\t%d\n", number[9]);
   
   //<!------_Print_each_element_of_the_character_array_------!>
   printf("Choosed character:\t%c\n", character[0]);
   printf("Choosed character:\t%c\n", character[1]);
   printf("Choosed character:\t%c\n", character[2]);
   printf("Choosed character:\t%c\n", character[3]);
   printf("Choosed character:\t%c\n", character[4]);
   printf("Choosed character:\t%c\n", character[5]);

   return 0; //<!------_Indicate_successful_program_termination_------!>
}