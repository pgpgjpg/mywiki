#include <stdio.h>
#include <stdlib.h>

int main()
{
	char ch;

	system("stty raw -echo  min 1 time 0");

	while(1){
		ch = getchar();
		if(ch == '\n' || ch == '\r')
			break;
		putchar('*');
	}
	
	printf("\n");

	system("stty sane"); 

	return 0;
}
