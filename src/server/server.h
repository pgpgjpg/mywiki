#ifndef _SERVER_H
#define _SERVER_H
#include <arpa/inet.h>
#define BACKLOG 1024
typedef struct Server{
    struct Server *m_this; // 자신을 가리키는 포인터 변수
    int m_sockfd, m_new_fd;
    struct sockaddr_in m_server_addr, m_client_addr;      
    
    void (*set)(struct Server *);    
    void (*run)(struct Server *);    
 
    void (*deleteServer)(struct Server *);    
}Server;

Server *newServer();
void deleteServer_(Server *);   
void set_(Server *);
void run_(Server *);
void* start_main(void* arg);

#endif