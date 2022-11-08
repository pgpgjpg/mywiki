#include <stdio.h>
#include <sys/times.h>
#include <time.h>

int main(int argc, char** argv)
{
    struct tm *newtime;
    time_t now, oldTime;
    char* week[] = {"일", "월", "화", "수", "목", "금", "토"};
    /*현재의 시간을 얻는다 */
    time( &now );
    
    /*time_t를 지역시간으로 변경한다.*/
    newtime = localtime( &now ); 
    
    printf("%s\n",  asctime(newtime));
    printf("%s\n",  ctime(&now));
    
      
    return 0;    
    
}
