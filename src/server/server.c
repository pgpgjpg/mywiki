#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <pthread.h>
#include "server.h"
#include "finder.h"

Server *newServer()
{
    Server *tmp = (Server*)malloc(sizeof(Server));

    tmp->set = set_;
    tmp->run = run_;
    tmp->deleteServer = deleteServer_;

    return tmp;
}

void deleteServer_(Server *lpServer)
{
    free(lpServer);
}

void set_(Server *lpServer)
{    
	int yes=1;	

	if((lpServer->m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	if(setsockopt(lpServer->m_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}

	lpServer->m_server_addr.sin_family = AF_INET;
	lpServer->m_server_addr.sin_port = htons(60000);
	lpServer->m_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(lpServer->m_server_addr.sin_zero), '\0', 8);

	if(bind(lpServer->m_sockfd, (struct sockaddr *)&lpServer->m_server_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}

	if(listen(lpServer->m_sockfd, BACKLOG) == -1) 
	{
		perror("listen");
		exit(1);
	}
}

void run_(Server *lpServer)
{
    int sin_size;
    pthread_t tid;

    while(1)
	{
		sin_size = sizeof(struct sockaddr_in);

		if((lpServer->m_new_fd = accept(lpServer->m_sockfd, (struct sockaddr *) &lpServer->m_client_addr, &sin_size)) == -1)
		{
			perror("accept");
			continue;
		}
		printf("server : got connection from %s \n", inet_ntoa(lpServer->m_client_addr.sin_addr));
		if(pthread_create(&tid, NULL, start_main, &lpServer->m_new_fd)!=0) {
			perror("pthread_create");
		} else {
			pthread_detach(tid);
		}
	}
}

void* start_main(void* arg)
{
	char key,  menus[4][10] = {"Search", "Upload", "Revise", "Exit"};
	int sd = *(int*)arg, menuIdx = 0;
	Map *lpMap = newMap(); 		
    DB *lpDB = newDB();
	lpDB->load(lpDB);

    //무한 반복을 하면서 처리를 메뉴를 입력 반는다.    
    while (1) {
		lpMap->drawFrame(lpMap);
		lpMap->drawText(lpMap, (int)(lpMap->m_rows*1/6 + 0.5), lpMap->m_cols/2 - 3, "Search");
		lpMap->drawText(lpMap, (int)(lpMap->m_rows*2/6 + 0.5), lpMap->m_cols/2 - 3, "Upload");
		lpMap->drawText(lpMap, (int)(lpMap->m_rows*3/6 + 0.5), lpMap->m_cols/2 - 3, "Revise");
		lpMap->drawText(lpMap, (int)(lpMap->m_rows*4/6 + 1), lpMap->m_cols/2 - 2, "Exit");
        char *str = strstr(lpMap->m_map, menus[menuIdx]);
        if(str != NULL) *(str -2) = '*';
        send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);        

		keyCallback(sd, &key);

		switch(key){
			case 'u' : 				
			case 'l' : 
				if(menuIdx != 0) menuIdx--; 
		 		else menuIdx = 3;
				break;
			case 'd' : 				
			case 'r' : 
				if(menuIdx != 3) menuIdx++;
		 		else menuIdx = 0;
				break;

			case '\n' : 
				if(menuIdx == 0){
					// Search Alg
					printf("Client accessed \"Search\"\n");
					server_search(sd, lpMap, lpDB);
					sleep(1);
				}else if(menuIdx == 1){
					// Upload Alg
					printf("Client accessed \"Upload\"\n");
					server_upload(sd, lpMap, lpDB);
					sleep(1);
				}else if(menuIdx == 2){
					// Revise Alg
					printf("Client accessed \"Revise\"\n");
					server_revise(sd, lpMap);
					sleep(1);
				}else{
					// Exit
					exit(0);
				}				
				break;
			default:
				break;								
		}		
	}
	
	return NULL;
}

void server_search(int sd, Map *lpMap, DB *lpDB) 
{	
	Finder *lpFinder = newFinder();
    lpFinder->initFinder(lpFinder);

	// 검색할 tag 입력
	char ques[100] = "Enter hash tag to search";
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);

	char ans[MAX_LENGTH];
	char displayTags[MAX_LENGTH] = "";
	int rForDisplay = 0;
	LPARRAY lpArr;
	arrayCreate(&lpArr);
	while(1){
		memset(ans, 0, MAX_LENGTH);
		while(1){	
			appendCharCallback(sd, ans);		
			if(ans[strlen(ans) - 1] == '\n') break;		
			lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen(ans)/2, ans);
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);	
		}
		
		if(strlen(ans) == 1) break;
		ans[strlen(ans) - 1] = '\0';

		if(!lpFinder->searchTitleByTag(lpFinder, lpDB->m_lpHashTag, ans)){			
			lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen("                ")/2, "                ");
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
			continue;
		} 

		
		lpArr->size = 0;
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
			arrayAdd(lpArr, tmpKey);
			if (NULL == pos) {
				break;
			}
		}
		
		rForDisplay = 0;
		lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
		lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
		memset(displayTags, 0, MAX_LENGTH);

		for(int i = 0; i < lpArr->size; ++i){
			char *tmpTag;
            arrayGetAt(lpArr, i, (LPDATA*) &tmpTag);
			if(displayTags[0] == '\0'){
				strcpy(displayTags, tmpTag);
				lpMap->drawText(lpMap, 9 + rForDisplay, lpMap->m_cols/2 - strlen(displayTags)/2, displayTags);
			}else{
				if(strlen(displayTags) + strlen(tmpTag) > lpMap->m_cols*0.8){
					rForDisplay += 2;
					memset(displayTags, 0, MAX_LENGTH);
					strcpy(displayTags, tmpTag);
					lpMap->drawText(lpMap, 9 + rForDisplay, lpMap->m_cols/2 - strlen(displayTags)/2, displayTags);
				}else{
					strcat(displayTags, "   ");
					strcat(displayTags, tmpTag);
					lpMap->drawText(lpMap, 9 + rForDisplay, lpMap->m_cols/2 - strlen(displayTags)/2, displayTags);				
				}
			}
		}
		lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen("                ")/2, "                ");
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
		//arrayDestroy(lpArr);
	}		

	// 열어볼 제목 입력
	Data data;
	data.title = (char*)malloc(sizeof(char)*MAX_LENGTH);
	data.data = (char*)malloc(sizeof(char)*MAX_LENGTH);
	data.file = (char*)malloc(sizeof(char)*MAX_LENGTH);	
	strcpy(ques, "Enter title");		
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen("                          ")/2, "                          ");
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);		
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);	
	
	
	while(1){
		memset(ans, 0, MAX_LENGTH);
		while(1){	
			appendCharCallback(sd, ans);		
			if(ans[strlen(ans) - 1] == '\n') break;
			lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen(ans)/2, ans);
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
		}
		ans[strlen(ans) - 1] = '\0';
		strcpy(data.title, ans);
		
		if(!lpFinder->loadDataByTitle(lpFinder, lpDB->m_lpData, lpDB->m_nData, data.title)){
			lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen("                ")/2, "                ");
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
			continue;
		}

		char buf[MAX_LENGTH] = "";
		strcpy(buf, "------- 제  목 -------\n");
		strcat(buf, lpFinder->m_data.title);
		strcat(buf, "\n");
		strcat(buf, "\n------- 내  용 -------\n");
		strcat(buf, lpFinder->m_data.data);
		strcat(buf, "\n");		
		
		for(int i = 0; i < lpFinder->m_data.tags->size; ++i){
			char *tmpTag;
            arrayGetAt(lpFinder->m_data.tags, i, (LPDATA*) &tmpTag);
			strcat(buf, "#");
			strcat(buf, tmpTag);
			strcat(buf, "    ");
		}
		strcat(buf, "\n\n");
		strcat(buf, "Do you want to open example file?[y/n] --> ");
		send(sd, buf, strlen(buf), 0);		

		break;
	}	
	int key;
	recv(sd, &key, sizeof(key), 0);

	if(key == 'y'){
		char buf[MAX_LENGTH] = "";
		FILE *fp = fopen(lpFinder->m_data.file, "r");	
		if (NULL == fp) {
			printf("%s 파일을 열수 없습니다\n", "db.txt");
			exit(1);
		}    					
		if(fread(buf, 1, MAX_LENGTH, fp) == 0) {
			printf("%s 파일을 읽을 수 없습니다\n", "db.txt");
			exit(1);
		}
		fclose(fp);
		send(sd, buf, strlen(buf), 0);	
		recv(sd, &key, sizeof(key), 0);

		strcpy(ques, "Do you want to save example file?[y/n] --> ");
		lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
		lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);	
		recv(sd, &key, sizeof(key), 0);

		if(key == 'y'){			
			char tmp[100];
			strcpy(tmp, lpFinder->m_data.title);
			strcat(tmp, ".c.example^"); // ^ : save 명령			
			send(sd, tmp, strlen(tmp), 0);	
			sleep(2);
			printf("%s file saving...\n", tmp);
			send(sd, buf, strlen(buf), 0);	
			printf("complete\n");
		}	
	}	
}

void server_upload(int sd, Map *lpMap, DB *lpDB) 
{
	Data data;
	data.title = (char*)malloc(sizeof(char)*MAX_LENGTH);
	data.data = (char*)malloc(sizeof(char)*MAX_LENGTH);
	data.file = (char*)malloc(sizeof(char)*MAX_LENGTH);

	// 업로드할 제목 입력
	char ques[100] = "Enter title to upload";
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);

	char ans[MAX_LENGTH];
	memset(ans, 0, MAX_LENGTH);
	while(1){	
		appendCharCallback(sd, ans);		
		if(ans[strlen(ans) - 1] == '\n') break;
		lpMap->drawText(lpMap, 10, lpMap->m_cols/2 - strlen(ans)/2, ans);
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	}
	ans[strlen(ans) - 1] = '\0';
	strcpy(data.title, ans);

	// 업로드할 내용 입력
	strcpy(ques, "Enter data");	
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);

	memset(ans, 0, MAX_LENGTH);
	while(1){	
		appendCharCallback(sd, ans);		
		if(ans[strlen(ans) - 1] == '\n') break;
		lpMap->drawText(lpMap, 10, lpMap->m_cols/2 - strlen(ans)/2, ans);
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	}
	ans[strlen(ans) - 1] = '\0';
	strcpy(data.data, ans);

	// Hash tag 입력
	strcpy(ques, "Enter tags");
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	
	LPARRAY lpArr;
	arrayCreate(&lpArr);
	char displayTags[MAX_LENGTH] = "";
	int rForDisplay = 0;
	char *addVal;
	while(1){
		memset(ans, 0, MAX_LENGTH);
		while(1){	
			appendCharCallback(sd, ans);		
			if(ans[strlen(ans) - 1] == '\n') break;		
			lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen(ans)/2, ans);
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);	
		}
		
		if(strlen(ans) == 1) break;
		ans[strlen(ans) - 1] = '\0';

		addVal = (char*)malloc(sizeof(char)*strlen(ans));
		strcpy(addVal, ans);
		arrayAdd(lpArr, addVal);				
		//printf("addVal address : %p\n", addVal);

		rForDisplay = 0;
		lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
		lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
		memset(displayTags, 0, MAX_LENGTH);

		for(int i = 0; i < lpArr->size; ++i){
			char *tmpTag;
            arrayGetAt(lpArr, i, (LPDATA*) &tmpTag);
			if(displayTags[0] == '\0'){
				strcpy(displayTags, tmpTag);
				lpMap->drawText(lpMap, 9 + rForDisplay, lpMap->m_cols/2 - strlen(displayTags)/2, displayTags);
			}else{
				if(strlen(displayTags) + strlen(tmpTag) > lpMap->m_cols*0.8){
					rForDisplay += 2;
					memset(displayTags, 0, MAX_LENGTH);
					strcpy(displayTags, tmpTag);
					lpMap->drawText(lpMap, 9 + rForDisplay, lpMap->m_cols/2 - strlen(displayTags)/2, displayTags);
				}else{
					strcat(displayTags, " ");
					strcat(displayTags, tmpTag);
					lpMap->drawText(lpMap, 9 + rForDisplay, lpMap->m_cols/2 - strlen(displayTags)/2, displayTags);				
				}
			}
		}
		lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen("                ")/2, "                ");
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	}
	//getchar();
	
	data.tags = lpArr;	

	// 첨부파일 입력	
	strcpy(ques, "If you want to upload file, enter file path+name");	
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거	
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
	strcpy(ques, "If not, just push enter key");	
	lpMap->drawText(lpMap, 6, lpMap->m_cols/2 - strlen(ques)/2, ques);	
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);

	memset(ans, 0, MAX_LENGTH);
	while(1){	
		appendCharCallback(sd, ans);		
		if(ans[strlen(ans) - 1] == '\n') break;
		lpMap->drawText(lpMap, 10, lpMap->m_cols/2 - strlen(ans)/2, ans);
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	}
	if(strlen(ans) == 1) strcpy(data.file, "NULL");
	else{
		char tmp[100] = "";
		char buffer[50000] = "";
		ans[strlen(ans) - 1] = '\0';
		strcpy(data.file, ans);		
		strcpy(tmp, data.file);
		strcat(tmp, "#");
		send(sd, tmp, strlen(tmp), 0);					
		recv(sd, buffer, 50000, 0);
		printf("file name : %s\n", tmp);
		printf("file data : %s\n", buffer);
		if(strcmp(buffer, "NULL")){
			char tmpTitle[100] = "";
			strcpy(tmpTitle, "../db/example/");
			strcat(tmpTitle, data.file);
			
			FILE *fp = fopen(tmpTitle, "w");	
			printf("saving... in %s\n", tmpTitle);
			//int n = recv(sd, buffer, MAX_LENGTH, 0);					
			fwrite(buffer, sizeof(char), 50000, fp);			
			printf("complete\n");
			fclose(fp);
		}
	}

	

	
	//arrayCopy(&data.tags, lpArr);	
	lpDB->save(lpDB, &data);
	
	// printf("title : %s\n", data.title);
	// printf("data : %s\n", data.data);
	// printf("file : %s\n", data.file);
	// for(int j = 0; j < data.tags->size; ++j){		
	// 	char *tmpChar;
	// 	arrayGetAt(data.tags, j, (LPDATA*) &tmpChar);
	// 	printf("[%d] %s\n", j, tmpChar);            
	// }
}
void server_revise(int sd, Map *lpMap, DB *lpDB) 
{
	Finder *lpFinder = newFinder();
    lpFinder->initFinder(lpFinder);

	// 검색할 tag 입력
	char ques[100] = "Enter hash tag to search";
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);

	char ans[MAX_LENGTH];
	char displayTags[MAX_LENGTH] = "";
	int rForDisplay = 0;
	LPARRAY lpArr;
	arrayCreate(&lpArr);
	while(1){
		memset(ans, 0, MAX_LENGTH);
		while(1){	
			appendCharCallback(sd, ans);		
			if(ans[strlen(ans) - 1] == '\n') break;		
			lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen(ans)/2, ans);
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);	
		}
		
		if(strlen(ans) == 1) break;
		ans[strlen(ans) - 1] = '\0';

		if(!lpFinder->searchTitleByTag(lpFinder, lpDB->m_lpHashTag, ans)){			
			lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen("                ")/2, "                ");
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
			continue;
		} 


		
		lpArr->size = 0;
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
			arrayAdd(lpArr, tmpKey);
			if (NULL == pos) {
				break;
			}
		}
		
		rForDisplay = 0;
		lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
		lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
		memset(displayTags, 0, MAX_LENGTH);

		for(int i = 0; i < lpArr->size; ++i){
			char *tmpTag;
            arrayGetAt(lpArr, i, (LPDATA*) &tmpTag);
			if(displayTags[0] == '\0'){
				strcpy(displayTags, tmpTag);
				lpMap->drawText(lpMap, 9 + rForDisplay, lpMap->m_cols/2 - strlen(displayTags)/2, displayTags);
			}else{
				if(strlen(displayTags) + strlen(tmpTag) > lpMap->m_cols*0.8){
					rForDisplay += 2;
					memset(displayTags, 0, MAX_LENGTH);
					strcpy(displayTags, tmpTag);
					lpMap->drawText(lpMap, 9 + rForDisplay, lpMap->m_cols/2 - strlen(displayTags)/2, displayTags);
				}else{
					strcat(displayTags, "   ");
					strcat(displayTags, tmpTag);
					lpMap->drawText(lpMap, 9 + rForDisplay, lpMap->m_cols/2 - strlen(displayTags)/2, displayTags);				
				}
			}
		}
		lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen("                ")/2, "                ");
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
		//arrayDestroy(lpArr);
	}		

	// 열어볼 제목 입력
	Data data;
	data.title = (char*)malloc(sizeof(char)*MAX_LENGTH);
	data.data = (char*)malloc(sizeof(char)*MAX_LENGTH);
	data.file = (char*)malloc(sizeof(char)*MAX_LENGTH);	
	strcpy(ques, "Enter title");		
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen("                          ")/2, "                          ");
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);		
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);	
	
	
	while(1){
		memset(ans, 0, MAX_LENGTH);
		while(1){	
			appendCharCallback(sd, ans);		
			if(ans[strlen(ans) - 1] == '\n') break;
			lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen(ans)/2, ans);
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
		}
		ans[strlen(ans) - 1] = '\0';
		strcpy(data.title, ans);
		
		if(!lpFinder->loadDataByTitle(lpFinder, lpDB->m_lpData, lpDB->m_nData, data.title)){
			lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen("                ")/2, "                ");
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
			continue;
		}
		break;	
	}	
	
	// 업로드할 내용 입력
	strcpy(ques, "Enter data");	
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);

	memset(ans, 0, MAX_LENGTH);
	while(1){	
		appendCharCallback(sd, ans);		
		if(ans[strlen(ans) - 1] == '\n') break;
		lpMap->drawText(lpMap, 10, lpMap->m_cols/2 - strlen(ans)/2, ans);
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	}
	ans[strlen(ans) - 1] = '\0';
	strcpy(data.data, ans);

	// Hash tag 입력
	strcpy(ques, "Enter tags");
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	
	LPARRAY lpArr;
	arrayCreate(&lpArr);
	char displayTags[MAX_LENGTH] = "";
	int rForDisplay = 0;
	char *addVal;
	while(1){
		memset(ans, 0, MAX_LENGTH);
		while(1){	
			appendCharCallback(sd, ans);		
			if(ans[strlen(ans) - 1] == '\n') break;		
			lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen(ans)/2, ans);
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);	
		}
		
		if(strlen(ans) == 1) break;
		ans[strlen(ans) - 1] = '\0';

		addVal = (char*)malloc(sizeof(char)*strlen(ans));
		strcpy(addVal, ans);
		arrayAdd(lpArr, addVal);				
		//printf("addVal address : %p\n", addVal);

		rForDisplay = 0;
		lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
		lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
		memset(displayTags, 0, MAX_LENGTH);

		for(int i = 0; i < lpArr->size; ++i){
			char *tmpTag;
            arrayGetAt(lpArr, i, (LPDATA*) &tmpTag);
			if(displayTags[0] == '\0'){
				strcpy(displayTags, tmpTag);
				lpMap->drawText(lpMap, 9 + rForDisplay, lpMap->m_cols/2 - strlen(displayTags)/2, displayTags);
			}else{
				if(strlen(displayTags) + strlen(tmpTag) > lpMap->m_cols*0.8){
					rForDisplay += 2;
					memset(displayTags, 0, MAX_LENGTH);
					strcpy(displayTags, tmpTag);
					lpMap->drawText(lpMap, 9 + rForDisplay, lpMap->m_cols/2 - strlen(displayTags)/2, displayTags);
				}else{
					strcat(displayTags, " ");
					strcat(displayTags, tmpTag);
					lpMap->drawText(lpMap, 9 + rForDisplay, lpMap->m_cols/2 - strlen(displayTags)/2, displayTags);				
				}
			}
		}
		lpMap->drawText(lpMap, 7, lpMap->m_cols/2 - strlen("                ")/2, "                ");
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	}
	//getchar();
	
	data.tags = lpArr;	

	// 첨부파일 입력	
	strcpy(ques, "If you want to upload file, enter file path+name");	
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거	
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);	
	strcpy(ques, "If not, just push enter key");	
	lpMap->drawText(lpMap, 6, lpMap->m_cols/2 - strlen(ques)/2, ques);	
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);

	memset(ans, 0, MAX_LENGTH);
	while(1){	
		appendCharCallback(sd, ans);		
		if(ans[strlen(ans) - 1] == '\n') break;
		lpMap->drawText(lpMap, 10, lpMap->m_cols/2 - strlen(ans)/2, ans);
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	}
	if(strlen(ans) == 1) strcpy(data.file, "NULL");
	else{
		char tmp[100] = "";
		char buffer[50000] = "";
		ans[strlen(ans) - 1] = '\0';
		strcpy(data.file, ans);		
		strcpy(tmp, data.file);
		strcat(tmp, "#");
		send(sd, tmp, strlen(tmp), 0);					
		recv(sd, buffer, 50000, 0);
		printf("file name : %s\n", tmp);
		printf("file data : %s\n", buffer);
		if(strcmp(buffer, "NULL")){
			char tmpTitle[100] = "";
			strcpy(tmpTitle, "../db/example/");
			strcat(tmpTitle, data.file);
			
			FILE *fp = fopen(tmpTitle, "w");	
			printf("saving... in %s\n", tmpTitle);
			//int n = recv(sd, buffer, MAX_LENGTH, 0);					
			fwrite(buffer, sizeof(char), 50000, fp);			
			printf("complete\n");
			fclose(fp);
		}
	}

	

	
	//arrayCopy(&data.tags, lpArr);	
	lpDB->save(lpDB, &data);



}

void keyCallback(int sd, char *key)
{	
	static int key1, key2, key3;	
	int idx = 0, n;
	recv(sd, &key1, sizeof(key1), 0);

	if(key1 == '\n'){		
		*key = key1;
		return;
	}			
	else if(key1 != 27) return;

	recv(sd, &key2, sizeof(key2), 0);
	if(key2 != 91) return;

	recv(sd, &key3, sizeof(key3), 0);
	if(key3 < 65 || key3 > 68) return;     
	
	switch(key3){
		case 65 : 
			*key = 'u';			
			break;
		case 68 :
			*key = 'l';
			break;
		case 66 :  
			*key = 'd';
			break;              
		case 67 :
			*key = 'r';
			break;
	}
}

void appendCharCallback(int sd, char* str)
{	
	int ch;
	
	recv(sd, &ch, sizeof(ch), 0);		
	str[strlen(str)] = (char)ch;	
}