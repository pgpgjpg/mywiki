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
	char key;
	int sd = *(int*)arg;
	Map *lpMap = newMap(); 
	int menuIdx = 0;
    char menus[4][10] = {"Search", "Upload", "Revise", "Exit"};
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
					server_search(sd, lpMap);
				}else if(menuIdx == 1){
					// Upload Alg
					server_upload(sd, lpMap);
				}else if(menuIdx == 2){
					// Revise Alg
					server_revise(sd, lpMap);
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

void keyCallback(int sd, char *key)
{	
	static int key1, key2, key3;	
	int idx = 0, n;
	recv(sd, &key1, sizeof(key1), 0);

	if(key1 == '\n'){
		printf("enter 침\n");
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

void server_search(int sd, Map *lpMap) 
{
	char *ques = "Enter hash tag to search";
	lpMap->clearMap(lpMap); // frame만 남기고 나머지 다 제거
	lpMap->drawText(lpMap, 5, lpMap->m_cols/2 - strlen(ques)/2, ques);
	send(sd, lpMap->m_map, strlen(lpMap->m_map), 0);    
	getchar();
}

void server_upload(int sd, Map *lpMap) {}
void server_revise(int sd, Map *lpMap) {}