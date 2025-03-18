#include <stdio.h>

int main() {
    FILE *fptr; //<!------File_pointer_to_handle_the_file------!>
    char buffer[100]; //<!------Buffer_to_store_the_file_content------!>

    //<!------Open_the_file_in_read_mode------!>
    fptr = fopen("test.txt", "r");
    if (fptr == NULL) { //<!------Check_if_file_opening_failed------!>
        printf("Error_opening_file_for_reading!\n"); //<!------Print_error_message------!>
        return 1; //<!------Return_error_code------!>
    }

    //<!------Read_the_first_line_from_the_file------!>
    fgets(buffer, 100, fptr);

    //<!------Print_the_file_content------!>
    printf("File_content:%s", buffer);

    //<!------Close_the_file------!>
    fclose(fptr);

    return 0; //<!------Return_success_code------!>
}