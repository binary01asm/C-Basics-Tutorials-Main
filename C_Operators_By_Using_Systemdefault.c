#include <stdio.h>

int main() {
    int a = 10, b = 5;
    int result;

    //<!------Arithmetic_Operators------!>
    printf("Arithmetic_Operators:\n");
    printf("%d + %d = %d\n", a, b, a + b); //<!------Addition------!>
    printf("%d - %d = %d\n", a, b, a - b); //<!------Subtraction------!>
    printf("%d * %d = %d\n", a, b, a * b); //<!------Multiplication------!>
    printf("%d / %d = %d\n", a, b, a / b); //<!------Division------!>
    printf("%d %% %d = %d\n", a, b, a % b); //<!------Modulus------!>
    printf("\n");

    //<!------Relational_Operators------!>
    printf("Relational_Operators:\n");
    printf("%d > %d = %d\n", a, b, a > b); //<!------Greater_than------!>
    printf("%d < %d = %d\n", a, b, a < b); //<!------Less_than------!>
    printf("%d == %d = %d\n", a, b, a == b); //<!------Equal_to------!>
    printf("%d != %d = %d\n", a, b, a != b); //<!------Not_equal_to------!>
    printf("%d >= %d = %d\n", a, b, a >= b); //<!------Greater_than_or_equal_to------!>
    printf("%d <= %d = %d\n", a, b, a <= b); //<!------Less_than_or_equal_to------!>
    printf("\n");

    //<!------Logical_Operators------!>
    printf("Logical_Operators:\n");
    printf("(%d > %d) && (%d < 20) = %d\n", a, b, a, (a > b) && (a < 20)); //<!------Logical_AND------!>
    printf("(%d > %d) || (%d < 0) = %d\n", a, b, a, (a > b) || (a < 0)); //<!------Logical_OR------!>
    printf("!(%d > %d) = %d\n", a, b, !(a > b)); //<!------Logical_NOT------!>
    printf("\n");

    //<!------Assignment_Operators------!>
    printf("Assignment_Operators:\n");
    result = a; //<!------Simple_assignment------!>
    printf("result = %d\n", result);
    result += b; //<!------Addition_assignment------!>
    printf("result += %d => result = %d\n", b, result);
    result -= b; //<!------Subtraction_assignment------!>
    printf("result -= %d => result = %d\n", b, result);
    result *= b; //<!------Multiplication_assignment------!>
    printf("result *= %d => result = %d\n", b, result);
    result /= b; //<!------Division_assignment------!>
    printf("result /= %d => result = %d\n", b, result);
    result %= b; //<!------Modulus_assignment------!>
    printf("result %%= %d => result = %d\n", b, result);
    printf("\n");

    return 0;
}