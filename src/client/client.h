#ifndef _CLIENT_H
#define _CLIENT_H
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>
#define MAXDATASIZE 3000
#define IP_ADDR "127.0.0.1"
typedef struct Client{
    struct Client *m_this; // 자신을 가리키는 포인터 변수
    int m_sockfd, m_new_fd;
    struct sockaddr_in m_server_addr;  
    char *m_buf[MAXDATASIZE];
    pthread_t m_tid1, m_tid2;
         
    void (*set)(struct Client *);
    void (*run)(struct Client *);
    void* (*recv)(void *);
    void* (*send)(void *);
    void (*deleteClient)(struct Client *);    
}Client;

Client *newClient();
void deleteClient_(Client *);   
void set_(Client *);
void run_(Client *);
void* recv_(void *);
void* send_(void *);

int linux_kbhit(void);
void cleanup(void *arg);

#endif