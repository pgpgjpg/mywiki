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
    
    tmp->m_this = tmp;
    tmp->setServer = setServer_;
    tmp->recv = recv_;  
    tmp->send = send_;
    tmp->deleteServer = deleteServer_;  

    return tmp;
}

void deleteServer_(Server *p_this)
{
    free(p_this);
}

void setServer_(Server *p_this)
{
    if((p_this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
       perror("socket");
       exit(1);
    }
    int value = 1;
    if(setsockopt(p_this->sockfd, SOL_SOCKET, SO_REUSEADDR, (void*) &value, sizeof(int)) == -1){
        perror("setsockopt");
        exit(1);
    }

    p_this->server_addr.sin_family = AF_INET;
    p_this->server_addr.sin_port = htons(60000);
    p_this->server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&(p_this->server_addr.sin_zero), '\0', 8);

    if(bind(p_this->sockfd, (struct sockaddr *)&p_this->server_addr,
            sizeof(struct sockaddr))==-1) {
        perror("bind");
        exit(1);
    }
    if(listen(p_this->sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    p_this->client_list = (struct epoll_event *)malloc(sizeof(*p_this->client_list) * 20);
    if((p_this->efd = epoll_create(100)) < 0){
		perror("epoll_create");
		exit(1);
	}

    p_this->ev.events = EPOLLIN;
    p_this->ev.data.fd = p_this->sockfd;
    epoll_ctl(p_this->efd, EPOLL_CTL_ADD, p_this->sockfd, &p_this->ev);



    p_this->map = newMap();
    p_this->map->initMap(p_this->map);
}
void send_(Server *p_this)
{    
    int ret;
    pthread_t tid; 
    
    if((ret = pthread_create(&tid, NULL, thread_send, p_this)) != 0){
        perror("pthread_create");
    }

    pthread_detach(tid);
}

void* thread_send(void *arg)
{	  
    Server server = *((Server*)arg);	 
    
    if(send(server.new_fd, server.m_map, strlen(server.m_map), 0) == -1)
        perror("send");			

    return NULL;
}

void recv_(Server *p_this)
{
    int sin_size, n;;
    struct sockaddr_in client_addr;
    int cIdx = 0;
    int clients[100] = {0,};
    int readn;
    char buf_in[256];

    while(1) {
        if((n = epoll_wait(p_this->efd, p_this->client_list, 20, -1)) == -1){
			perror("epoll_wait");
			exit(1);
		}
        printf("It is passed from epoll\n");
        for (int i = 0;i < n; i++){
            if (p_this->client_list[i].data.fd == p_this->sockfd) {
                printf("Accept\n");
                int socklen = sizeof(p_this->client_addr);
                int client_sd = accept(p_this->sockfd, (struct sockaddr *)&p_this->client_addr, & socklen);             
                printf("server:Get\n");
                printf("server : got connection from %s \n", inet_ntoa(client_addr.sin_addr));       
                p_this->ev.events = EPOLLIN;
                p_this->ev.data.fd = client_sd;
                epoll_ctl(p_this->efd, EPOLL_CTL_ADD, client_sd, &p_this->ev);
				clients[cIdx++] = client_sd;
                printf("client sd : %d\n", client_sd);
            } else {                
                readn = read(p_this->client_list[i].data.fd, buf_in, 255);
				buf_in[readn] = '\0';
                if(readn <= 0){
                    epoll_ctl(p_this->efd, EPOLL_CTL_DEL, p_this->client_list[i].data.fd, p_this->client_list);
                    close(p_this->client_list[i].data.fd);
                    printf("Close fd\n");
                } else {
                    printf("read data %s\n", buf_in);
					if(!fork()){                        
                        printf("[0] : %d\n%d\n", clients[0], p_this->client_list[i].data.fd);
						for(int j = 0; j < cIdx; ++j){
							if(clients[j] == p_this->client_list[i].data.fd){
                                readn = strlen(p_this->map->frame);                                
								write(clients[j], p_this->map->frame, readn);		     
                            }                           
						}
					}
					
					while(waitpid(-1, NULL, WNOHANG) > 0);


                }
            }   // else-end
        }

        
        
        
    }    
}

