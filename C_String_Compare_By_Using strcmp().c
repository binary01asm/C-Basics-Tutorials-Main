#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	//<!------Declare_and_initialize_two_strings------!>
	char str1[]={"Apple"};
	char str2[]={"Banana"};
	
	//<!------Compare_the_two_strings_using_strcmp------!>
	//<!------strcmp_returns:_0_if_equal,_negative_if_str1<str2,_positive_if_str1>str2------!>
	int result= strcmp(str1,str2);
	
	//<!------Check_the_comparison_result------!>
	if (result==0){
		//<!------Execute_if_strings_are_equal------!>
		printf("String are same!");
	}else if(result < 0) {
		//<!------Execute_if_str1_comes_before_str2_alphabetically------!>
		printf("%s comes before %s",str1,str2);          
	}else {		
		//<!------Execute_if_str1_comes_after_str2_alphabetically------!>
		printf("%s comes after %s",str1,str2);
	}
	
	//<!------Return_0_to_indicate_successful_execution------!>
	return 0;
}