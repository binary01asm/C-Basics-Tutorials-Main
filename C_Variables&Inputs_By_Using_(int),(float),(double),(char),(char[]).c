#include <stdio.h>

int main() {
    //<!------Integer Variable Section------!>
    int integerNum;
    printf("Enter_an_integer:");  //<!------Prompt user to enter an integer------!>
    scanf("%d", &integerNum);      //<!------Read the integer input------!>
    printf("You_entered:%d\n", integerNum);  //<!------Display the entered integer------!>

    //<!------Floating-Point Variable Section------!>
    float floatNum;
    printf("Enter_a_float:");  //<!------Prompt user to enter a float------!>
    scanf("%f", &floatNum);     //<!------Read the float input------!>
    printf("You_entered:%.2f\n", floatNum);  //<!------Display the entered float with 2 decimal places------!>

    //<!------Double Variable Section------!>
    double doubleNum;
    printf("Enter_a_double:");  //<!------Prompt user to enter a double------!>
    scanf("%lf", &doubleNum);    //<!------Read the double input------!>
    printf("You_entered:%.2lf\n", doubleNum);  //<!------Display the entered double with 2 decimal places------!>

    //<!------Character Variable Section------!>
    char character;
    printf("Enter_a_character:");  //<!------Prompt user to enter a character------!>
    scanf(" %c", &character);       //<!------Read the character input (space before %c to ignore leftover newline)------!>
    printf("You_entered:%c\n", character);  //<!------Display the entered character------!>

    //<!------String (Character Array) Variable Section------!>
    char string[100];
    printf("Enter_a_string:");  //<!------Prompt user to enter a string------!>
    scanf(" %[^\n]", string);    //<!------Read the string input until a newline is encountered------!>
    printf("You_entered:%s\n", string);  //<!------Display the entered string------!>

    return 0;  //<!------End of program------!>
}