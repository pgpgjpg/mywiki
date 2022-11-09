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
		return;
		//exit(1);
	}

	if(setsockopt(lpServer->m_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		return;
		//exit(1);
	}

	lpServer->m_server_addr.sin_family = AF_INET;
	lpServer->m_server_addr.sin_port = htons(60000);
	lpServer->m_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(lpServer->m_server_addr.sin_zero), '\0', 8);

	if(bind(lpServer->m_sockfd, (struct sockaddr *)&lpServer->m_server_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		return;
		//exit(1);
	}

	if(listen(lpServer->m_sockfd, BACKLOG) == -1) 
	{
		return;
		//exit(1);
	}
}

void run_(Server *lpServer)
{
    int sin_size;
    pthread_t tid;

	printf("Waiting for client...\n");
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
	int nMenu = 5;
	char key,  menus[5][10] = {"Search", "Upload", "Revise", "Delete", "Exit"};
	int sd = *(int*)arg, menuIdx = 0;
	Map *lpMap = newMap(); 		
    DB *lpDB = newDB();
	lpDB->load(lpDB);	

    //무한 반복을 하면서 처리를 메뉴를 입력 반는다.    
    while (1) {
		lpMap->drawFrame(lpMap);
		drawQues("Search", lpMap, (int)(lpMap->m_rows*1/7 + 0.5), lpMap->m_cols/2);
		drawQues("Upload", lpMap, (int)(lpMap->m_rows*2/7 + 0.5), lpMap->m_cols/2);
		drawQues("Revise", lpMap, (int)(lpMap->m_rows*3/7 + 0.5), lpMap->m_cols/2);
		drawQues("Delete", lpMap, (int)(lpMap->m_rows*4/7 + 0.5), lpMap->m_cols/2);
		drawQues("Exit", lpMap, (int)(lpMap->m_rows*5/7 + 0.5), lpMap->m_cols/2);
        char *str = strstr(lpMap->m_map, menus[menuIdx]);
        if(str != NULL) *(str - 2) = '*';
		
        send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);        

		keyCallback(sd, &key);
		switch(key){
			case 'u' : 				
			case 'l' : 
				if(menuIdx != 0) menuIdx--; 
		 		else menuIdx = nMenu-1;
				break;
			case 'd' : 				
			case 'r' : 
				if(menuIdx != nMenu-1) menuIdx++;
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
					server_revise(sd, lpMap, lpDB);
					sleep(1);
				}else if(menuIdx == 3){
					// Revise Alg
					printf("Client accessed \"Delete\"\n");
					server_delete(sd, lpMap, lpDB);
					sleep(1);
				}else{
					// Exit
					lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
					sendQues("Exit", sd, lpMap, R_QUES, lpMap->m_cols/2);
					sleep(1);
					send(sd, "end", 3, 0);
					return NULL;
					//exit(1);
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

	// Map에 질문 작성하여 client로 전달
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	sendQues("Enter hash tag to search", sd, lpMap, R_QUES, lpMap->m_cols/2);

	LPARRAY lpArr;
	arrayCreate(&lpArr);
	while(1){
		// 질문에 대한 답 받아오기 client -> server
		char ans[MAX_LENGTH] = "";		
		syncAnswer(ans, sd, lpMap, R_ANS, lpMap->m_cols/2);		

		// 답변 없이 Enter 입력 시, loop break
		if(strlen(ans) == 1) break;
		ans[strlen(ans) - 1] = '\0';

		// 현재 DB에 입력한 Tag를 포함하는 데이터가 있는지 조회
		if(!lpFinder->searchTitleByTag(lpFinder, lpDB->m_lpHashTag, ans)){	
			drawBlank(lpMap, R_QUES, lpMap->m_cols - 6);			
			drawBlank(lpMap, R_ANS, lpMap->m_cols - 6);			
			drawQues("The tag you put is not in DB, please try again", lpMap, R_QUES, lpMap->m_cols/2);				
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
			//sleep(1);
			continue;
		} 
		
		lpArr->size = 0;
		// copy hash data to array data (type -> 0:key 1:val 2:arr)
		hash2arr(lpArr, lpFinder->m_lpHashTitle, 0);		
	
		// 작성한 Tags들 Map에 표시
		lpMap->clearMap(lpMap);
		drawQues("If you want to add tag, enter another tag", lpMap, R_QUES, lpMap->m_cols/2);		
		drawQues("If not, just put \"Enter\" key", lpMap, R_QUES+1, lpMap->m_cols/2);				
		drawTags(lpArr, lpMap,R_TAG, lpMap->m_cols/2);
		drawBlank(lpMap, R_ANS, lpMap->m_cols - 6);		
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);		
	}		

	// 찾고자 하는 데이터의 제목 입력
	drawBlank(lpMap, R_QUES, lpMap->m_cols - 6);			
	drawBlank(lpMap, R_QUES+1, lpMap->m_cols - 6);			
	drawQues("Enter title", lpMap, R_QUES, lpMap->m_cols/2);						
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	
	// 입력한 제목의 데이터를 DB로부터 불러오기
	while(1){
		char ans[MAX_LENGTH] = "";		
		char title[MAX_LENGTH] = "";
		syncAnswer(ans, sd, lpMap, R_ANS, lpMap->m_cols/2);
		ans[strlen(ans) - 1] = '\0';
		strcpy(title, ans);
		
		if(!lpFinder->loadDataByTitle(lpFinder, lpDB->m_lpData, lpDB->m_nData, title)){			
			drawBlank(lpMap, R_ANS, lpMap->m_cols - 6);			
			drawQues("The title you put is not in DB, please try again", lpMap, R_QUES, lpMap->m_cols/2);				
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
			continue;
		}

		char buf[MAX_LENGTH] = "";
		drawInfoFromDB(buf, lpFinder);
		send(sd, buf, strlen(buf), 0);		

		break;
	}	

	// 예제 파일 열기 여부 수신
	int key;
	while(1){
		recv(sd, &key, sizeof(key), 0);
		if(key == 'y' || key == 'Y' || key == 'n' || key == 'N')
			break;
	}

	if(key == 'y'){
		char buf[MAXDATASIZE] = "";
		FILE *fp = fopen(lpFinder->m_data.file, "r");	
		if (NULL == fp) {
			printf("%s 파일을 열수 없습니다\n", "db.txt");
			lpMap->clearMap(lpMap);
			sendQues("There is not file in DB", sd, lpMap, R_QUES, lpMap->m_cols/2);
			return;
		}    					
		if(fread(buf, 1, MAXDATASIZE, fp) == 0) {
			printf("%s 파일을 읽을 수 없습니다\n", "db.txt");
			lpMap->clearMap(lpMap);
			sendQues("There is not file in DB", sd, lpMap, R_QUES, lpMap->m_cols/2);
			return;
		}
		fclose(fp);
		send(sd, buf, strlen(buf), 0);	

		while(1){
			recv(sd, &key, sizeof(key), 0);
			if(key == '\n')
				break;
		}		

		lpMap->clearMap(lpMap); 
		drawQues("Do you want to save example file?[y/n] --> ", lpMap, R_QUES, lpMap->m_cols/2);
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);	

		// 예제 파일 저장 여부 수신		
		while(1){
			recv(sd, &key, sizeof(key), 0);
			if(key == 'y' || key == 'Y' || key == 'n' || key == 'N')
				break;
		}
		if(key == 'y'){			
			char tmp[MAXTITLESIZE];
			strcpy(tmp, lpFinder->m_data.title);
			strcat(tmp, ".example.c^"); // ^ : save 명령							
			send(sd, tmp, strlen(tmp), 0);	
			usleep(500000);					
			send(sd, buf, strlen(buf), 0);	
			usleep(500000);					
			lpMap->clearMap(lpMap); 	
			sendQues("saving...", sd, lpMap, R_QUES, lpMap->m_cols/2);	
			sleep(1);
			lpMap->clearMap(lpMap); 	
			sendQues("complete", sd, lpMap, R_QUES, lpMap->m_cols/2);	
		}	
	}	
}

void server_upload(int sd, Map *lpMap, DB *lpDB) 
{
	Data uploadData;
	uploadData.title = (char*)malloc(sizeof(char)*MAX_LENGTH);
	uploadData.data = (char*)malloc(sizeof(char)*MAX_LENGTH);
	uploadData.file = (char*)malloc(sizeof(char)*MAX_LENGTH);
	
	// 업로드할 제목 입력	
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	sendQues("Enter title to upload", sd, lpMap, R_QUES, lpMap->m_cols/2);	
	
	char ans[MAX_LENGTH] = "";
	syncAnswer(ans, sd, lpMap, R_ANS, lpMap->m_cols/2);	
	ans[strlen(ans) - 1] = '\0';	
	strcpy(uploadData.title, ans);
	//printf("here?\n");
	if(lpDB->isInDB(lpDB, uploadData.title)){
		lpMap->clearMap(lpMap);
		sendQues("The title you wrote is already in DB", sd, lpMap, R_QUES, lpMap->m_cols/2);	
		return;
	}

	// 업로드할 내용 입력	
	lpMap->clearMap(lpMap);
	sendQues("Enter data", sd, lpMap, R_QUES, lpMap->m_cols/2);	

	memset(ans, 0, MAX_LENGTH);
	syncAnswer(ans, sd, lpMap, R_ANS, lpMap->m_cols/2);		
	ans[strlen(ans) - 1] = '\0';	
	strcpy(uploadData.data, ans);

	// Hash tag 입력 질문 전달	
	lpMap->clearMap(lpMap); 
	sendQues("Enter tags", sd, lpMap, R_QUES, lpMap->m_cols/2);		
	
	LPARRAY lpArr;
	arrayCreate(&lpArr);		
	char *addVal;
	while(1){
		// hash tag 입력 client->server
		memset(ans, 0, MAX_LENGTH);
		syncAnswer(ans, sd, lpMap, R_ANS, lpMap->m_cols/2);			
		if(strlen(ans) == 1) break;
		ans[strlen(ans) - 1] = '\0';

		// 입력받은 hash tag들을 array에 저장
		addVal = (char*)malloc(sizeof(char)*strlen(ans));
		strcpy(addVal, ans);
		arrayAdd(lpArr, addVal);						

		// 현재까지 입력받은 hash tag들을 client에 전달
		int rForDisplay = 0;
		lpMap->clearMap(lpMap);
		drawQues("Enter tags", lpMap, R_QUES, lpMap->m_cols/2);
		drawTags(lpArr, lpMap, R_TAG, lpMap->m_cols/2);		
		drawBlank(lpMap, R_ANS, lpMap->m_cols - 6);		
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	}	
	uploadData.tags = lpArr;	

	// 첨부파일 입력		
	lpMap->clearMap(lpMap); 
	drawQues("If you want to upload file, enter file path+name", lpMap, R_QUES, lpMap->m_cols/2);	
	drawQues("If not, just push enter key", lpMap, R_QUES+1, lpMap->m_cols/2);			
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);

	memset(ans, 0, MAX_LENGTH);
	syncAnswer(ans, sd, lpMap, R_ANS+1, lpMap->m_cols/2);	
	if(strlen(ans) == 1) strcpy(uploadData.file, "NULL");
	else{
		char tmp[MAXTITLESIZE] = "";
		char buffer[MAXDATASIZE] = "";
		ans[strlen(ans) - 1] = '\0';

		strcpy(uploadData.file, ans);		
		strcpy(tmp, uploadData.file);
		strcat(tmp, "#");

		send(sd, tmp, strlen(tmp), 0);						
		recv(sd, buffer, MAXDATASIZE, 0);
		
		if(strcmp(buffer, "NULL")){
			char tmpTitle[MAXTITLESIZE] = "";
			strcpy(tmpTitle, uploadData.file);			
			strcpy(uploadData.file, EXAMPLE_PATH);
			strcat(uploadData.file, tmpTitle);
			
			FILE *fp = fopen(uploadData.file, "w");				
			lpMap->clearMap(lpMap); 	
			sendQues("saving...", sd, lpMap, R_QUES, lpMap->m_cols/2);	
			sleep(1);		
			
			fwrite(buffer, sizeof(char), strlen(buffer), fp);	
			lpMap->clearMap(lpMap); 	
			sendQues("complete", sd, lpMap, R_QUES, lpMap->m_cols/2);	
			fclose(fp);
		}else{
			strcpy(uploadData.file, "NULL");
		}
	}

	lpDB->save(lpDB, &uploadData);
	free(uploadData.title);
	free(uploadData.data);
	free(uploadData.file);
}
void server_revise(int sd, Map *lpMap, DB *lpDB) 
{
	Finder *lpFinder = newFinder();
    lpFinder->initFinder(lpFinder);

	// Map에 질문 작성하여 client로 전달
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	sendQues("Enter hash tag to search", sd, lpMap, R_QUES, lpMap->m_cols/2);

	LPARRAY lpArr;
	arrayCreate(&lpArr);
	while(1){
		// 질문에 대한 답 받아오기 client -> server
		char ans[MAX_LENGTH] = "";		
		syncAnswer(ans, sd, lpMap, R_ANS, lpMap->m_cols/2);		

		// 답변 없이 Enter 입력 시, loop break
		if(strlen(ans) == 1) break;
		ans[strlen(ans) - 1] = '\0';

		// 현재 DB에 입력한 Tag를 포함하는 데이터가 있는지 조회
		if(!lpFinder->searchTitleByTag(lpFinder, lpDB->m_lpHashTag, ans)){	
			drawBlank(lpMap, R_QUES, lpMap->m_cols - 6);			
			drawBlank(lpMap, R_ANS, lpMap->m_cols - 6);			
			drawQues("The tag you put is not in DB, please try again", lpMap, R_QUES, lpMap->m_cols/2);				
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
			//sleep(1);
			continue;
		} 
		
		lpArr->size = 0;
		// copy hash data to array data (type -> 0:key 1:val 2:arr)
		hash2arr(lpArr, lpFinder->m_lpHashTitle, 0);		
	
		// 작성한 Tags들 Map에 표시
		lpMap->clearMap(lpMap);
		drawQues("If you want to add tag, enter another tag", lpMap, R_QUES, lpMap->m_cols/2);		
		drawQues("If not, just put \"Enter\" key", lpMap, R_QUES+1, lpMap->m_cols/2);				
		drawTags(lpArr, lpMap,R_TAG, lpMap->m_cols/2);
		drawBlank(lpMap, R_ANS, lpMap->m_cols - 6);		
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);		
	}		

	// 찾고자 하는 데이터의 제목 입력
	drawBlank(lpMap, R_QUES, lpMap->m_cols - 6);			
	drawBlank(lpMap, R_QUES+1, lpMap->m_cols - 6);	
	drawQues("Enter title", lpMap, R_QUES, lpMap->m_cols/2);						
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	
	Data reviseData;
	reviseData.title = (char*)malloc(sizeof(char)*MAX_LENGTH);
	reviseData.data = (char*)malloc(sizeof(char)*MAX_LENGTH);
	reviseData.file = (char*)malloc(sizeof(char)*MAX_LENGTH);
	char ans[MAX_LENGTH] = "";
	// 입력한 제목의 데이터를 DB로부터 불러오기	
	while(1){
		syncAnswer(ans, sd, lpMap, R_ANS, lpMap->m_cols/2);
		ans[strlen(ans) - 1] = '\0';
		strcpy(reviseData.title, ans);
		
		if(!lpDB->isInDB(lpDB, reviseData.title)){
			drawBlank(lpMap, R_ANS, lpMap->m_cols - 6);			
			drawQues("The title you put is not in DB, please try again", lpMap, R_QUES, lpMap->m_cols/2);				
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
			continue;
		}
	
		break;
	}	
	
	// 업로드할 내용 입력	
	lpMap->clearMap(lpMap);
	sendQues("Enter data", sd, lpMap, R_QUES, lpMap->m_cols/2);	

	memset(ans, 0, MAX_LENGTH);
	syncAnswer(ans, sd, lpMap, R_ANS, lpMap->m_cols/2);		
	ans[strlen(ans) - 1] = '\0';	
	strcpy(reviseData.data, ans);

	// Hash tag 입력 질문 전달	
	lpMap->clearMap(lpMap); 
	sendQues("Enter tags", sd, lpMap, R_QUES, lpMap->m_cols/2);			
	
	lpArr->size = 0;	
	char *addVal;
	while(1){
		// hash tag 입력 client->server
		memset(ans, 0, MAX_LENGTH);
		syncAnswer(ans, sd, lpMap, R_ANS, lpMap->m_cols/2);			
		if(strlen(ans) == 1) break;
		ans[strlen(ans) - 1] = '\0';

		// 입력받은 hash tag들을 array에 저장
		addVal = (char*)malloc(sizeof(char)*strlen(ans));
		strcpy(addVal, ans);
		arrayAdd(lpArr, addVal);						

		// 현재까지 입력받은 hash tag들을 client에 전달
		int rForDisplay = 0;
		lpMap->clearMap(lpMap);
		drawQues("If you want to add tag, enter another tag", lpMap, R_QUES, lpMap->m_cols/2);		
		drawQues("If done, just put Enter key", lpMap, R_QUES+1, lpMap->m_cols/2);		
		//drawQues("If not, just put \"Enter\" key", lpMap, R_QUES+1, lpMap->m_cols/2);	
		drawTags(lpArr, lpMap, R_TAG, lpMap->m_cols/2);		
		drawBlank(lpMap, R_ANS, lpMap->m_cols - 6);		
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);	
	}	
	reviseData.tags = lpArr;	

	// 첨부파일 입력		
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거	
	drawQues("If you want to upload file, enter file path+name", lpMap, R_QUES, lpMap->m_cols/2);	
	drawQues("If not, just push enter key", lpMap, R_QUES+1, lpMap->m_cols/2);			
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);

	memset(ans, 0, MAX_LENGTH);
	syncAnswer(ans, sd, lpMap, R_ANS + 1, lpMap->m_cols/2);	
	if(strlen(ans) == 1) strcpy(reviseData.file, "NULL");
	else{
		char tmp[MAXTITLESIZE] = "";
		char buffer[MAXDATASIZE] = "";
		ans[strlen(ans) - 1] = '\0';

		strcpy(reviseData.file, ans);		
		strcpy(tmp, reviseData.file);
		strcat(tmp, "#");

		send(sd, tmp, strlen(tmp), 0);					
		recv(sd, buffer, MAXDATASIZE, 0);

		if(strcmp(buffer, "NULL")){
			char tmpTitle[MAXTITLESIZE] = "";
			strcpy(tmpTitle, reviseData.file);			
			strcpy(reviseData.file, EXAMPLE_PATH);
			strcat(reviseData.file, tmpTitle);
			
			FILE *fp = fopen(reviseData.file, "w");						
			lpMap->clearMap(lpMap); 	
			sendQues("saving...", sd, lpMap, R_QUES, lpMap->m_cols/2);	
			sleep(1);
			fwrite(buffer, sizeof(char), strlen(buffer), fp);	
			lpMap->clearMap(lpMap); 	
			sendQues("complete", sd, lpMap, R_QUES, lpMap->m_cols/2);			
			fclose(fp);		
		}else{
			strcpy(reviseData.file, "NULL");
		}
	}

	lpDB->save(lpDB, &reviseData);
	free(reviseData.title);
	free(reviseData.data);
	free(reviseData.file);
}

void server_delete(int sd, Map *lpMap, DB *lpDB)
{
	Finder *lpFinder = newFinder();
    lpFinder->initFinder(lpFinder);

	// Map에 질문 작성하여 client로 전달
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	sendQues("Enter hash tag to search", sd, lpMap, R_QUES, lpMap->m_cols/2);

	LPARRAY lpArr;
	arrayCreate(&lpArr);
	while(1){
		// 질문에 대한 답 받아오기 client -> server
		char ans[MAX_LENGTH] = "";		
		syncAnswer(ans, sd, lpMap, R_ANS, lpMap->m_cols/2);		

		// 답변 없이 Enter 입력 시, loop break
		if(strlen(ans) == 1) break;
		ans[strlen(ans) - 1] = '\0';

		// 현재 DB에 입력한 Tag를 포함하는 데이터가 있는지 조회
		if(!lpFinder->searchTitleByTag(lpFinder, lpDB->m_lpHashTag, ans)){	
			drawBlank(lpMap, R_QUES, lpMap->m_cols - 6);			
			drawBlank(lpMap, R_ANS, lpMap->m_cols - 6);			
			drawQues("The tag you put is not in DB, please try again", lpMap, R_QUES, lpMap->m_cols/2);				
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
			//sleep(1);
			continue;
		} 
		
		lpArr->size = 0;
		// copy hash data to array data (type -> 0:key 1:val 2:arr)
		hash2arr(lpArr, lpFinder->m_lpHashTitle, 0);		
	
		// 작성한 Tags들 Map에 표시
		lpMap->clearMap(lpMap);
		drawQues("If you want to add tag, enter another tag", lpMap, R_QUES, lpMap->m_cols/2);		
		drawQues("If not, just put \"Enter\" key", lpMap, R_QUES+1, lpMap->m_cols/2);					
		drawTags(lpArr, lpMap,R_TAG, lpMap->m_cols/2);
		drawBlank(lpMap, R_ANS, lpMap->m_cols - 6);		
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);		
	}		

	// 찾고자 하는 데이터의 제목 입력
	drawBlank(lpMap, R_QUES, lpMap->m_cols - 6);			
	drawBlank(lpMap, R_QUES+1, lpMap->m_cols - 6);			
	drawQues("Enter title", lpMap, R_QUES, lpMap->m_cols/2);						
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
	
	
	char *remove_title = (char*)malloc(sizeof(char)*MAX_LENGTH);		
	// 입력한 제목의 데이터를 DB로부터 불러오기	
	while(1){
		char ans[MAX_LENGTH] = "";		
		char title[MAX_LENGTH] = "";
		syncAnswer(ans, sd, lpMap, R_ANS, lpMap->m_cols/2);
		ans[strlen(ans) - 1] = '\0';
		strcpy(title, ans);	
		
		if(!lpDB->isInDB(lpDB, title)){
			drawBlank(lpMap, R_ANS, lpMap->m_cols - 6);			
			drawQues("The title you wrote is not in DB, please try again", lpMap, R_QUES, lpMap->m_cols/2);				
			send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);
			continue;
		}
		strcpy(remove_title, title);
	
		break;
	}	
	
	lpDB->remove(lpDB, remove_title);	
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	sendQues("Deleted successfully", sd, lpMap, R_QUES, lpMap->m_cols/2);
	lpDB->save(lpDB, NULL);
	free(remove_title);
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

void drawQues(char* ques, Map *lpMap, int r, int c)
{	
	lpMap->drawText(lpMap, r, c - strlen(ques)/2, ques);		
}

void sendQues(char* ques, int sd, Map *lpMap, int r, int c)
{	
	lpMap->drawText(lpMap, r, c - strlen(ques)/2, ques);	
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);		
}

void syncAnswer(char* ans, int sd, Map *lpMap, int r, int c)
{
	while(1){	
		appendCharCallback(sd, ans);		
		// 개행문자 입력 시 빠져나감
		if(ans[strlen(ans) - 1] == '\n') break;		
		// backspace	
		if(strlen(ans) > 1 && (ans[strlen(ans) - 1] == 127 || ans[strlen(ans) - 1] == 8))
			memset(ans + strlen(ans) - 2, '\0', sizeof(char)*2);			
		drawBlank(lpMap, r, strlen(ans) + 2);
		lpMap->drawText(lpMap, r, c - strlen(ans)/2, ans);
		send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);	
	}
}

void hash2arr(LPARRAY lpArr, LPHASH lpHash, int type)
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
		switch(type){
			case 0:
				arrayAdd(lpArr, tmpKey);
				break;
			case 1:
				arrayAdd(lpArr, tmpVal);
				break;
			default:
				printf("error : you enter wrong key in hash2arr()\n");
				//exit(1);
		}
		
		if (NULL == pos) {
			break;
		}
	}	
}

void drawTags(LPARRAY lpArr, Map *lpMap, int r, int c)
{
    int gap = 0;
    char displayTags[MAX_LENGTH] = "";	

    for(int i = 0; i < lpArr->size; ++i){
        char *tmpTag;
        arrayGetAt(lpArr, i, (LPDATA*) &tmpTag);
        if(displayTags[0] == '\0'){
            strcpy(displayTags, tmpTag);
            lpMap->drawText(lpMap, r + gap, c - strlen(displayTags)/2, displayTags);
        }else{
            if(strlen(displayTags) + strlen(tmpTag) > lpMap->m_cols*0.8){
                gap += 2;
                memset(displayTags, 0, MAX_LENGTH);
                strcpy(displayTags, tmpTag);
                lpMap->drawText(lpMap, 9 + gap, c - strlen(displayTags)/2, displayTags);
            }else{
                strcat(displayTags, "   ");
                strcat(displayTags, tmpTag);
                lpMap->drawText(lpMap, 9 + gap, c - strlen(displayTags)/2, displayTags);				
            }
        }
    }
}

void drawBlank(Map *lpMap, int r, int len)
{
	char *blank = (char*)malloc(sizeof(char)*len);
	memset(blank, ' ', len);
	blank[len - 1] = '\0';
	lpMap->drawText(lpMap, r, lpMap->m_cols/2 - strlen(blank)/2, blank);
	free(blank);
}

void drawInfoFromDB(char *buf, Finder *lpFinder)
{	
	strcpy(buf, " Title : ");
	strcat(buf, lpFinder->m_data.title);
	strcat(buf, "\n\n");
	strcat(buf, " Decirption : ");
	strcat(buf, lpFinder->m_data.data);
	strcat(buf, "\n\n ");			
	
	for(int i = 0; i < lpFinder->m_data.tags->size; ++i){
		char *tmpTag;
		arrayGetAt(lpFinder->m_data.tags, i, (LPDATA*) &tmpTag);
		strcat(buf, "#");
		strcat(buf, tmpTag);
		strcat(buf, "  ");
	}
	strcat(buf, "\n\n");
	strcat(buf, " Do you want to open example file?[y/n] --> ");
}