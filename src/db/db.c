#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

DB *newDB()
{
    DB *tmp = (DB*)malloc(sizeof(DB));

    tmp->m_this = tmp;
    tmp->deleteDB = deleteDB_; // 소멸자
    tmp->save = save_;
    tmp->remove = remove_;
    tmp->getTag = getTag_;
    tmp->load = load_;     
    tmp->isInDB = isInDB_;

    int nErr;
  
    nErr = hashCreate(&tmp->m_lpHashTag);
	if (ERR_HASH_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
		return 0;
	}
    
    //hash 테이블에 메모리를 해제 할 수 있는 함수를 등록한다.
    nErr = hashSetFree(tmp->m_lpHashTag, (void(*)(void*))free);
	if (ERR_HASH_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
		return 0;
	}

    return tmp;
}

void deleteDB_(DB *lpDB)
{
    free(lpDB);
}

int load_(DB *lpDB)
{    
	LPPROFILE lpProfile;
    int nErr, dCount = 0;
    char  szBuf[256];
    
    //프로파일 메모리를 할당한다.    
    nErr = profileCreate(&lpProfile, DB_PATH);
    if (ERR_PROFILE_OK != nErr) {
        return nErr;
    }
 
    profileGetIntValue(lpProfile, DATA_COUNT, &dCount);
    lpDB->m_nData = dCount;

    lpDB->m_lpData = (Data*)calloc(dCount, sizeof(Data));    
    if (NULL == lpDB->m_lpData) {
        //프로파일 구조체 메모리를 해제한다.
        profileDestroy(lpProfile);
        return -1;
    }
    
    for(int i = 0; i < lpDB->m_nData; ++i){        
        char *tmpTags;
        sprintf(szBuf, "%s_%d", FILE_TITLE, i+1);        
        profileGetStringValue(lpProfile, szBuf, &lpDB->m_lpData[i].title);

        sprintf(szBuf, "%s_%d", FILE_DATA, i+1);        
        profileGetStringValue(lpProfile, szBuf, &lpDB->m_lpData[i].data);

        sprintf(szBuf, "%s_%d", FILE_PATH, i+1);        
        profileGetStringValue(lpProfile, szBuf, &lpDB->m_lpData[i].file);

        sprintf(szBuf, "%s_%d", FILE_TAGS, i+1);        
        profileGetStringValue(lpProfile, szBuf, &tmpTags);      

        arrayCreate(&lpDB->m_lpData[i].tags);
        char *str = tmpTags;
        char *val;
        while (1) {	    
            if (0 == str[0]) {
                break;
            }
            if (0 != str[0]) {                
                val = strtok(str, " ");           
                str = val + strlen(val) + 1;                                
                arrayAdd(lpDB->m_lpData[i].tags, (const LPDATA) val);   
            }            
	    } 
    }    
    
    for(int i = 0; i < lpDB->m_nData; ++i){        
        // printf("[%d]title : %s\n", i+1, lpDB->m_lpData[i].title);
        // printf("[%d]data : %s\n", i+1, lpDB->m_lpData[i].data);
        // printf("[%d]file : %s\n", i+1, lpDB->m_lpData[i].file);        
        
        for(int j = 0; j < lpDB->m_lpData[i].tags->size; ++j){
            char *tmpChar;
            arrayGetAt(lpDB->m_lpData[i].tags, j, (LPDATA*) &tmpChar);
            //printf("[%d]tags : %s\n", i+1, tmpChar);            
        }
    }   
    
    // key : hashTag, value : title
    for(int i = 0; i < lpDB->m_nData; i++) {
        for(int j = 0; j < lpDB->m_lpData[i].tags->size; ++j){
            char *tag;
            arrayGetAt(lpDB->m_lpData[i].tags, j, (LPDATA*) &tag);
            nErr = hashSetValue(lpDB->m_lpHashTag, tag, (LPDATA) lpDB->m_lpData[i].title);
            
            if (ERR_HASH_OK != nErr) {
                printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
    	    }
        }      
    }

    POSITION pos;
    //hash 테이블의 처음 위치를 얻는다.
	nErr = hashGetFirstPostion(lpDB->m_lpHashTag, &pos);
	if (ERR_HASH_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
	}
	//다음 위치로 이동하여 
	while (1) {        
        char *tmpKey;
        char *tmpVal;
        LPARRAY tmpArr;     
        // 마지막 매개 변수는 char* 반환임! 난 array를 받아야 함
		nErr = hashGetNextPostion(lpDB->m_lpHashTag, &pos, &tmpKey, (LPDATA*)&tmpVal, &tmpArr);
        
		if (ERR_HASH_OK != nErr) {
			printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
			break;
		}
        //printf("tmpArr Size : %d\n", tmpArr->size);
        for(int j = 0; j < tmpArr->size; ++j){
            char *tmpChar;
            arrayGetAt(tmpArr, j, (LPDATA*) &tmpChar);
            //printf("key[%s] : %s\n", tmpKey, tmpChar);            
        }
		
		//다음 위치가 없음 while 루프를 종료한다.
		if (NULL == pos) {
			break;
		}
	}
}

void save_(DB *lpDB, Data *lpData)
{
    FILE *fp = fopen(DB_PATH, "r");	
	if (NULL == fp) {
	    printf("%s 파일을 열수 없습니다\n", "db.txt");
	    exit(1);
	}    
    fseek(fp, 0, SEEK_END);  
    char *buf;
    int fSize = ftell(fp)+1000;
    buf = (char*)malloc(sizeof(char)*fSize*2);    
    fseek(fp, 0, SEEK_SET);  
 
    if(fread(buf, 1, fSize, fp) == 0) {
        printf("%s 파일을 읽을 수 없습니다\n", "db.txt");
        exit(1);
    }
    fclose(fp);

    fp = fopen(DB_PATH, "w");	    
    char strToFind[100];

    if(lpData != NULL){
        sprintf(strToFind, "=%s\nFILE_DATA_", lpData->title);
        char *lpStr = strstr(buf, strToFind);
        
        if(lpStr == NULL){        
            lpDB->m_nData++;
            lpDB->m_lpData = (Data*)realloc(lpDB->m_lpData, sizeof(Data)*lpDB->m_nData*2);
            lpDB->m_lpData[lpDB->m_nData - 1].title = (char*)malloc(sizeof(char)*100);
            lpDB->m_lpData[lpDB->m_nData - 1].data = (char*)malloc(sizeof(char)*100);
            lpDB->m_lpData[lpDB->m_nData - 1].file = (char*)malloc(sizeof(char)*100);


            strcpy(lpDB->m_lpData[lpDB->m_nData - 1].title, lpData->title);
            strcpy(lpDB->m_lpData[lpDB->m_nData - 1].data, lpData->data);
            strcpy(lpDB->m_lpData[lpDB->m_nData - 1].file, lpData->file);
            lpDB->m_lpData[lpDB->m_nData - 1].tags = lpData->tags;
        }
    }
    
    memset(buf, 0, sizeof(char)*fSize*2);    

    sprintf(buf + strlen(buf), "# 환경설정 파일을 예제 입니다.\n");
    sprintf(buf + strlen(buf), "# #은 주석으로 사용합니다.\n");
    sprintf(buf + strlen(buf), "# hash 테이블에서 발생한 오류를 문자열로 파일로 관리합니다\n\n");
    sprintf(buf + strlen(buf), "-2001=hash 구조체 메모리가 아닙니다.\n");
    sprintf(buf + strlen(buf), "-2002=hash 함수에서 메모리를 할당 받다 오류가 발생하였습니다.\n");
    sprintf(buf + strlen(buf), "-2002=hash 함수에서 메모리를 할당 받다 오류가 발생하였습니다.\n");
    sprintf(buf + strlen(buf), "-2003=hash 함수에 자료를 찾을 수가 없습니다.\n\n");

    sprintf(buf + strlen(buf), "DATA_COUNT=%d\n", lpDB->m_nData);
    int flag = 0;
    for(int i = 0; i < lpDB->m_nData; ++i){     
        sprintf(buf + strlen(buf), "#########################################################\n");
        sprintf(buf + strlen(buf), "FILE_TITLE_%d=%s\n", i+1, lpDB->m_lpData[i].title);
        if(lpData != NULL && !strcmp(lpDB->m_lpData[i].title, lpData->title)){
            sprintf(buf + strlen(buf), "FILE_DATA_%d=%s\n", i+1, lpData->data);
            sprintf(buf + strlen(buf), "FILE_PATH_%d=%s\n", i+1, lpData->file);
            sprintf(buf + strlen(buf), "FILE_TAGS_%d=", i+1);
            for(int j = 0; j < lpData->tags->size; ++j){
                char *tmpChar;
                arrayGetAt(lpData->tags, j, (LPDATA*) &tmpChar);
                sprintf(buf + strlen(buf), "%s ", tmpChar);
            }
            flag = 1;
        }else{
            sprintf(buf + strlen(buf), "FILE_DATA_%d=%s\n", i+1, lpDB->m_lpData[i].data);
            sprintf(buf + strlen(buf), "FILE_PATH_%d=%s\n", i+1, lpDB->m_lpData[i].file);        
            sprintf(buf + strlen(buf), "FILE_TAGS_%d=", i+1);
            for(int j = 0; j < lpDB->m_lpData[i].tags->size; ++j){
                char *tmpChar;
                arrayGetAt(lpDB->m_lpData[i].tags, j, (LPDATA*) &tmpChar);
                sprintf(buf + strlen(buf), "%s ", tmpChar);
            }            
        }       
        *(buf + strlen(buf) - 1) = '\n';    
    }    

    fwrite(buf, sizeof(char), strlen(buf), fp);    
    fclose(fp);
    free(buf);   

    lpDB->load(lpDB);
}

int isInDB_(DB *lpDB, char *title)
{    
    for(int i = 0; i < lpDB->m_nData; ++i){ 
        if(!strcmp(lpDB->m_lpData[i].title, title))
            return 1;        
    }
    return 0;
}

void remove_(DB *lpDB, char *title)
{
    for(int i = 0; i < lpDB->m_nData; ++i){
        if(!strcmp(lpDB->m_lpData[i].title, title)){            
            memmove(lpDB->m_lpData + i, lpDB->m_lpData + i + 1, sizeof(Data)*(lpDB->m_nData - (i+1)));
            lpDB->m_nData--;
            break;
        }
    }
}

void getTag_(DB *lpDB){}