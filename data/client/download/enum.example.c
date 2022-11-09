#include <stdio.h>

int main()
{
	enum color{
		yellow,
		green,
		red
	}sign;
	
	printf("%ld\n", sizeof(sign));

	scanf("%d", &sign);
	switch(sign){
		case yellow:
			printf("yellow\n");
			break;
		case green:
			printf("green\n");
			break;
		case red:
			printf("red\n");
			break;
		default:
			printf("unknown color\n");
			break;
	}

	return 0;
}
