# include <stdio.h>


int main()
{
	int table,count=10;
	printf("Enter the table number: ");
	scanf("%d",&table);

	for (int i = 0; i<=count;  i++){
	printf("%d x %d = %d\n",table,i,table*i);
	
	}
	return main();
}