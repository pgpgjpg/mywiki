#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "finder.h"

Finder *newFinder()
{
    Finder *tmp = (Finder*)malloc(sizeof(Finder));

    tmp->m_this = tmp;
    tmp->initFinder = initFinder_; 
    tmp->deleteFinder = deleteFinder_; // 소멸자
    tmp->searchTitleByTag = searchTitleByTag_;
    tmp->loadDataByTitle = loadDataByTitle_;
    tmp->showData = showData_;
    tmp->showTitles = showTitles_;

    return tmp;
}

void deleteFinder_(Finder *lpFinder)
{
    free(lpFinder);
}

int initFinder_(Finder *lpFinder) 
{
    int nErr = hashCreate(&lpFinder->m_lpHashTitle);
	if (ERR_HASH_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
		return 0;
	}
    
    //hash 테이블에 메모리를 해제 할 수 있는 함수를 등록한다.
    nErr = hashSetFree(lpFinder->m_lpHashTitle, (void(*)(void*))free);
	if (ERR_HASH_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
		return 0;
	}

    return 1;
}

int searchTitleByTag_(Finder *lpFinder, LPHASH lpHash, char *tag) 
{
    POSITION pos;
    int nErr;

    //hash 테이블의 처음 위치를 얻는다.
	nErr = hashGetFirstPostion(lpHash, &pos);
	if (ERR_HASH_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
	}
	//다음 위치로 이동하여 
	while (1) {        
        char *tmpKey;
        char *tmpVal;
        LPARRAY tmpArr;     
        // 마지막 매개 변수는 char* 반환임! 난 array를 받아야 함
		nErr = hashGetNextPostion(lpHash, &pos, &tmpKey, (LPDATA*)&tmpVal, &tmpArr);        
		if (ERR_HASH_OK != nErr) {
			printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
			break;
		}

        if(!strcmp(tmpKey, tag)){
            for(int j = 0; j < tmpArr->size; ++j){
                char *tmpTitle;
                arrayGetAt(tmpArr, j, (LPDATA*) &tmpTitle);
                nErr = hashSetValue(lpFinder->m_lpHashTitle, tmpTitle, NULL);
            }   
            return 1;
        }
        
		//다음 위치가 없음 while 루프를 종료한다.
		if (NULL == pos) {
			return 0;
		}
	}
}

int loadDataByTitle_(Finder *lpFinder, Data *lpData, int nData, char *title) 
{
    for(int i = 0; i < nData; ++i){
        if(!strcmp(lpData[i].title, title)){
            lpFinder->m_data = lpData[i];
            return 1;
        }
    }
    return 0;
}

void showData_(Finder *lpFinder)
{
    printf("title : %s\n", lpFinder->m_data.title);
    printf("data : %s\n", lpFinder->m_data.data);
    printf("file : %s\n", lpFinder->m_data.file);
    printf("tags : ");              
    for(int i = 0; i < lpFinder->m_data.tags->size; ++i){
        char *tmpChar;
        arrayGetAt(lpFinder->m_data.tags, i, (LPDATA*) &tmpChar);
        printf("%s ", tmpChar);            
    }
    printf("\n");
}

void showTitles_(Finder *lpFinder)
{
    POSITION pos;
    int nErr;
    //hash 테이블의 처음 위치를 얻는다.
	nErr = hashGetFirstPostion(lpFinder->m_lpHashTitle, &pos);
	if (ERR_HASH_OK != nErr) {
		printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
	}
	//다음 위치로 이동하여 
	while (1) {        
        char *tmpKey;
        char *tmpVal;
        LPARRAY tmpArr;     
        // 마지막 매개 변수는 char* 반환임! 난 array를 받아야 함
		nErr = hashGetNextPostion(lpFinder->m_lpHashTitle, &pos, &tmpKey, (LPDATA*)&tmpVal, &tmpArr);
		if (ERR_HASH_OK != nErr) {
			printf("%s:%d error code = %d\n",__FILE__, __LINE__, nErr);
			break;
		}
        printf("%s ", tmpKey);
		
		//다음 위치가 없음 while 루프를 종료한다.
		if (NULL == pos) {
			break;
		}
	}
    printf("\n");
}