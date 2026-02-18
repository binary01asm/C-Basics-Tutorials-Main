#include <stdio.h>

int main() {
  int num = 10; //<!------Initialize_variable_num_with_value_10------!>
  int *ptr = &num; //<!------Initialize_pointer_ptr_with_address_of_num------!>

  printf("Value:%d\n", *ptr); //<!------Print_value_pointed_by_ptr------!>
  *ptr = 20; //<!------Modify_value_pointed_by_ptr_to_20------!>
  printf("New_Value:%d\n", num); //<!------Print_updated_value_of_num------!>

  return 0; //<!------Return_0_to_indicate_successful_execution------!>
}