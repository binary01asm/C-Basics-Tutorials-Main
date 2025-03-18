#include <stdio.h>

int main() {
    int day;

    printf("Enter_a_day_number_between_1_and_7_(0_For_Exit):");
    scanf("%d", &day);

    switch (day) {
        case 0:
            printf("Exit_Successful.\n");
            return 0;
            break;
        case 1:
            printf("Monday\n");
            break;
        case 2:
            printf("Tuesday\n");
            break;
        case 3:
            printf("Wednesday\n");
            break;
        case 4:
            printf("Thursday\n");
            break;
        case 5:
            printf("Friday\n");
            break;
        case 6:
            printf("Saturday\n");
            break;
        case 7:
            printf("Sunday\n");
            break;
        default:
            printf("Invalid_input!_Please_enter_a_number_between_1_and_7_(0_For_Exit).\n");
    }

    return 0;
}