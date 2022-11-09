#include <stdio.h>
#include <math.h>

int main()
{
	int input, i, j, cnt, limit;
	int printCnt = 0;

	printf("Enter a number greater than 2 --> ");
	scanf("%d", &input);

	for(i = 2; i <= input; ++i){
		//limit = sqrt((double)i);
		for(j = 2, cnt = 0; j <= i; ++j){
			if(i%j == 0)
				++cnt;		
			if(cnt >= 2)
				break;
		}
		if(cnt == 1){
			printf("%5d", i);
			++printCnt;
		}
		if(printCnt == 5){
			printf("\n");
			printCnt = 0;
		}
	}
	printf("\n");

	return 0;
}

