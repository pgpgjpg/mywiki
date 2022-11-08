#include <stdio.h>
#include <string.h>
#define MAX_SIZE		16		/*문자열의 최대 개수    */
#define MAX_LENGTH		1026	/*한 문자열의 최대 길이 */

/*함수 원형 선언 */

void bubbleSort(char* strings[], int size);
void outputLine(char*);
char *inputLine(void);

int main(int argc, char** argv)
{
	int n, size;
	char *p, *q, *line;
	char *pArrays[MAX_SIZE];
	char strings[MAX_SIZE][MAX_LENGTH];

	/*표준 입력장치로 부터 최대 MAX_SIZE 만큼의 문자열을 읽는다*/
	for (size=0;size<MAX_SIZE;size++) {
		/*표준 입력장치로 부터 문자열을 읽는다*/
		line = inputLine();
		if (!strcmp("end", line)) break;
		
		/*읽어들인 문자열을 문자열 배열에 복사한다.*/
		q = strings[size];
		for (p=line; '\0' != *p; p++,q++) {
			*q = *p;
		}
		*q = '\0';
		/*복사된 문자열의 위치를 문자열 포인터에 설정한다.*/
		pArrays[size] = strings[size];
	}

	/*문자열을 정렬한다.*/
	bubbleSort(pArrays, size);

	/*문자열을 출력한다*/
    for (n = 0; n < size; n++)
        outputLine(pArrays[n]);

}

/* 버블정렬 - 기본적인 버블 정렬 알고리즘을 구현한다. */
void bubbleSort(char *strings[], int size)
{
    int i, j;
    char *tmp;

    for (j=size-1;j>=0;j--) {
        for (i = 0; i < j; i++) {
            /* 문자열의 길이를 비교하기 위해 strlen()함수를 사용한다.*/
            if (strcmp(strings[i], strings[i+1])>0) {
                tmp = strings[i+1];
                strings[i+1] = strings[i];
                strings[i] = tmp;
            }
        }
    }
}

char * inputLine(void)
{
	char ch;
	static char line[1024];
	int i=0;
	
	while(ch=getc(stdin)) {
		if(ch==EOF)
			return NULL;
		if(ch=='\n')
			break;
		line[i++]=ch;
	}	
	line[i]='\0';
	
	return line;
}

void outputLine(char *string)
{
	puts(string);
}