#ifndef _SERVER_H
#define _SERVER_H
#include <arpa/inet.h>
#include "map.h"
#include "db.h"
#define BACKLOG 1024
#define MAX_LENGTH 1024
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
void keyCallback(int sd, char* key);
void appendCharCallback(int sd, char* str);
void server_search(int sd, Map *lpMap, DB *lpDB);
void server_upload(int sd, Map *lpMap, DB *lpDB);
void server_revise(int sd, Map *lpMap, DB *lpDB);

#endif