#include <stdio.h>
#include <sys/types.h>
#include <time.h>

int main(void)
{
	time_t c_time;
	struct tm *l_time;
	char line[1024];
	
	time(&c_time);
	printf("c_time : %d\n",(int) c_time);
	l_time = localtime(&c_time);
	printf("%d-%d-%d\n",
	   l_time->tm_year+1900, l_time->tm_mon+1, l_time->tm_mday);
	strftime(line, 1023, "%Y-%m-%d %I:%M %p", l_time);
	printf("line : %s\n", line);
	return 0;
	
}
