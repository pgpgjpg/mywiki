#include <locale.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/*
 * Sample formats.
 */
char *formats[] = {
    "%A, %B %e, %Y, %H:%M:%S",
    "%I:%M %p, %d-%b-%y",
    "%x %X",
    "%C",
    "%c",
    NULL
};

int main(int argc, char **argv)
{
    int i, j;
    time_t clock;
    struct tm *tm;
    char buf[BUFSIZ];

    /*
     * 현재의 시간을 얻는다.
     */
    time(&clock);
    tm = gmtime(&clock);

    /* 지역 시간대를 설정합니다*/    
    setlocale(LC_TIME, "kr");

    /*
     * 각 형식 지정자를 적용해 봅니다.
     */
    for (j=0; formats[j] != NULL; j++) {
        strftime(buf, sizeof(buf), formats[j], tm);
        printf("\t%-25s %s\n", formats[j], buf);
    }

    exit(0);
}
