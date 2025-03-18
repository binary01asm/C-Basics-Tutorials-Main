#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char str1[]={"Apple"};
	char str2[]={"Banana"};
	int result= strcmp(str1,str2);
	
	if (result==0){
		printf("String are same!");
		}else if(result < 0) {
		printf("%s comes before %s",str1,str2);          }else {		
			printf("%s comes after %s",str1,str2);
		}
	return 0;
	
}