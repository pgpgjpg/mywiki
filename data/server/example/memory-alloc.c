#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int *p;

	/*메모리를 할당합니다 */
	p = (int*) malloc (sizeof(int) * 2);
	if (NULL == p) {
		printf("메모리를 할당할 수 없습니다\n");
		return 1;
	}

	printf("malloc() 함수의 의해서 할당된 메모리 내용을 확인합니다\n");
	printf("0) p = %p\n", p);
	printf("p[0] = %d\n", p[0]);
	printf("p[1] = %d\n\n", p[1]);

	free(p);

	/*배열 메모리를 할당합니다 */
	p = (int*) calloc (sizeof(int), 2);
	if (NULL == p) {
		printf("메모리를 할당할 수 없습니다\n");
		return 1;
	}
	printf("calloc() 함수의 의해서 할당된 메모리 내용을 확인합니다\n");
	printf("1) p = %p\n", p);
	printf("p[0] = %d\n", p[0]);
	printf("p[1] = %d\n\n", p[1]);

	printf("할당된 기억장소 내용 변경 후 \n");
	p[0] = 100;
	p[1] = 200;
	printf("p[0] = %d\n", p[0]);
	printf("p[1] = %d\n\n", p[1]);

    

	p = realloc(p, sizeof(int) * 10);
	if (NULL == p) {
		printf("메모리를 할당할 수 없습니다\n");
		return 1;
	}
	printf("realloc() 함수의 의해서 할당된 메모리 포인터를 확인합니다\n");
	printf("2) p = %p\n", p);
	printf("p[0] = %d\n", p[0]);
	printf("p[1] = %d\n\n", p[1]);

	p = realloc(p, sizeof(int) * 100);
	if (NULL == p) {
		printf("메모리를 할당할 수 없습니다\n");
		return 1;
	}
	printf("realloc() 함수의 의해서 할당된 메모리 포인터를 확인합니다\n");
	printf("3) p = %p\n", p);
	printf("p[0] = %d\n", p[0]);
	printf("p[1] = %d\n\n", p[1]);

	p = realloc(p, sizeof(int) * 1000000);
	if (NULL == p) {
		printf("메모리를 할당할 수 없습니다\n");
		return 1;
	}
	printf("realloc() 함수의 의해서 할당된 메모리 포인터를 확인합니다\n");
	printf("4) p = %p\n", p);
	printf("p[0] = %d\n", p[0]);
	printf("p[1] = %d\n\n", p[1]);

	free(p);

	return 0;
}

