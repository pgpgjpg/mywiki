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
#include "menu.h"

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
	LPMENU lpMenu;

	int sd = *((int*) arg);	

	menuCreate(&lpMenu);
	menuRun(lpMenu, sd);
	menuDestroy(lpMenu);
	close(sd);
	return NULL;
}