#include <stdio.h>
#include <stdlib.h>

int main() {
   //<!------ Declare and initialize an integer array with values 0 to 9 ------!>
   int number[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
   
   //<!------ Declare and initialize a character array with values 'A' to 'F' ------!>
   char character[] = {'A','B','C','D','E','F'};
   
   //<!------ Print each element of the integer array ------!>
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
   
   //<!------ Print each element of the character array ------!>
   printf("Choosed character:\t%c\n", character[0]);
   printf("Choosed character:\t%c\n", character[1]);
   printf("Choosed character:\t%c\n", character[2]);
   printf("Choosed character:\t%c\n", character[3]);
   printf("Choosed character:\t%c\n", character[4]);
   printf("Choosed character:\t%c\n", character[5]);

   return 0; //<!------ Indicate successful program termination ------!>
}