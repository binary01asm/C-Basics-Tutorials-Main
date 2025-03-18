#include <stdio.h>

int main() {
    int limit = 50; //<!------ Define the upper limit for the loop ------!>
    int loop = 0;   //<!------ Initialize the loop counter ------!>
    
    //<!------ Start a while loop that runs until 'loop' is less than 'limit' ------!>
    while (loop < limit) {
        //<!------ Print the current value of 'loop' ------!>
        printf("Loop Started...%d\n", loop);
        loop++; //<!------ Increment the loop counter by 1 ------!>
    }
    
    return 0; //<!------ Indicate successful program termination ------!>
}