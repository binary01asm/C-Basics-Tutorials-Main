#include <stdio.h>

int main() {
    //<!------Declare_a_file_pointer------!>
    FILE *fptr;

    //<!------Open_the_file_in_write_mode------!>
    fptr = fopen("test.txt", "w");

    //<!------Check_if_the_file_was_opened_successfully------!>
    if (fptr == NULL) {
        printf("Error_opening_file_for_writing!\n");
        return 1;
    }

    //<!------Write_a_string_to_the_file------!>
    fprintf(fptr, "Hello File\n");

    //<!------Close_the_file------!>
    fclose(fptr);

    //<!------Print_a_success_message------!>
    printf("File_written_successfully.\n");

    return 0;
}