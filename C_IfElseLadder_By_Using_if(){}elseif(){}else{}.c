#include <stdio.h>

int main() {
  int num;

  //<!------Prompt_the_user_to_enter_a_number------!>
  printf("Enter_a_number:");
  scanf("%d", &num);

  //<!------Check_if_the_number_is_positive,_negative,_or_zero------!>
  if (num > 0) {
    printf("Positive\n");
  } else if (num < 0) {
    printf("Negative\n");
  } else {
    printf("Zero\n");
  }

  return 0;
}