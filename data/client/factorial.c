#include <stdio.h>

int factorial(int num);

int main()
{
	int num, res;
	
	printf("number : ");
	scanf("%d", &num);
	
	res = factorial(num);
	printf("result : %d\n", res);

	return 0;
}

int factorial(int num)
{
	if(num == 1)
		return 1;
		
	return num*factorial(num - 1);	
}
