#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
	char *tempName;
	char template[32];
    int i;
    FILE* fp;
    
    /*임시 파일명 생성 */    
    printf("mktemp() 함수 \n");
    for (i=0;i<5;i++) {
        strcpy(template, "/tmp/mytempXXXXXX");
    	tempName = mktemp(template);
    	
    	printf("임시 파일명 : %s\n", tempName);
    }

    printf("mkstemp() 함수 \n");
    for (i=0;i<5;i++) {
        strcpy(template, "/tmp/mytempXXXXXX");
    	mkstemp(template);
    	
    	printf("mkstemp() : %s\n", tempName);
    }
    
    return 0;    
    
}
