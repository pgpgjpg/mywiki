#include <stdio.h>
#include <string.h>
#define MAX_LENGTH		1026	/*한 문자열의 최대 길이 */

/*함수 원형 선언 */
void markLine(char *line, char *start, char *stop);
void outputLine(char* s);

int main(int argc, char** argv)
{
	char strings[][MAX_LENGTH] = {
		 "one"
		,"two"
		,"three"
		,"four"
		,"five"
		,"six"
	};
	int size = sizeof(strings)/sizeof(strings[0]);
	int index, length = 0;
	char *p;
	char ch;

	/*프로그램 실행을 위한 인자 확인 */
	if (2 != argc) {
		outputLine("Usage : search-char character");
		return 0;
	}

	/*검색 문자을 설정한다.*/
	ch = argv[1][0];

	for (index=0;index<size;index++) {
		/*문자열에서 문자를 검색한다 */
		p = strchr(strings[index], ch);
		if (NULL != p) {
			outputLine(strings[index]);
			markLine(strings[index], p, p);
			outputLine(strings[index]);
		}
	}

}

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