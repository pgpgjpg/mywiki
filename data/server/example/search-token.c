#include <stdio.h>
#include <string.h>
#define MAX_LENGTH		20	/*한 문자열의 최대 길이 */

/*함수 원형 선언 */
void markLine(char *line, char *start, char *stop);
void outputLine(char* s);
char *inputLine(void);

void markLine(char *line, char *start, char *stop)
{
    char *p;

    for (p = line; p < start; p++)
        *p = ' ';

    for (p = start; p <= stop; p++)
        *p = '^';

    for (p = stop+1; *p != '\0'; p++)
        *p = ' ';
}

void outputLine(char *string)
{
	puts(string);
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

int main(int argc, char** argv)
{
	char *token;
	char *delimit;
	char *p;
	char *line;

	/*프로그램 실행을 위한 인자 확인 */
	if (3 != argc) {
		outputLine("Usage : search-token token delimit");
		return 0;
	}

	/*검색 부분 문자열을 설정한다.*/
	token = argv[1];
	delimit = argv[2];

	while (NULL != (line = inputLine())) {

		/*첫번째 토큰을 탐색한다.*/
		p=strtok(line, delimit);

		while (NULL != p) {
			printf("분리된 token = %s\n", p);
			/*원하는 토큰을 찾는다*/
			if (0 == strcmp(p, token)) {
				outputLine("        match token");
			}

			/*다음 토큰을 얻는다.*/
			p=strtok(NULL,delimit);

		}
	}
}

