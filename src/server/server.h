#ifndef _SERVER_H
#define _SERVER_H

#include <arpa/inet.h>
#include "../map/map.h"
#include <sys/epoll.h>
#define BACKLOG 10
typedef struct Server{
    int sockfd, new_fd;
    struct Server* m_this;
    struct sockaddr_in server_addr, client_addr;  
    Map *map;
    char *m_map;
    struct epoll_event ev, *client_list;
    int efd;
    
    void (*setServer)(struct Server *m_this);
    void (*recv)(struct Server *m_this);
    void (*send)(struct Server *m_this);
    void (*deleteServer)(struct Server *m_this);
    
}Server;

Server *newServer();
void deleteServer_(Server *p_this);
void setServer_(Server *p_this);
void recv_(Server *p_this);
void send_(Server *p_this);
void* thread_send(void *arg);

#endif