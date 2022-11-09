#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "client.h"

Client *newClient()
{
    Client *tmp = (Client*)malloc(sizeof(Client));
    
    tmp->m_this = tmp;
    tmp->set = set_;
    tmp->run = run_;
    tmp->recv = recv_;
    tmp->send = send_;
    tmp->deleteClient = deleteClient_;

    return tmp;
}

void deleteClient_(Client *lpClient)
{    
    free(lpClient);
}

void set_(Client *lpClient)
{
	if((lpClient->m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	lpClient->m_server_addr.sin_family = AF_INET;
	lpClient->m_server_addr.sin_port = htons(60000);
	lpClient->m_server_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
	memset(&(lpClient->m_server_addr.sin_zero), '\0', 8);
	
	while(1){
		if(connect(lpClient->m_sockfd, (struct sockaddr*)&lpClient->m_server_addr, sizeof(lpClient->m_server_addr)) == -1){
			perror("CONNECT");
			printf("Waiting for server...\n");
			sleep(1);
		}else{
			break;
		}
	}
}

void run_(Client *lpClient)
{

    if(pthread_create(&lpClient->m_tid1, NULL, lpClient->send, lpClient) != 0){
		perror("pthread_create");
	}
	if(pthread_create(&lpClient->m_tid2, NULL, lpClient->recv, lpClient) != 0){
		perror("pthread_create");
	}

	pthread_join(lpClient->m_tid1, NULL);
	pthread_join(lpClient->m_tid2, NULL);
	
	close(lpClient->m_sockfd);
}

void *send_(void *arg)
{
    Client lpClient = *((Client*)arg);  

	int n;
	char buffer[1024];
	pthread_cleanup_push(cleanup, 0);
	while(1){
		int key = linux_kbhit();
		send(lpClient.m_sockfd, &key, sizeof(key), 0);		
	}
	pthread_cleanup_pop(0);
	pthread_cancel(lpClient.m_tid2);
	pthread_exit(NULL);	    
}

void *recv_(void *arg)
{
    Client lpClient = *((Client*)arg);    

	int n;
	char buffer[MAXDATASIZE];
	
	while(1){		
		n = recv(lpClient.m_sockfd, buffer, MAXDATASIZE, 0);
		if(n <= 0){
			printf("\n서버 연결 끊김\n");
			break;
		}				
		buffer[n] = '\0';
		if(!strcmp(buffer, "end")){
			break;
		} 		
		
		if(buffer[n-1] == '^'){	
			buffer[n - 1] = '\0';
			char fileData[MAXDATASIZE] = "";
			char filePath[MAXTITLESIZE] = PATH_SAVE_FILE;			
			
			strcat(filePath, buffer);
			FILE *fp = fopen(filePath, "w");	
			n = recv(lpClient.m_sockfd, fileData, MAXDATASIZE, 0);					
			fwrite(fileData, sizeof(char), n, fp);			
			fclose(fp);
		}else if(buffer[n - 1] == '#'){			
			buffer[n - 1] = '\0';		
			char fileData[MAXDATASIZE] = "";
			char filePath[MAXTITLESIZE] = PATH_SEND_FILE;				
			strcat(filePath, buffer);
		
			FILE *fp = fopen(filePath, "r");	
			
			if (NULL == fp) {	
				send(lpClient.m_sockfd, "NULL", strlen("NULL"), 0);
				continue;
			}    			
			int fLen = fread(fileData, 1, MAXDATASIZE, fp);

			if(fLen == 0) {									
				send(lpClient.m_sockfd, "NULL", strlen("NULL"), 0);
				continue;
			}

			fclose(fp);
			send(lpClient.m_sockfd, fileData, fLen, 0);		
		}else{
			buffer[n] = '\0';
			system("clear");		
			printf("%s\n", buffer);								
		}
		fflush(stdout);		
	}
	
	pthread_cancel(lpClient.m_tid1);	
	pthread_exit(NULL);    
	//exit(0);
}

void cleanup(void *arg)
{
    struct termios oldt, newt;     
    tcgetattr( STDIN_FILENO, &oldt ); 
    newt = oldt;
    newt.c_lflag |= ( ICANON | ECHO ); 
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );            
}

int linux_kbhit(void)
{
    struct termios oldt, newt; 
    int ch;
    tcgetattr( STDIN_FILENO, &oldt ); 
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO ); 
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );    
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}