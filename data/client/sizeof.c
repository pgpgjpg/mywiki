#include <stdio.h>

struct one {
	int size;
};

struct two {
	int size;
	char array[0];
};

struct three {
	int size;
	char *array;
};
int main(void)
{
	printf("struct one : %ld\n", sizeof(struct one));
	printf("struct two : %ld\n", sizeof(struct two));
	printf("struct three : %ld\n", sizeof(struct three));
	printf("size of pointer : %ld\n", sizeof(char*));
	printf("size of int : %ld\n", sizeof(int));
	return 0;
}
