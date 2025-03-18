#include <stdio.h>
#include <string.h>

int main() {
    int limit = 50; //<!------ Initialize the limit for the loop ------!>

    //<!------ Start a loop that runs from 0 to the specified limit ------!>
    for (int loop = 0; loop <= limit; loop++) {
        //<!------ Print the current loop iteration ------!>
        printf("Loop Started...%d\n", loop);
    }

    return 0; //<!------ Indicate successful program termination ------!>
}