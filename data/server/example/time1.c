#include <stdio.h>
#include <sys/times.h>
#include <time.h>

int main(int argc, char** argv)
{
    struct tm *newtime;
    time_t now;
    char* week[] = {"일", "월", "화", "수", "목", "금", "토"};
    /*현재의 시간을 얻는다 */
    time( &now );
    
    /*time_t를 지역시간으로 변경한다.*/
    newtime = localtime( &now ); 
    
    printf("tm_sec  = %d초\n", newtime->tm_sec  );
    printf("tm_min  = %d분\n", newtime->tm_min  );
    printf("tm_hour = %d시\n", newtime->tm_hour );
    printf("tm_mday = %d일\n", newtime->tm_mday );
    printf("tm_mon  = %d월\n", newtime->tm_mon  + 1);
    printf("tm_year = %d년\n", newtime->tm_year + 1900);
    printf("tm_wday = %s요일\n", week[newtime->tm_wday] );
    printf("tm_yday = %d일자\n", newtime->tm_yday );
    printf("tm_isdst= %d\n", newtime->tm_isdst);
      
    return 0;    
    
}
