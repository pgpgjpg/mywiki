#ifndef _SERVER_H
#define _SERVER_H
#include <arpa/inet.h>
#include "map.h"
#include "db.h"
#include "finder.h"
#define BACKLOG 1024
#define MAX_LENGTH 1024
#define R_QUES 5
#define R_ANS 7
#define R_TAG 9

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
void server_delete(int sd, Map *lpMap, DB *lpDB);
void drawBlank(Map *lpMap, int r, int len);
void drawQues(char* ques, Map *lpMap, int r, int c);
void sendQues(char* ques, int sd, Map *lpMap, int r, int c);
void syncAnswer(char* ans, int sd, Map *lpMap, int r, int c);
void hash2arr(LPARRAY lpArr, LPHASH lpHash, int type);
void drawTags(LPARRAY lpArr, Map *lpMap, int r, int c);
void drawInfoFromDB(char *buf, Finder *lpFinder);

#endif